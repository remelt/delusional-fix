#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../mplayer/mplayer.h"
#include "../misc/misc.hpp"
#include <imgui/imgui_internal.h>
//#include "../visuals/display/display.hpp"

void draw_screen_effect(i_material* material) {
	static auto fn = find_pattern("client.dll", "55 8B EC 83 E4 ? 83 EC ? 53 56 57 8D 44 24 ? 89 4C 24 ?");
	int w, h;
	interfaces::engine->get_screen_size(w, h);
	__asm {
		push h
		push w
		push 0
		xor edx, edx
		mov ecx, material
		call fn
		add esp, 12
	}
}

void features::visuals::motion_blur(view_setup_t* setup) {
	if (!c::visuals::mbenabled)
		return;

	static float motion_blur_values[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (setup) {
		const float time_elapsed = interfaces::globals->realtime - history.last_time_update;

		const auto view_angles = setup->view;

		float current_pitch = view_angles.x;

		while (current_pitch > 180.0f)
			current_pitch -= 360.0f;
		while (current_pitch < -180.0f)
			current_pitch += 360.0f;

		float current_yaw = view_angles.y;

		while (current_yaw > 180.0f)
			current_yaw -= 360.0f;
		while (current_yaw < -180.0f)
			current_yaw += 360.0f;

		vec3_t current_side_vector;
		vec3_t current_forward_vector;
		vec3_t current_up_vector;
		math::angle_vectors(setup->view, &current_forward_vector, &current_side_vector, &current_up_vector);

		vec3_t current_position = setup->origin;
		vec3_t position_change = history.previous_pos - current_position;

		if ((position_change.length() > 30.0f) && (time_elapsed >= 0.5f)) {
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[2] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else if (time_elapsed > (1.0f / 15.0f)) {
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[2] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else if (position_change.length() > 50.0f) {
			history.no_rotational_mb_until = interfaces::globals->realtime + 1.0f;
		}
		else {
			const float horizontal_fov = setup->fov;
			const float vertical_fov = (setup->aspect_ratio <= 0.0f) ? (setup->fov) : (setup->fov / setup->aspect_ratio);
			const float viewdot_motion = current_forward_vector.dot_product(position_change);

			if (c::visuals::mbforwardEnabled)
				motion_blur_values[2] = viewdot_motion;

			const float sidedot_motion = current_side_vector.dot_product(position_change);
			float yawdiff_original = history.previous_yaw - current_yaw;
			if (((history.previous_yaw - current_yaw > 180.0f) || (history.previous_yaw - current_yaw < -180.0f)) &&
				((history.previous_yaw + current_yaw > -180.0f) && (history.previous_yaw + current_yaw < 180.0f)))
				yawdiff_original = history.previous_yaw + current_yaw;

			float yawdiff_adjusted = yawdiff_original + (sidedot_motion / 3.0f);

			if (yawdiff_original < 0.0f)
				yawdiff_adjusted = std::clamp(yawdiff_adjusted, yawdiff_original, 0.0f);
			else
				yawdiff_adjusted = std::clamp(yawdiff_adjusted, 0.0f, yawdiff_original);

			const float undampened_yaw = yawdiff_adjusted / horizontal_fov;
			motion_blur_values[0] = undampened_yaw * (1.0f - (fabsf(current_pitch) / 90.0f));

			const float pitch_compensate_mask = 1.0f - ((1.0f - fabsf(current_forward_vector[2])) * (1.0f - fabsf(current_forward_vector[2])));
			const float pitchdiff_original = history.previous_pitch - current_pitch;
			float pitchdiff_adjusted = pitchdiff_original;

			if (current_pitch > 0.0f)
				pitchdiff_adjusted = pitchdiff_original - ((viewdot_motion / 2.0f) * pitch_compensate_mask);
			else
				pitchdiff_adjusted = pitchdiff_original + ((viewdot_motion / 2.0f) * pitch_compensate_mask);


			if (pitchdiff_original < 0.0f)
				pitchdiff_adjusted = std::clamp(pitchdiff_adjusted, pitchdiff_original, 0.0f);
			else
				pitchdiff_adjusted = std::clamp(pitchdiff_adjusted, 0.0f, pitchdiff_original);

			motion_blur_values[1] = pitchdiff_adjusted / vertical_fov;
			motion_blur_values[3] = undampened_yaw;
			motion_blur_values[3] *= (fabs(current_pitch) / 90.0f) * (fabs(current_pitch) / 90.0f) * (fabs(current_pitch) / 90.0f);

			if (time_elapsed > 0.0f)
				motion_blur_values[2] /= time_elapsed * 30.0f;
			else
				motion_blur_values[2] = 0.0f;

			motion_blur_values[2] = std::clamp((fabsf(motion_blur_values[2]) - c::visuals::mbfallingMin) / (c::visuals::mbfallingMax - c::visuals::mbfallingMin), 0.0f, 1.0f) * (motion_blur_values[2] >= 0.0f ? 1.0f : -1.0f);
			motion_blur_values[2] /= 30.0f;
			motion_blur_values[0] *= c::visuals::mbrotationIntensity * .15f * c::visuals::mbstrength;
			motion_blur_values[1] *= c::visuals::mbrotationIntensity * .15f * c::visuals::mbstrength;
			motion_blur_values[2] *= c::visuals::mbrotationIntensity * .15f * c::visuals::mbstrength;
			motion_blur_values[3] *= c::visuals::mbfallingIntensity * .15f * c::visuals::mbstrength;
		}

		if (interfaces::globals->realtime < history.no_rotational_mb_until) {
			motion_blur_values[0] = 0.0f;
			motion_blur_values[1] = 0.0f;
			motion_blur_values[3] = 0.0f;
		}
		else {
			history.no_rotational_mb_until = 0.0f;
		}
		history.previous_pos = current_position;

		history.previous_pitch = current_pitch;
		history.previous_yaw = current_yaw;
		history.last_time_update = interfaces::globals->realtime;
		return;
	}

	i_material* material = interfaces::material_system->find_material("dev/motion_blur", "RenderTargets", false);
	if (material->is_error_material())
		return;

	const auto motion_blur_internal = material->find_var("$MotionBlurInternal", nullptr, false);

	motion_blur_internal->set_vec_component_value(motion_blur_values[0], 0);
	motion_blur_internal->set_vec_component_value(motion_blur_values[1], 1);
	motion_blur_internal->set_vec_component_value(motion_blur_values[2], 2);
	motion_blur_internal->set_vec_component_value(motion_blur_values[3], 3);

	const auto motion_blur_view_port_internal = material->find_var("$MotionBlurViewportInternal", nullptr, false);

	motion_blur_view_port_internal->set_vec_component_value(0.0f, 0);
	motion_blur_view_port_internal->set_vec_component_value(0.0f, 1);
	motion_blur_view_port_internal->set_vec_component_value(1.0f, 2);
	motion_blur_view_port_internal->set_vec_component_value(1.0f, 3);

	//static convar* mat_res = interfaces::console->get_convar("mat_resolveFullFrameDepth");
	//mat_res->set_value(0);

	draw_screen_effect(material);
}

void features::visuals::apply_zoom() {
	if (!interfaces::engine->is_in_game()) {
		return;
	}

	if (c::visuals::apply_zoom && g::local) {
		if ((g::local->fov() == 90 || g::local->fov_start() == 90)) {
			if (menu::checkkey(c::visuals::apply_zoom_key, c::visuals::apply_zoom_key_s)) {
				g::local->fov() = 40;
				g::local->fov_start() = 40;
			}
		}
	}
}

void features::visuals::fog() {
	static auto fog_override = interfaces::console->get_convar("fog_override"); 

	if (!c::visuals::fog) {
		fog_override->set_value(0);
		return;
	}
	fog_override->set_value(1);

	static auto fog_start = interfaces::console->get_convar("fog_start");

	fog_start->set_value(0);

	static auto fog_end = interfaces::console->get_convar("fog_end");

	fog_end->set_value(c::visuals::fog_distance);

	static auto fog_maxdensity = interfaces::console->get_convar("fog_maxdensity");

	fog_maxdensity->set_value((float)c::visuals::fog_density * 0.01f);

	int red = c::visuals::fog_color[0] * 255;
	int green = c::visuals::fog_color[1] * 255;
	int blue = c::visuals::fog_color[2] * 255;
	char buffer_color[12];
	sprintf_s(buffer_color, 12, "%i %i %i", red, green, blue);

	static auto fog_color = interfaces::console->get_convar("fog_color");

	fog_color->set_value(buffer_color);
}

void features::visuals::shadows() {
	if (!interfaces::engine->is_in_game() || !g::local || !interfaces::engine->is_connected())
		return;

	static convar* cl_csm_shadows = interfaces::console->get_convar("cl_csm_shadows");
	if (!cl_csm_shadows)
		return;

	static convar* cl_csm_rot_override = interfaces::console->get_convar("cl_csm_rot_override");
	if (!cl_csm_rot_override)
		return;

	if (!c::visuals::shadows) {
		cl_csm_shadows->set_value(1);
		cl_csm_rot_override->set_value(0);
		return;
	}

	cl_csm_shadows->set_value(1);
	cl_csm_rot_override->set_value(1);

	static convar* cl_csm_rot_x = interfaces::console->get_convar("cl_csm_rot_x");
	static convar* cl_csm_rot_y = interfaces::console->get_convar("cl_csm_rot_y");

	static float time = 0.0f;

	if (GImGui == nullptr)
		return;
	time += ImGui::GetIO().DeltaTime;

	float speed = c::visuals::shadow_rotation_speed;
	float base_y = c::visuals::shadow_rot_y;

	float dynamic_y = base_y;

	if (speed > 0.0f && c::visuals::dynamic_shadows) {
		c::visuals::shadow_rot_y = fmod(time * speed, 360.0f);
	}

	if (cl_csm_rot_x)
		cl_csm_rot_x->set_value(c::visuals::shadow_rot_x);
	if (cl_csm_rot_y)
		cl_csm_rot_y->set_value(c::visuals::shadow_rot_y);
}

void features::visuals::gravity_ragdoll() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	static auto ragdollGravity = interfaces::console->get_convar("cl_ragdoll_gravity");
	ragdollGravity->set_value(c::visuals::gravity_ragdoll ? -600 : 600);
}

bool update = false;
void features::visuals::skybox_changer() {
	if (!g::local)
		update = true;
	static auto load_skybox = reinterpret_cast<void(__fastcall*)(const char*)>(find_pattern("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
	static const auto sv_skyname = interfaces::console->get_convar("sv_skyname");
	static std::string skybox_name;
	static int saved_skybox = c::visuals::skybox;
	switch (c::visuals::skybox) {
	case 1: skybox_name = "cs_tibet"; break;
	case 2: skybox_name = "cs_baggage_skybox_"; break;
	case 3: skybox_name = "italy"; break;
	case 4: skybox_name = "jungle"; break;
	case 5: skybox_name = "office"; break;
	case 6: skybox_name = "sky_cs15_daylight01_hdr"; break;
	case 7: skybox_name = "sky_cs15_daylight02_hdr"; break;
	case 8: skybox_name = "vertigoblue_hdr"; break;
	case 9: skybox_name = "vertigo"; break;
	case 10: skybox_name = "sky_day02_05_hdr"; break;
	case 11: skybox_name = "nukeblank"; break;
	case 12: skybox_name = "sky_venice"; break;
	case 13: skybox_name = "sky_cs15_daylight03_hdr"; break;
	case 14: skybox_name = "sky_cs15_daylight04_hdr"; break;
	case 15: skybox_name = "sky_csgo_cloudy01"; break;
	case 16: skybox_name = "sky_csgo_night02"; break;
	case 17: skybox_name = "sky_csgo_night02b"; break;
	case 18: skybox_name = "sky_csgo_night_flat"; break;
	case 19: skybox_name = "sky_dust"; break;
	case 20: skybox_name = "vietnam"; break;
	}

	if (g::local && (update || saved_skybox != c::visuals::skybox)) {
		if (c::visuals::skybox == 0)
			load_skybox(sv_skyname->string);
		else
			load_skybox(skybox_name.c_str());
		saved_skybox = c::visuals::skybox;
	}
}

void features::visuals::watermark() {
	if (!c::misc::watermark)
		return;

	static float frame_rate = 0;
	frame_rate = 0.9f * frame_rate + (1.f - 0.9f) * interfaces::globals->absolute_frametime;
	int get_fps = (1.f / frame_rate);

	std::string output = "";
	std::string main = "delusional";
	output += main;

	int w, h;
	auto text_size = im_render.measure_text(output.c_str(), fonts::watermark_font, 12.f);
	static const ImVec2& padding = ImVec2(7, 7);
	static const ImVec2& margin = ImVec2(4, 3);
	static const int& box_offset = 10;
	interfaces::engine->get_screen_size(w, h);

	const std::vector<ImVec2>& points = {
		ImVec2(w - text_size.x - padding.x - margin.x * 2.f, padding.y),
		ImVec2(w - padding.x + box_offset, padding.y),
		ImVec2(w - padding.x, text_size.y + padding.y + margin.y * 2.f),
		ImVec2(w - text_size.x - padding.x - margin.x * 2.f - box_offset, text_size.y + padding.y + margin.y * 2.f),
	};

	ImColor bg_color(0.08f, 0.08f, 0.08f, 1.f);
	ImColor outline_start_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f);
	ImColor outline_end_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 0.f);

	ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(
		{ w - text_size.x - padding.x - margin.x * 2.f - 1, padding.y - 1 },
		{ w - padding.x + 1, text_size.y + padding.y + margin.y * 2.f + 1 },
		outline_start_color,
		outline_start_color,
		outline_end_color,
		outline_end_color
	);

	ImGui::GetBackgroundDrawList()->AddRectFilled(
		{ w - text_size.x - padding.x - margin.x * 2.f, padding.y },
		{ w - padding.x, text_size.y + padding.y + margin.y * 2.f },
		bg_color,
		0.f
	);

	ImGui::GetBackgroundDrawList()->AddText(fonts::watermark_font, 12.f, ImVec2(w - text_size.x - padding.x - margin.x + 1, padding.y + margin.y), ImColor(0, 0, 0, 255),main.c_str());
	ImGui::GetBackgroundDrawList()->AddText( fonts::watermark_font, 12.f, ImVec2(w - text_size.x - padding.x - margin.x, padding.y + margin.y), ImColor(255, 255, 255), main.c_str());
}

static std::chrono::steady_clock::time_point progressStartTime = std::chrono::steady_clock::now();
static double calculatedPositionMs = 0.0;
static std::string lastTitle = "";
int last;

void UpdateCalculatedTrackPosition(mPlayer& mplayer)
{
	if (lastTitle != mplayer.Title) {
		progressStartTime = std::chrono::steady_clock::now();
		lastTitle = mplayer.Title;
		calculatedPositionMs = static_cast<double>(mplayer.CurrentTime);
		last = mplayer.CurrentTime;
	}

	if (mplayer.isPlaying) {
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<double, std::milli> delta = now - progressStartTime;
		if (mplayer.CurrentTime != last) {
			calculatedPositionMs = static_cast<double>(mplayer.CurrentTime);
			last = mplayer.CurrentTime;
		}
		else {
			calculatedPositionMs += delta.count();
		}
		progressStartTime = now;
	}
}


void features::visuals::RenderMediaPlayer()
{
	if (!c::misc::show_spotify_currently_playing || c::misc::player_type == 0)
		return;

	UpdateCalculatedTrackPosition(mplayer);

	float progress = 0.0f;
	if (mplayer.TotalTime > 0) {
		progress = static_cast<float>(calculatedPositionMs) / static_cast<float>(mplayer.TotalTime);
		if (progress > 1.0f)
			progress = 1.0f;
	}

	static float smoothProgress = 0.0f;
	if (progress < smoothProgress)
		smoothProgress = progress;
	smoothProgress += (progress - smoothProgress) * 0.1f;

	static ImVec2 sz{ };
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	float m = c::misc::watermark ? 25: 0;
	ImGui::SetNextWindowPos({ x - sz.x + 2, 4.5f + m });

	ImGui::Begin("Media Player", nullptr,
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	float padding = 10.0f;
	float imageWidth = 30.0f;
	float imageHeight = 30.0f;


	auto sizex1 = ImGui::CalcTextSize(strartist.c_str()).x;
	auto sizex2 = ImGui::CalcTextSize(strtitle.c_str()).x;

	float sizey1 = y * 1.5f / 720.f;
	float sizey2 = y * 1.5f / 720.f;

	auto text_size1 = im_render.measure_text(strartist.c_str(), fonts::lb_player_font, 12.f);
	auto text_size2 = im_render.measure_text(strtitle.c_str(), fonts::lb_player_font, 12.f);

	float windowWidth = ImGui::GetWindowSize().x;

	ImGui::PushFont(fonts::lb_player_font);

	if (albumArtTexture) {
		ImGui::SetCursorPos(ImVec2(windowWidth - imageWidth - padding, 3));
		ImGui::Image(albumArtTexture, ImVec2(imageWidth, imageWidth));
	}
	if (albumArtTexture)
		ImGui::SetCursorPos({ windowWidth - imageWidth - padding - text_size1.x - padding + 1 + 2, 3 + imageHeight / 2 - (text_size1.y) / 2 + 6 + 1 });
	else
		ImGui::SetCursorPos({ windowWidth - padding - text_size1.x - padding + 1 + 2, 3 + imageHeight / 2 - (text_size1.y) / 2 + 6 + 1 });

	ImGui::TextColored(ImVec4(0.f, 0.f, 0.f, 1.f), strartist.c_str());
	if (albumArtTexture)
		ImGui::SetCursorPos({ windowWidth - imageWidth - padding - text_size1.x - padding + 2, 3 + imageHeight / 2 - (text_size1.y) / 2 + 6 });
	else
		ImGui::SetCursorPos({ windowWidth - padding - text_size1.x - padding + 2, 3 + imageHeight / 2 - (text_size1.y) / 2 + 6 });
	ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.f), strartist.c_str());
	if (albumArtTexture)
		ImGui::SetCursorPos({ windowWidth - imageWidth - padding - text_size2.x - padding + 1 + 2, 3 + imageHeight / 2 - (text_size2.y) / 2 - 6 + 1 });
	else
		ImGui::SetCursorPos({ windowWidth - padding - text_size2.x - padding + 1 + 2, 3 + imageHeight / 2 - (text_size2.y) / 2 - 6 + 1 });

	ImGui::TextColored(ImVec4(0.f, 0.f, 0.f, 1.f), strtitle.c_str());
	if (albumArtTexture)
		ImGui::SetCursorPos({ windowWidth - imageWidth - padding - text_size2.x - padding + 2, 3 + imageHeight / 2 - (text_size2.y) / 2 - 6 });
	else
		ImGui::SetCursorPos({ windowWidth - padding - text_size2.x - padding + 2, 3 + imageHeight / 2 - (text_size2.y) / 2 - 6 });
	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), strtitle.c_str());

	if (albumArtTexture && c::misc::progressbar_enable) {
		ImGui::PushItemWidth(108);
		ImGui::SetCursorPos({ sz.x - imageWidth - padding - 78, 40 });

		//should be here but idc
		//u can fix that if u want ig
		ImGui::PushStyleColor(ImGuiCol_MPlayer_ProgressbarBg, ImVec4(0.05f, 0.05f, 0.05f, 0.03f));
		ImGui::PushStyleColor(ImGuiCol_MPlayer_Progressbar, ImVec4(1.f, 1.f, 1.f, 0.85f));
		ImGui::ProgressBar(smoothProgress, ImVec2(0.0f, 2.0f));
		ImGui::PopStyleColor();
		ImGui::PopItemWidth();
	}
	ImGui::Spacing();

	ImGui::SetWindowSize({ 400, 100 });
	sz = ImGui::GetWindowSize();

	ImGui::PopFont();

	ImGui::End();
}

