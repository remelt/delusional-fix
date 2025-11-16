#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"
#include "../../includes/imgui/imgui_internal.h"
#include "../../includes/json/json.h"

#undef max;
#undef min;

float LerpFloatReal(float a, float b, float t)
{
	return a + (b - a) * t;
}

struct counter {
	float alpha = 0.0f;
	std::string info;

	void update(bool shouldShow, float deltaTime) {
		float targetAlpha = shouldShow ? 1.0f : 0.0f;
		float lerp_factor = 1.0f - std::exp(-6.f * deltaTime);

		alpha += (targetAlpha - alpha) * lerp_factor;
		alpha = std::clamp(alpha, 0.0f, 1.0f);
	}
};
static counter anim;

struct frame {
	float viewangles[2];
	float forwardmove;
	float sidemove;
	float upmove;
	int buttons;
	short mousedx;
	short mousedy;
	vec3_t position;

	frame(c_usercmd* cmd, vec3_t pos) {
		this->viewangles[0] = cmd->view_angles.x;
		this->viewangles[1] = cmd->view_angles.y;
		this->forwardmove = cmd->forward_move;
		this->sidemove = cmd->side_move;
		this->upmove = cmd->up_move;
		this->buttons = cmd->buttons;
		this->mousedx = cmd->mouse_dx;
		this->mousedy = cmd->mouse_dy;
		this->position = pos;
	}

	void replay(c_usercmd* cmd) {
		cmd->view_angles.x = this->viewangles[0];
		cmd->view_angles.y = this->viewangles[1];
		cmd->forward_move = this->forwardmove;
		cmd->side_move = this->sidemove;
		cmd->up_move = this->upmove;
		cmd->buttons = this->buttons;
		cmd->mouse_dx = this->mousedx;
		cmd->mouse_dy = this->mousedy;
	}

	void replay_wo_viewangles(c_usercmd* cmd) {
		cmd->forward_move = this->forwardmove;
		cmd->side_move = this->sidemove;
		cmd->up_move = this->upmove;
		cmd->buttons = this->buttons;
		cmd->mouse_dx = this->mousedx;
		cmd->mouse_dy = this->mousedy;
	}
};

typedef std::vector<frame> framecontainer;

struct fullroute {
	std::string routename;
	std::string startwpn;
	framecontainer frames;
};

std::vector<fullroute> mapWithRoutes;

class recorder_t {
private:
	bool is_recording_active = false;
	framecontainer recording_frames;

public:
	void startrecording() {
		this->is_recording_active = true;
	}

	void stoprecording() {
		this->is_recording_active = false;
	}

	bool isrecordingactive() const {
		return this->is_recording_active;
	}

	framecontainer& getactiverecording() {
		return this->recording_frames;
	}

	void setrecording(framecontainer frames) {
		this->recording_frames = frames;
	}
};

class playback_t {
private:
	bool is_playback_active = false;
	size_t current_frame = 0;
	int start_tick = 0;
	const framecontainer& active_demo = framecontainer();

public:
	void startplayback(framecontainer& frames) {
		this->is_playback_active = true;

		uintptr_t active_demoptr = (uintptr_t)&this->active_demo;
		*(framecontainer*)active_demoptr = frames;
	};

	void stopplayback() {
		this->is_playback_active = false;
		this->current_frame = 0;
		this->start_tick = 0;
	};

	bool isplaybackactive() const {
		return this->is_playback_active;
	}

	size_t getcurrentframe() const {
		return this->current_frame;
	};

	void setcurrentframe(size_t frame) {
		this->current_frame = frame;
	};

	const framecontainer& getactivedemo() {
		return this->active_demo;
	}
	void set_start_tick(int tick) { start_tick = tick; }
	int get_start_tick() const { return start_tick; }
};

playback_t playback;
recorder_t rec;

char filename[32] = "";

int curroute = -1;

static char buffer[32];
static char buffer1[32];