void features::visuals::display_spotify() {
	if (!c::misc::show_spotify_currently_playing || c::misc::player_type == 1)
		return;

	int w, h;
	std::string name;
	interfaces::engine->get_screen_size(w, h);
	h = c::misc::watermark ? 30 : 5;

	//simple check for " - " in the track name
	//cuz its usually used when artist name is in the track title
	if (strtitle.find(" - ") == std::string::npos && !strartist.empty()) {
		name = strartist + " - " + strtitle;
	}
	else {
		name = strtitle;
	}

	auto text_size = im_render.get_text_size(name.c_str(), fonts::main_spec_font, 0.f, 12.f);
	auto paused_size = im_render.get_text_size("stopped / paused", fonts::main_spec_font, 0.f, 12.f);

	if (mplayer.isPlaying) {
		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(w - 6 - text_size + 1, h + 1), ImColor(0, 0, 0, 255), name.c_str());
		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(w - 6 - text_size, h), ImColor(255, 255, 255, 255), name.c_str());
	}
	else {
		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(w - 6 - paused_size + 1, h + 1), ImColor(0, 0, 0, 255), "stopped / paused");
		ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(w - 6 - paused_size, h), ImColor(255, 255, 255, 255), "stopped / paused");
	}
}


bool once = false;
void features::visuals::removals() {
	if (!interfaces::engine->is_in_game() || !g::local) 
		return;

	static convar* shadows = interfaces::console->get_convar("cl_csm_enabled");
	static convar* postproccesing = interfaces::console->get_convar("mat_postprocess_enable");
	static convar* sky = interfaces::console->get_convar("r_3dsky");
	static convar* mat_postprocess_enable = interfaces::console->get_convar("mat_disable_bloom");
	static convar* blur = interfaces::console->get_convar("@panorama_disable_blur");
	static bool* disable_postprocessing = *reinterpret_cast<bool**>(find_pattern("client.dll", "83 EC 4C 80 3D") + 0x5);
	*disable_postprocessing = c::visuals::removals[1];

	if (c::visuals::removals[0]) {
		sky->set_value(0);
	}
	else {
		sky->set_value(1);
	}
	if (c::visuals::removals[2]) {
		shadows->set_value(0);
	}
	else {
		shadows->set_value(1);
	}
	if (c::visuals::removals[3]) {
		mat_postprocess_enable->set_value(0);
	}
	else {
		mat_postprocess_enable->set_value(1);
	}
	if (c::visuals::removals[4] && !once) {
		blur->set_value(0);
		once = true;
	}
	else if (once && !c::visuals::removals[4]) {
		blur->set_value(1);
		once = false;
	}
}