static int delselection;

std::string routestodelete = "";
std::string routelist = "";

void mrecorder::forcestop() {
	playback.stopplayback();
}

void mrecorder::draw() {

	int flags;
	flags = ImGuiInputTextFlags_EnterReturnsTrue;

	auto& routeItems = route->getRoutes();

	static int currentRoute = -1;

	if (static_cast<std::size_t>(currentRoute) >= routeItems.size())
		currentRoute = -1;

	ImGui::Text(("map files"));

	ImGui::ListBoxHeader("", routeItems.size(), routeItems.size());
	for (int i = 0; i < routeItems.size(); i++) {
		bool isSelected = (currentRoute == i);
		if (ImGui::Selectable(routeItems[i].c_str(), isSelected))
			currentRoute = i;
	}
	ImGui::ListBoxFooter();

	if (interfaces::engine->is_in_game()) {

		if (ImGui::Button("create map file", ImVec2(-1, 15))) {
			std::string routname = interfaces::engine->get_level_name() + std::to_string(int(1.0f / interfaces::globals->interval_per_tick));
			route->add(routname.c_str());
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}
	}

	if (currentRoute != -1) {

		if (ImGui::Button("load selected map file", ImVec2(-1, 15))) {
			route->load(currentRoute);
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}


		if (ImGui::Button("delete selected map file", ImVec2(-1, 15))) {
			route->remove(currentRoute);
			currentRoute = -1;
			ZeroMemory(buffer, 32);
			ZeroMemory(buffer1, 32);
		}

		if (ImGui::Button("clear routes from map file", ImVec2(-1, 15))) {
			if (!mapWithRoutes.empty())
				mapWithRoutes.clear();
		}

		if (ImGui::Button("reload records", ImVec2(-1, 15)))
		route->listroutes();

		if (!mapWithRoutes.empty()) {

			ImGui::PushItemWidth(-1);

			static int route_index = -1;

			ImGui::Text(("routes"));

			if (ImGui::ListBoxHeader(("##rout3s"), mapWithRoutes.size(), mapWithRoutes.size())) {
				for (int i = 0; i < mapWithRoutes.size(); ++i) {
					std::string const& route_name = mapWithRoutes[i].routename;
					if (ImGui::Selectable(route_name.c_str(), i == route_index)) {
						route_index = i;
					}
				}
				ImGui::ListBoxFooter();
			}
			ImGui::PopItemWidth();

			if (ImGui::InputTextWithHint(("##route name"), ("route name"), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
				route->save(currentRoute, buffer);
				ZeroMemory(buffer, 32);
			}

			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("press enter to create a new record");
			}

			//for buffer
			if (currentRoute != -1) {

				if (ImGui::Button("save route", ImVec2(-1, 15))) {
					route->save(currentRoute, buffer);
					ZeroMemory(buffer, 32);
				}
			}

			if (ImGui::Button("delete route", ImVec2(-1, 15))) {
				if (route_index >= 0 && route_index < mapWithRoutes.size()) {
					mapWithRoutes.erase(mapWithRoutes.begin() + route_index);
				}
			}
		}
	}

	framecontainer& recording = rec.getactiverecording();
	int currentFrame = playback.getcurrentframe();
	int maxFrames = recording.size();
	ImGui::Text(std::string(("frames: ") + std::to_string(currentFrame) + (" / ") + std::to_string(maxFrames)).c_str());
	if (rec.isrecordingactive())
		ImGui::Text(("recording is active"));
	if (playback.isplaybackactive())
		ImGui::Text(("playback is active"));;
}

//TODO: FINISH THE ANIMATION LOGIC