void features::visuals::fullbright() {
	static auto bright = interfaces::console->get_convar("mat_fullbright");
	bright->set_value(c::visuals::fullbright ? 1 : 0);
}

void features::visuals::flashalpha() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !g::local)
		return;

	if (c::visuals::change_flashalpha) {
		if (c::visuals::flashalpha <= 0)
			return;

		g::local->flash_alpha() = c::visuals::flashalpha;
	}
}

void features::visuals::nosmoke() {
	static auto linegoesthrusmoke = find_pattern("client.dll", "A3 ? ? ? ? 57 8B CB");
	static bool set = true;
	std::vector<const char*> vistasmoke_wireframe =  {
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
	};

	std::vector<const char*> vistasmoke_nodraw = {
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!c::visuals::nosmoke) {
		if (set) {
			for (auto material_name : vistasmoke_wireframe) {
				i_material* smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
				smoke->set_material_var_flag(material_var_flags_t::material_var_wireframe, false);
			}
			for (auto material_name : vistasmoke_nodraw) {
				i_material* smoke = interfaces::material_system->find_material(material_name, TEXTURE_GROUP_OTHER);
				smoke->set_material_var_flag(material_var_flags_t::material_var_no_draw, false);
			}
			set = false;
		}
		return;
	}

	set = true;

	for (auto mat_s : vistasmoke_wireframe) {
		i_material* smoke = interfaces::material_system->find_material(mat_s, TEXTURE_GROUP_OTHER);
		smoke->set_material_var_flag(material_var_flags_t::material_var_wireframe, true);
	}

	for (auto mat_n : vistasmoke_nodraw) {
		i_material* smoke = interfaces::material_system->find_material(mat_n, TEXTURE_GROUP_OTHER);
		smoke->set_material_var_flag(material_var_flags_t::material_var_wireframe, true);
	}

	static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x1);
	*(int*)(smokecout) = 0;
}