static std::vector<float> point_progress;
float start_time = 0.0f;
//static std::vector<float> start_time;
//static std::vector<float> line_progress;
void mrecorder::drawroute() {
	if (!c::misc::movement_rec)
		return;
	if (point_progress.size() != mapWithRoutes.size())
		point_progress.resize(mapWithRoutes.size(), 0.0f);

	//if (start_time.size() != mapWithRoutes.size())
	//	start_time.resize(mapWithRoutes.size(), 0.0f);

	//if (line_progress.size() != mapWithRoutes.size())
	//	line_progress.resize(mapWithRoutes.size(), 0.0f);

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));

	if (playback.isplaybackactive() || rec.isrecordingactive()) {
		if (rec.getactiverecording().size() > 2) {
			if (interfaces::engine->is_connected()) {
				if (g::local) {
					if (g::local->is_alive()) {
						auto& recording = rec.getactiverecording();
						for (int i = 1; i < recording.size(); i++) {
							vec3_t from; vec3_t to;
							auto accentclr = ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f);
							if (interfaces::debug_overlay->world_to_screen(recording.at(i).position, from) && interfaces::debug_overlay->world_to_screen(recording.at(i - 1).position, to) && c::misc::movement_rec_show_line) {
								//draw the recording line
								//if (start_time[i] == 0.0f) {
								//	if (i - 1 == 0 || line_progress[i - 1] == 1.0f) {
								//		start_time[i] = interfaces::globals->cur_time;
								//	}
								//}
								//float elapsed = (interfaces::globals->cur_time - start_time[i]) * 4;
								//if (i - 1 == 0 || line_progress[i - 1] == 1.0f) {
								//	line_progress[i] = std::clamp(elapsed / 1.0f, 0.0f, 1.0f);
								//}
								float elapsed = (interfaces::globals->cur_time - start_time) * 4;
								float progress = std::clamp(elapsed / 1.0f, 0.0f, 1.0f);
								int alpha = 255 * progress;
								float accent_red = LerpFloatReal(255.0f, accentclr.x * 255.0f, progress);
								float accent_blue = LerpFloatReal(255.0f, accentclr.y * 255.0f, progress);
								float accent_green = LerpFloatReal(255.0f, accentclr.z * 255.0f, progress);
								im_render.drawline(from.x, from.y, to.x, to.y, color_t(static_cast <int>(accent_red), static_cast <int>(accent_blue), static_cast <int>(accent_green), alpha), 0.5f);
								//circle @ start
								if (i == 1) {
									im_render.drawcirclefilled_3d(recording.at(i - 1).position, 21.f, color_t(0, 0, 0, 100));
									im_render.drawcircle_3d(recording.at(i - 1).position, 21.f, color_t(static_cast <int>(accent_red), static_cast <int>(accent_blue), static_cast <int>(accent_green), 255));
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		if (!mapWithRoutes.empty()) {
			if (g::local->is_alive()) {
				for (size_t i = 0; i < mapWithRoutes.size(); i++) {
					if (GImGui == nullptr)
						return;

					vec3_t scrpos;
					if (interfaces::debug_overlay->world_to_screen(mapWithRoutes[i].frames[0].position, scrpos))
					{
						auto accentclr = ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f);
						float distance = g::local->origin().distance_to(mapWithRoutes[i].frames[0].position);
						float baseRadius = 11.0f;
						float delta_time = ImGui::GetIO().DeltaTime;

						if (distance > 300.0f) {
							float t = std::clamp((distance - 300.0f) / 300.0f, 0.0f, 1.0f);
							baseRadius = LerpFloatReal(11.0f, 8.0f, t);
						}

						float targetScale = (distance < 15.f) ? 2.f : 1.f;
						point_progress[i] = LerpFloatReal(point_progress[i], targetScale, delta_time * 5.f);

						baseRadius = std::max(baseRadius, 3.0f);

						float alphaMultiplier = 1.0f;
						if (distance > 900.0f) {
							alphaMultiplier = std::clamp((1000.0f - distance) / 100.0f, 0.0f, 1.0f);
						}

						float effectiveRadius = baseRadius * point_progress[i];
						const color_t filled(0, 0, 0, static_cast<int>(100.0f * alphaMultiplier));
						const color_t outline(255, 255, 255, static_cast<int>(255.0f * alphaMultiplier));

						static float time = 0.0f;
						time += ImGui::GetIO().DeltaTime;
						int alpha = 200 - fmod(time * 20.84f, 200.0f); //25 : 1,2
						float radius = fmod(time * 4.17f, 40.0f); //5 : 1,2

						vec3_t pos2 = mapWithRoutes[i].frames[0].position;
						pos2.z += 2.f;

						if (distance < 15.f) {
							im_render.drawcircle_3d(pos2, radius, color_t(255, 255, 255, alpha), 1.0f);
						}

						im_render.drawcirclefilled_3d(mapWithRoutes[i].frames[0].position, effectiveRadius, filled);
						im_render.drawcircle_3d(mapWithRoutes[i].frames[0].position, effectiveRadius, outline);

						im_render.text(scrpos.x, scrpos.y - 20, 12.f, fonts::esp_misc,
							mapWithRoutes[i].routename.c_str(), true, outline);
					}

					if (g::local->origin().distance_to(mapWithRoutes[i].frames[0].position) < 15.f)
					{
						vec3_t o = g::local->get_eye_pos();
						vec3_t angles = { mapWithRoutes[i].frames[0].viewangles[0],
										  mapWithRoutes[i].frames[0].viewangles[1], 0.f };
						vec3_t forward;
						math::angle_vectors(angles, &forward);
						vec3_t out;
						if (interfaces::debug_overlay->world_to_screen(o + (forward * 100.f), out)) {
							auto text_size = fonts::esp_misc->CalcTextSizeA(12.f, FLT_MAX, 0.0f, "aim here");
							im_render.text(out.x + 7, out.y - (text_size.y / 2), 12.f, fonts::esp_misc,
								"aim here", false, color_t(255, 255, 255, 255));
							im_render.drawcirclefilled(out.x, out.y, 4, 32, color_t(0, 0, 0, 100));
							im_render.drawcircle(out.x, out.y, 4, 32, color_t(255, 255, 255, 255));
						}
					}
				}
			}
		}
	}
}

void mrecorder::endscene(ImDrawList* draw) {
	if (!c::misc::movement_rec || !c::misc::movement_rec_render)
		return;
	if (mapWithRoutes.empty())
		return;
	if (!g::local->is_alive())
		return;
	if (GImGui == nullptr)
		return;
	float deltaTime = ImGui::GetIO().DeltaTime;
	if (!playback.isplaybackactive() && !rec.isrecordingactive()) {
		anim.update(false, deltaTime);
	}
	else {
		anim.update(true, deltaTime);
	}
	if (anim.alpha <= 0.01f) {
		return;
	}
	//getting text
	framecontainer& recording = rec.getactiverecording();
	int currentFrame = playback.getcurrentframe();
	int maxFrames = recording.size();
	std::string text = std::to_string(currentFrame) + (" / ") + std::to_string(maxFrames);

	//kinda lazy to make custom font for that
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 infoTextSize = fonts::recorder_font->CalcTextSizeA(12.f, FLT_MAX, NULL, text.c_str());
	static const ImVec2& padding = ImVec2(7, 7);
	static const ImVec2& margin = ImVec2(4, 3);
	ImVec2 size = ImVec2(infoTextSize.x + margin.x * 2.f, infoTextSize.y + margin.y * 2.f);

	//positions
	ImVec2 pos;
	switch (c::misc::movement_rec_position) {
	case 0:
		pos = ImVec2(padding.x, padding.y);
		break;
	case 1:
		pos = ImVec2(padding.x, displaySize.y - size.y - padding.y);
		break;
	case 2:
		pos = ImVec2(displaySize.x - size.x - padding.x, displaySize.y - size.y - padding.y);
		break;
	}
	ImVec2 textPos = ImVec2(pos.x + margin.x, pos.y + margin.y);

	//colors
	ImColor bg_color(0.08f, 0.08f, 0.08f, anim.alpha);
	ImColor outline_start_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], anim.alpha);
	ImColor outline_end_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 0.f);

	//drawing
	draw->AddRectFilledMultiColor(pos + ImVec2(-1, -1), pos + ImVec2(0, size.y + 1), outline_start_color, outline_start_color, outline_end_color, outline_end_color);
	draw->AddRectFilled(pos + ImVec2(0, -1), pos + ImVec2(size.x, 0), outline_start_color);
	draw->AddRectFilledMultiColor(pos + ImVec2(size.x, -1), pos + ImVec2(size.x + 1, size.y + 1), outline_start_color, outline_start_color, outline_end_color, outline_end_color);
	draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);
	draw->AddText(fonts::recorder_font, 12.f, ImVec2(textPos.x, textPos.y), ImColor(255.f, 255.f, 255.f, anim.alpha), text.c_str());
}