std::tuple<float, float, float> draw_rainbow(float speed) {
	constexpr float pi = std::numbers::pi_v<float>;
	float r = std::sin(speed * interfaces::globals->realtime) * 0.5f + 0.5f;
	float g = std::sin(speed * interfaces::globals->realtime + 2 * pi / 3) * 0.5f + 0.5f;
	float b = std::sin(speed * interfaces::globals->realtime + 4 * pi / 3) * 0.5f + 0.5f;

	r = std::pow(r, 2.2f);
	g = std::pow(g, 2.2f);
	b = std::pow(b, 2.2f);

	return std::make_tuple(r, g, b);
}

void features::visuals::jump_trail() {
	if (!g::local || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected() || !g::local->is_alive())
		return;

	if (c::visuals::trails) {
		const int move_type = g::local->move_type();

		vec3_t origin = g::local->origin();
		int vel = g::local->velocity().length_2d();

		const auto [r, g, b] { draw_rainbow(4.f) };
		if (!(g::local->flags() & fl_onground) && tick == 0) {

			color_t rainbow_col = color_t(r * 255, g * 255, b * 255);

			BeamInfo_t beam_info;
			beam_info.m_nType = 0;
			beam_info.m_pszModelName = "sprites/physbeam.vmt";
			beam_info.m_nModelIndex = -1;
			beam_info.m_flHaloScale = 0.0;
			beam_info.m_flLife = 2.5f;
			beam_info.m_flWidth = 5;
			beam_info.m_flEndWidth = 5;
			beam_info.m_flFadeLength = 0.0;
			beam_info.m_flAmplitude = 2.0;
			beam_info.m_flBrightness = 255.f;
			beam_info.m_flSpeed = 0.5;
			beam_info.m_nStartFrame = 0.;
			beam_info.m_flFrameRate = 0.;
			beam_info.m_flRed = (float)rainbow_col.r();
			beam_info.m_flGreen = (float)rainbow_col.g();
			beam_info.m_flBlue = (float)rainbow_col.b();
			beam_info.m_nSegments = 2;
			beam_info.m_bRenderable = true;
			beam_info.m_nFlags = 0;
			beam_info.m_vecStart = origin_old;
			beam_info.m_vecEnd = origin;

			Beam_t* myBeam = interfaces::render_beams->create_beam_point(beam_info);
			if (myBeam && !(move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)) {
				interfaces::render_beams->draw_beam(myBeam);
			}

			velocity_old = vel;
		}
		if (tick == 0) {
			origin_old = origin;
			tick = igonre_ticks + 1;
		}
		tick = tick - 1;
	}
}

void features::visuals::dlights(player_t* entity) {
	if (!c::visuals::dlight || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if (interfaces::engine->is_in_game() && interfaces::engine->is_connected()) {
		vec3_t getorig = entity->origin();
		vec3_t  getheadorig = entity->get_eye_pos();

		if (entity->is_enemy() && !entity->dormant()) {

			dlight_t* elight = interfaces::effects->cl_alloc_elight(entity->index());
			elight->color.r = float(c::visuals::dlight_clr[0] * 255.f);
			elight->color.g = float(c::visuals::dlight_clr[1] * 255.f);;
			elight->color.b = float(c::visuals::dlight_clr[2] * 255.f);;
			elight->color.exponent = 8.f;
			elight->direction = getheadorig;
			elight->origin = getheadorig;
			elight->radius = 200.0f;	
			elight->die_time = interfaces::globals->cur_time + 0.1f;
			elight->decay = 50.0f;		
			elight->key = entity->index();

			dlight_t* dlight = interfaces::effects->cl_alloc_dlight(entity->index());
			dlight->color.r = float(c::visuals::dlight_clr[0] * 255.f);
			dlight->color.g = float(c::visuals::dlight_clr[1] * 255.f);;
			dlight->color.b = float(c::visuals::dlight_clr[2] * 255.f);;
			dlight->color.exponent = 8.f;
			dlight->direction = getorig;
			dlight->origin = getorig;
			dlight->radius = 100.f;		
			dlight->die_time = interfaces::globals->cur_time + 0.1f;
			dlight->decay = dlight->radius / 2.f;		
			dlight->key = entity->index();
		}
	}
}

static int buttons = 0;
void features::visuals::run_freecam(c_usercmd* cmd, vec3_t angles) {
	static vec3_t currentviewangles = vec3_t{};
	static vec3_t realviewangles = vec3_t{};
	static bool wascrouching = false;
	static bool washoldingattack = false;
	static bool washoldinguse = false;
	static bool hassetangles = false;
	buttons = cmd->buttons;

	if (!c::misc::freecam || !menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s)) {
		if (hassetangles) {
			interfaces::engine->set_view_angles(realviewangles);
			cmd->view_angles = currentviewangles;
			if (wascrouching)
				cmd->buttons |= in_duck;
			if (washoldingattack)
				cmd->buttons |= in_attack;
			if (washoldinguse)
				cmd->buttons |= in_use;
			wascrouching = false;
			washoldingattack = false;
			washoldinguse = false;
			hassetangles = false;
		}
		currentviewangles = vec3_t{};
		return;
	}

	if (!g::local || !g::local->is_alive())
		return;

	if (currentviewangles.null()) {
		currentviewangles = cmd->view_angles;
		realviewangles = angles;
		wascrouching = cmd->buttons & in_duck;
	}

	cmd->forward_move = 0;
	cmd->side_move = 0;
	cmd->buttons = 0;

	if (wascrouching)
		cmd->buttons |= in_duck;

	if (washoldingattack)
		cmd->buttons |= in_attack;

	if (washoldinguse)
		cmd->buttons |= in_use;

	cmd->view_angles = currentviewangles;
	hassetangles = true;
}