bool goingtostart = false;
bool smoothingtostart = false;
bool wishtostart = false;
vec3_t aimto;

void mrecorder::create_move(c_usercmd* cmd) {
	if (!c::misc::movement_rec)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	bool isplaybackactive = playback.isplaybackactive();
	bool isRecordingActive = rec.isrecordingactive();

	if (g::local->is_alive()) {

		if (menu::checkkey(c::misc::movement_rec_keystartrecord, c::misc::movement_rec_keystartrecord_s) && !rec.isrecordingactive() && !playback.isplaybackactive() && !menu::open)
			rec.startrecording();

		if (menu::checkkey(c::misc::movement_rec_keystoprecord, c::misc::movement_rec_keystoprecord_s) && !playback.isplaybackactive()) {
			rec.stoprecording();
			cmd->view_angles.normalize3();
		}

		if (menu::checkkey(c::misc::movement_rec_keystartplayback, c::misc::movement_rec_keystartplayback_s) && !rec.isrecordingactive() && !playback.isplaybackactive() && !menu::open) {
			if (mapWithRoutes.size() > 0) {

				size_t closestroute = -1;
				float bestdist = 999999;

				for (size_t i = 0; i < mapWithRoutes.size(); i++) {
					framecontainer& currec = mapWithRoutes.at(i).frames;
					if (currec.at(0).position.distance_to(prediction_backup::origin) < bestdist) {
						closestroute = i;
						bestdist = currec.at(0).position.distance_to(prediction_backup::origin);
					}
				}

				if (closestroute == -1)
					return;

				if (mapWithRoutes.at(closestroute).frames.size() > 0) {
					if (mapWithRoutes.at(closestroute).frames.at(0).position.distance_to(prediction_backup::origin) < 15.f) {
						rec.setrecording(mapWithRoutes.at(closestroute).frames);
						goingtostart = true;
						if (c::misc::movement_rec_lockva)
							smoothingtostart = true;
						wishtostart = true;
					}
				}
			}
		}

		if (goingtostart) {

			float dist = prediction_backup::origin.distance_to(rec.getactiverecording().at(0).position);

			if (dist <= 0.1f) {
				goingtostart = false;
			}

			auto curPos = prediction_backup::origin;
			auto difference = curPos - rec.getactiverecording().at(0).position;

			float cos_flt, sin_flt;
			cos_flt = cos(cmd->view_angles.y / 180.f * m_pi);
			sin_flt = sin(cmd->view_angles.y / 180.f * m_pi);
			auto velocity = vec3_t {
				difference.x * cos_flt + difference.y * sin_flt,
				difference.y * cos_flt - difference.x * sin_flt,
				difference.z
			};

			cmd->forward_move = -velocity.x * 20.f;
			cmd->side_move = velocity.y * 20.f;
		}

		if (smoothingtostart) {
			cmd->mouse_dx = 0;
			cmd->mouse_dy = 0;

			vec2_t targetviewangles = { rec.getactiverecording().at(0).viewangles[0], rec.getactiverecording().at(0).viewangles[1] };

			vec3_t deltaang = vec3_t{ targetviewangles.x - cmd->view_angles.x, targetviewangles.y - cmd->view_angles.y, 0.f };

			if (deltaang.length_r() < 0.1f || !c::misc::movement_rec_lockva) {
				smoothingtostart = false;
			}

			if (menu::checkkey(c::misc::movement_rec_keystopplayback, c::misc::movement_rec_keystopplayback_s) && !rec.isrecordingactive()) {
				goingtostart = false;
				smoothingtostart = false;
				return;
			}

			deltaang.normalize3();

			deltaang.x /= c::misc::movement_rec_smoothing;
			deltaang.y /= c::misc::movement_rec_smoothing;
			cmd->view_angles.x += deltaang.x;
			cmd->view_angles.y += deltaang.y;
			aimto = cmd->view_angles;
			interfaces::engine->set_view_angles(aimto);
		}

		if (wishtostart) {
			if (!smoothingtostart && !goingtostart) {
				if (!rec.getactiverecording().empty()) {
					if (prediction_backup::origin.distance_to(rec.getactiverecording().at(0).position) < 0.1f) {
						if ((vec3_t{ vec3_t{ rec.getactiverecording().at(0).viewangles[0], rec.getactiverecording().at(0).viewangles[1], 0.f } - cmd->view_angles }.length_2d() < 0.1f) || !c::misc::movement_rec_lockva) {
							playback.startplayback(rec.getactiverecording());
							playback.set_start_tick(interfaces::globals->tick_count);
							start_time = interfaces::globals->cur_time;
							wishtostart = false;
							return;
						}
					}
				}
			}
		}

		if (menu::checkkey(c::misc::movement_rec_keystopplayback, c::misc::movement_rec_keystopplayback_s) && playback.isplaybackactive() && !rec.isrecordingactive()) {
			goingtostart = false;
			smoothingtostart = false;
			playback.stopplayback();
		}

		//.............. idk how to fix it im really sorry for thatt
		if (isplaybackactive) {
			const size_t current_playback_frame = playback.getcurrentframe();
			try {
				if (current_playback_frame + 1 == rec.getactiverecording().size()) {
					playback.stopplayback();
				}
			}
			catch (std::out_of_range) {
				playback.stopplayback();
			}
		}

		if (isRecordingActive) {
			prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			rec.getactiverecording().push_back(frame(cmd, g::local->origin()));
		}

		//TODO: UNDERSTAND WHY THE IT STILL LAGS FOR THE FIRST TRY

		if (isplaybackactive) {
			const size_t current_playback_frame = playback.getcurrentframe();
			try {
				vec3_t orang = cmd->view_angles;
				rec.getactiverecording().at(current_playback_frame).replay(cmd);
				vec3_t tomvfix = cmd->view_angles;
				if (!c::misc::movement_rec_lockva) {
					cmd->view_angles = orang;
					features::movement::fix_movement(cmd, tomvfix);
				}
				playback.setcurrentframe(current_playback_frame + 1);
			}
			catch (std::out_of_range) {
				playback.stopplayback();
			}
		}
	}

	if (menu::checkkey(c::misc::movement_rec_keyclearrecord, c::misc::movement_rec_keyclearrecord_s) && !menu::open) {
		if (!rec.isrecordingactive() && !playback.isplaybackactive()) {
			rec.getactiverecording().clear();
		}
	}

	//???
	cmd->view_angles.normalize3();
}

void mrecorder::frame_stage(int stage)
{
	if (stage == frame_start) {
		if (!playback.isplaybackactive() || !c::misc::movement_rec_lockva)
			return;

		//getting current recorder frame
		size_t current_idx = playback.getcurrentframe();
		auto& recording = rec.getactiverecording();

		//if ended
		if (current_idx >= recording.size() || current_idx + 1 >= recording.size()) {
			return;
		}

		//calculating shit
		auto& frame_current = recording[current_idx];
		auto& frame_next = recording[current_idx + 1];

		int current_tick = playback.get_start_tick() + current_idx;
		int next_tick = current_tick + 1;

		float current_frame_time = ticks_to_time(current_tick);
		float next_frame_time = ticks_to_time(next_tick);

		float total_time = next_frame_time - current_frame_time;
		float elapsed_time = interfaces::globals->cur_time - current_frame_time;	
		float progress = elapsed_time / total_time;

		if (progress > 1.0f) progress = 1.0f;
		if (progress < 0.0f) progress = 0.0f;

		float pitch_interp = frame_current.viewangles[0] +
			(frame_next.viewangles[0] - frame_current.viewangles[0]) * progress;

		float yaw_delta = math::normalize_yaw(frame_next.viewangles[1] - frame_current.viewangles[1]);
		float yaw_interp = frame_current.viewangles[1] + (yaw_delta * progress);

		//setting viewangles (instead of doing that in createmove)
		vec3_t interpolated = { pitch_interp, yaw_interp, 0.0f };
		interfaces::engine->set_view_angles(interpolated);
	}
}