void features::visuals::freecam(view_setup_t* setup) {
	static vec3_t origin = vec3_t{ };

	if (!c::misc::freecam || !GetAsyncKeyState(c::misc::freecam_key)) {
		origin = vec3_t{ };
		return;
	}

	if (!g::local || !g::local->is_alive())
		return;

	float cam_speed = fabsf(static_cast<float>(2)); // cfg later 

	if (origin.null())
		origin = setup->origin;

	vec3_t forward{ }, right{ }, up{ };

	math::angle_vectors_alternative(setup->view, &forward, &right, &up);

	const bool inback = buttons & in_back;
	const bool inforward = buttons & in_forward;
	const bool rightBtn = buttons & in_moveright;
	const bool inleft = buttons & in_moveleft;
	const bool inshift = buttons & in_speed;
	const bool induck = buttons & in_duck;
	const bool injump = buttons & in_jump;

	if (induck)
		cam_speed *= 0.45f;

	if (inshift)
		cam_speed *= 1.65f;

	if (inforward)
		origin += forward * cam_speed;

	if (rightBtn)
		origin += right * cam_speed;

	if (inleft)
		origin -= right * cam_speed;

	if (inback)
		origin -= forward * cam_speed;

	if (injump)
		origin += up * cam_speed;

	setup->origin = origin;
}
void features::visuals::key_strokes() {
	if (!c::movement::key_strokes)
		return;

	if (!g::local || !g::local->is_alive())
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	int w, h;
	interfaces::engine->get_screen_size(w, h);
	c_usercmd* cmd = g::cmd;

	color_t clr;

	if (cmd->buttons & in_moveleft && cmd->buttons & in_moveright)
		clr = color_t(0.6f, 0.2f, 0.2f);
	else
		clr = color_t(1.f, 1.f, 1.f);

	im_render.text(w / 2 + 13, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);
	im_render.text(w / 2 - 14, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);
	im_render.text(w / 2, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);
	im_render.text(w / 2 - 14, c::movement::key_strokes_position + 14, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);
	im_render.text(w / 2, c::movement::key_strokes_position + 13, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);
	im_render.text(w / 2 + 13, c::movement::key_strokes_position + 13, 12, fonts::key_strokes_font, "_", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_jump)
		im_render.text(w / 2 + 13, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "J", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_duck)
		im_render.text(w / 2 - 14, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "C", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_forward)
		im_render.text(w / 2, c::movement::key_strokes_position, 12, fonts::key_strokes_font, "W", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_moveleft)
		im_render.text(w / 2 - 14, c::movement::key_strokes_position + 14, 12, fonts::key_strokes_font, "A", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_back)
		im_render.text(w / 2, c::movement::key_strokes_position + 13, 12, fonts::key_strokes_font, "S", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->buttons & in_moveright)
		im_render.text(w / 2 + 13, c::movement::key_strokes_position + 13, 12, fonts::key_strokes_font, "D", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->mouse_dx < 0.f)
		im_render.text(w / 2 - 14, c::movement::key_strokes_position + 28, 12, fonts::key_strokes_font, "<", true, color_t(1.f, 1.f, 1.f), true);

	if (cmd->mouse_dx > 0.f)
		im_render.text(w / 2 + 13, c::movement::key_strokes_position + 28, 12, fonts::key_strokes_font, ">", true, color_t(1.f, 1.f, 1.f), true);
}

void features::visuals::console() {
	static i_material* material[5];
	if (!material[0] || !material[1] || !material[2] || !material[3] || !material[4]) {
		for (material_handle_t h = interfaces::material_system->first_material(); h != interfaces::material_system->invalid_material_handle(); h = interfaces::material_system->next_material(h)) {
			const auto mat = interfaces::material_system->get_material(h);

			if (!mat)
				continue;

			if (strstr(mat->get_name(), "vgui_white"))
				material[0] = mat;
			else if (strstr(mat->get_name(), "800corner1"))
				material[1] = mat;
			else if (strstr(mat->get_name(), "800corner2"))
				material[2] = mat;
			else if (strstr(mat->get_name(), "800corner3"))
				material[3] = mat;
			else if (strstr(mat->get_name(), "800corner4"))
				material[4] = mat;
		}
	}
	else {
		for (unsigned int num = 0; num < 5; num++) {
			if (!c::misc::custom_console || !interfaces::engine->is_console_visible()) {
				material[num]->color_modulate(1.f, 1.f, 1.f);
				material[num]->alpha_modulate(1.f);
				continue;
			}

			material[num]->color_modulate(c::misc::custom_console_clr[0], c::misc::custom_console_clr[1], c::misc::custom_console_clr[2]);
			material[num]->alpha_modulate(c::misc::custom_console_clr[3]);
		}
	}
}