void mrecorder::camera_lock(float& x, float& y)
{
	if (playback.isplaybackactive() && c::misc::movement_rec_lockva) {
		x = 0;
		y = 0;

	}
	else if (c::misc::movement_rec_lockgoingtostart && (smoothingtostart || goingtostart)) {
		x = 0;
		y = 0;
	}
	else {
		return;
	}
}

savingroute::savingroute(const char* name) {
	path /= "C:/delusional";
	path /= name;
	std::error_code ec;
	if (!std::filesystem::exists(path))
		std::filesystem::create_directory(path, ec);
	listroutes();
}

bool is_empty_file(std::ifstream& file) {
	return file.peek() == std::ifstream::traits_type::eof();
}

void savingroute::load(size_t id) {

	//playback.stopplayback();
	mapWithRoutes.clear();

	Json::Value json;

	std::ifstream in(path.string() + "\\" + routes[id].c_str());

	if (in && in.good()) {
		if (!is_empty_file(in)) {
			in >> json;
		}
		else {
			return;
		}
	}
	else {
		return;
	}

	//std::vector<FullRoute> topush;
	auto& mainjson = json[("main")];

	size_t routeamount = 0;
	if (mainjson.isMember(("records"))) routeamount = mainjson[("records")].asInt();

	for (size_t i = 0; i < routeamount; i++) {
		fullroute thisroute = {};
		//memset(&thisroute, 0, sizeof(thisroute));

		auto& globaljson = json[("globals")][i];
		if (globaljson.isMember(("routename"))) thisroute.routename = globaljson[("routename")].asCString();
		if (globaljson.isMember(("startwpn"))) thisroute.startwpn = globaljson[("startwpn")].asCString();
		size_t framecount = 0;
		if (globaljson.isMember(("framecount"))) framecount = globaljson[("framecount")].asInt();

		for (size_t j = 0; j < framecount; j++) {
			auto& framejson = json[("frame")][i][j];

			c_usercmd curframe = c_usercmd();
			vec3_t pos = {};

			if (framejson.isMember(("viewanglex"))) curframe.view_angles.x = framejson[("viewanglex")].asFloat();
			if (framejson.isMember(("viewangley"))) curframe.view_angles.y = framejson[("viewangley")].asFloat();
			if (framejson.isMember(("forwardmove"))) curframe.forward_move = framejson[("forwardmove")].asFloat();
			if (framejson.isMember(("sidemove"))) curframe.side_move = framejson[("sidemove")].asFloat();
			if (framejson.isMember(("upmove"))) curframe.up_move = framejson[("upmove")].asFloat();
			if (framejson.isMember(("buttons"))) curframe.buttons = framejson[("buttons")].asInt();
			if (framejson.isMember(("mousedx"))) curframe.mouse_dx = framejson[("mousedx")].asInt();
			if (framejson.isMember(("mousedy"))) curframe.mouse_dy = framejson[("mousedy")].asInt();
			if (framejson.isMember(("posx"))) pos.x = framejson[("posx")].asFloat();
			if (framejson.isMember(("posy"))) pos.y = framejson[("posy")].asFloat();
			if (framejson.isMember(("posz"))) pos.z = framejson[("posz")].asFloat();

			thisroute.frames.push_back(frame(&curframe, pos));
		}
		mapWithRoutes.push_back(thisroute);
	}
	curroute = id;
}

void savingroute::save(size_t id, std::string name) const {

	if (rec.getactiverecording().size() < 1)
		return;

	fullroute routetosave;
	routetosave.frames = rec.getactiverecording();
	routetosave.routename = name.c_str();

	mapWithRoutes.push_back(routetosave);

	Json::Value json;
	auto& mainjson = json[("main")];
	mainjson[("records")] = mapWithRoutes.size();

	for (size_t i = 0; i < mapWithRoutes.size(); i++) {
		fullroute thisroute = mapWithRoutes[i];

		auto& globaljson = json[("globals")][i];
		globaljson[("routename")] = thisroute.routename;
		globaljson[("startwpn")] = thisroute.startwpn;
		globaljson[("framecount")] = thisroute.frames.size();
		for (size_t j = 0; j < thisroute.frames.size(); j++) {
			auto& framejson = json[("frame")][i][j];

			framejson[("viewanglex")] = thisroute.frames.at(j).viewangles[0];
			framejson[("viewangley")] = thisroute.frames.at(j).viewangles[1];
			framejson[("forwardmove")] = thisroute.frames.at(j).forwardmove;
			framejson[("sidemove")] = thisroute.frames.at(j).sidemove;
			framejson[("upmove")] = thisroute.frames.at(j).upmove;
			framejson[("buttons")] = thisroute.frames.at(j).buttons;
			framejson[("mousedx")] = thisroute.frames.at(j).mousedx;
			framejson[("mousedy")] = thisroute.frames.at(j).mousedy;
			framejson[("posx")] = thisroute.frames.at(j).position.x;
			framejson[("posy")] = thisroute.frames.at(j).position.y;
			framejson[("posz")] = thisroute.frames.at(j).position.z;
		}
	}

	std::ofstream out(path.string() + "\\" + routes[id].c_str());

	if (out && out.good()) {
		out << json;
		curroute = id;
	}
}

void savingroute::add(const char* name) {
	if (*name && std::find(routes.cbegin(), routes.cend(), name) == routes.cend()) {
		routes.emplace_back(name);
		if (routes.size() == 0) {
			if (std::ofstream out{ path.string() + "\\" + routes[0].c_str() }; out.good())
				out << "";
		}
		else {
			if (std::ofstream out{ path.string() + "\\" + routes[routes.size() - 1].c_str() }; out.good())
				out << "";
		}
	}
}

void savingroute::remove(size_t id) {
	std::error_code ec;
	std::filesystem::remove(path.string() + "\\" + routes[id].c_str(), ec);
	routes.erase(routes.cbegin() + id);
}

void savingroute::rename(size_t item, const char* newName) {
	std::error_code ec;
	std::filesystem::rename(path.string() + "\\" + routes[item].c_str(), path.string() + "\\" + newName, ec);
	routes[item] = newName;
}


void savingroute::listroutes() {
	routes.clear();

	std::error_code ec;
	std::transform(std::filesystem::directory_iterator{ path, ec },
		std::filesystem::directory_iterator{ },
		std::back_inserter(routes),
		[](const auto& entry) { return std::string{ (const char*)entry.path().filename().u8string().c_str() }; });
}