#include "movement.hpp"
#include <vector>
#include <memory>
#include "prediction/prediction.hpp"
#include <imgui/imgui_internal.h>

#undef max;
#undef min;


bool alert_mini_crouch_hop = false;
bool bounce_alert_mini_crouch_hop = false;

int gloabal_settings_timer = 0;


struct NewCmd3 {
	vec3_t viewangle;
	float forwardmove;
	float sideMove;
	int buttons;
	vec3_t origin;
};
NewCmd3 NewCmd_assist[128];

struct assist_t {
	bool in_crosshair = false;
	bool set_point = false;


} m_assist_t;

struct AnimatedPoint {
	vec3_t position;
	float animation_progress = 0.f;
	bool is_appearing = true;
	bool is_removing = false;
	float current_size = 0.f;
};
std::vector< AnimatedPoint > animated_pixelsurf_points;

class WindowDetect
{
public:
	enum WindowState {
		FADE_IN,
		VISIBLE,
		FADE_OUT
	};

	struct AnimatedWindow {
		int type;
		std::string info;
		float elapsed;
		WindowState state;
		float fadeInDuration;
		float visibleDuration;
		float fadeOutDuration;
		float totalDuration;
		float alpha;

		ImVec2 targetPosition;
		ImVec2 currentPosition;

		float accentProgress;
		float accentWidth;
		bool startFadeOut;
	};

	static constexpr float NORMAL_FADEIN_DURATION = 0.25f;
	static constexpr float QUICK_FADEIN_DURATION = 0.15f;
	static constexpr float VISIBLE_DURATION = 2.5f;
	static constexpr float NORMAL_FADEOUT_DURATION = 0.2f;
	static constexpr float QUICK_FADEOUT_DURATION = 0.12f;

	static constexpr float WINDOW_SPACING = 5.0f / 1080.0f;
	void AddWindow(int type, const std::string& info)
	{
		const float baseYPosition = 0.8f;

		constexpr int MAX_VISIBLE_WINDOWS = 5;


		ImVec2 infoTextSize = fonts::assist_font->CalcTextSizeA(12.f, FLT_MAX, NULL, info.c_str());		// ?????????????????? //ужас
		float heightPadding = 22.0f;
		float windowHeight = (infoTextSize.y + heightPadding) / 1080.0f;

		AnimatedWindow newWin;
		newWin.type = type;
		newWin.info = info;
		newWin.elapsed = 0.f;
		newWin.state = FADE_IN;
		newWin.fadeInDuration = NORMAL_FADEIN_DURATION;
		newWin.visibleDuration = VISIBLE_DURATION;
		newWin.fadeOutDuration = NORMAL_FADEOUT_DURATION;
		newWin.totalDuration = NORMAL_FADEIN_DURATION + VISIBLE_DURATION + NORMAL_FADEOUT_DURATION;
		newWin.alpha = 0.f;
		newWin.accentProgress = 0.f;
		newWin.accentWidth = 0.f;
		newWin.startFadeOut = false;

		int visibleCount = 0;
		for (auto& win : windows) {
			if (win.state != FADE_OUT) {
				visibleCount++;
			}
		}

		for (auto& win : windows) {
			if (win.state != FADE_OUT) {
				win.targetPosition.y -= (windowHeight + WINDOW_SPACING);
				win.currentPosition.y -= (windowHeight + WINDOW_SPACING);
				if (visibleCount >= MAX_VISIBLE_WINDOWS &&
					win.targetPosition.y < baseYPosition - (MAX_VISIBLE_WINDOWS - 1) * (windowHeight + WINDOW_SPACING)) {
					win.state = FADE_OUT;
					win.elapsed = 0.f;
					win.fadeOutDuration = NORMAL_FADEOUT_DURATION;
				}
			}
		}
		newWin.targetPosition = ImVec2(0.5f, baseYPosition);
		newWin.currentPosition = ImVec2(0.5f, baseYPosition);
		windows.push_back(newWin);
	}
	float Lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}
	float EaseInOutCubic(float t)
	{
		return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
	}
	void Update(float deltaTime)
	{
		CleanupOldWindows();
		std::sort(windows.begin(), windows.end(),
			[](const AnimatedWindow& a, const AnimatedWindow& b) { return a.targetPosition.y > b.targetPosition.y; });

		for (auto it = windows.begin(); it != windows.end(); ) {
			it->elapsed += deltaTime;

			float totalElapsed = 0.0f;
			if (it->state == FADE_IN) {
				totalElapsed = it->elapsed;
			}
			else if (it->state == VISIBLE) {
				totalElapsed = it->fadeInDuration + it->elapsed;
			}
			else if (it->state == FADE_OUT) {
				totalElapsed = it->fadeInDuration + it->visibleDuration + it->elapsed;
			}
			it->accentProgress = totalElapsed / it->totalDuration;
			if (it->accentProgress > 1.0f)
				it->accentProgress = 1.0f;

			switch (it->state) {
			case FADE_IN:

				it->alpha = it->elapsed / it->fadeInDuration;
				if (it->alpha > 1.0f)
					it->alpha = 1.0f;

				if (it->elapsed >= it->fadeInDuration) {
					it->state = VISIBLE;
					it->elapsed = 0.f;
					it->alpha = 1.f;
				}
				break;
			case VISIBLE:
				it->alpha = 1.f;

				if (it->elapsed >= it->visibleDuration && !it->startFadeOut) {
					it->startFadeOut = true;
					it->state = FADE_OUT;
					it->elapsed = 0.f;
				}
				break;
			case FADE_OUT:

				it->alpha = 1.0f - (it->elapsed / it->fadeOutDuration);
				if (it->alpha < 0.0f)
					it->alpha = 0.0f;

				if (it->elapsed >= it->fadeOutDuration) {

					it = windows.erase(it);
					continue;
				}
				break;
			}
			++it;
		}
	}


	void Draw(ImDrawList* draw, const ImVec2& displaySize)
	{
		for (const auto& win : windows) {
			// Skip fully faded-out windows to prevent flickering
			if (win.state == FADE_OUT && win.alpha <= 0.0f) {
				continue;
			}
			if (!c::assist::assist_render_style) {
				ImVec2 infoTextSize = fonts::assist_font->CalcTextSizeA(12.f, FLT_MAX, NULL, win.info.c_str());
				float widthPadding = 16.0f;
				float heightPadding = 10.0f;
				
				//pos
				ImVec2 size = ImVec2(infoTextSize.x + widthPadding, infoTextSize.y + heightPadding);
				ImVec2 pos = ImVec2(displaySize.x * win.currentPosition.x - size.x * 0.5f, displaySize.y * win.currentPosition.y - size.y * 0.5f);
				ImVec2 textPos = ImVec2(pos.x + (size.x - infoTextSize.x) * 0.5f, pos.y + (size.y - infoTextSize.y) * 0.5f);

				//colors
				int a = static_cast<int>(win.alpha * 255);
				ImColor bg_color(0.08f, 0.08f, 0.08f, win.alpha);
				ImColor outline_start_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], win.alpha);
				ImColor outline_end_color(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 0.f);
				ImU32 textColor = IM_COL32(255, 255, 255, a);

				draw->AddRectFilledMultiColor(pos + ImVec2(-1, -1), pos + ImVec2(0, size.y + 1), outline_start_color, outline_start_color, outline_end_color, outline_end_color);
				draw->AddRectFilled(pos + ImVec2(0, -1), pos + ImVec2(size.x, 0), outline_start_color);
				draw->AddRectFilledMultiColor(pos + ImVec2(size.x, -1), pos + ImVec2(size.x + 1, size.y + 1), outline_start_color, outline_start_color, outline_end_color, outline_end_color);
				draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bg_color);

				draw->AddText(fonts::assist_font, 12.f, ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, a / 4), win.info.c_str());
				draw->AddText(fonts::assist_font, 12.f, textPos, textColor, win.info.c_str());
			}
			else {
				ImVec2 infoTextSize = fonts::assist_font->CalcTextSizeA(12.f, FLT_MAX, NULL, win.info.c_str());

				float widthPadding = 18.0f;
				float heightPadding = 18.0f;
				ImVec2 size = ImVec2(infoTextSize.x + widthPadding, infoTextSize.y + heightPadding);

				ImVec2 pos = ImVec2(displaySize.x * win.currentPosition.x - size.x * 0.5f, displaySize.y * win.currentPosition.y - size.y * 0.5f);

				int a = static_cast<int>(win.alpha * 255);
				ImU32 bgColor = IM_COL32(14, 14, 14, a);
				ImU32 textColor = IM_COL32(255, 255, 255, a);

				const float cornerRadius = 4.0f;

				draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor, cornerRadius);

				auto accentColor = ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f);		// ?????????????????????????? ImGuiCol_Accent //без мя даж акцент хакнуть не смог XD
				accentColor.w = win.alpha;

				const float accentHeight = 1.0f;

				float maxWidth = size.x - 4.0f;
				float margin = 2.0f;

				float currentWidth = maxWidth * (1.0f - EaseInOutCubic(win.accentProgress));
				if (currentWidth < 0)
					currentWidth = 0;

				float startX = pos.x + margin;

				if (currentWidth > 0) {
					float centerX = pos.x + size.x * 0.5f;
					float halfWidth = currentWidth * 0.5f;
					float lineY = pos.y + 0.0f;

					draw->AddRectFilled(ImVec2(centerX - halfWidth, lineY), ImVec2(centerX + halfWidth, lineY + accentHeight),
						ImGui::GetColorU32(accentColor));
				}

				ImVec2 textPos = ImVec2(pos.x + (size.x - infoTextSize.x) * 0.5f, pos.y + (size.y - infoTextSize.y) * 0.5f);

				draw->AddText(fonts::assist_font, 12.f, ImVec2(textPos.x + 1, textPos.y + 1), IM_COL32(0, 0, 0, a / 4), win.info.c_str());

				draw->AddText(fonts::assist_font, 12.f, textPos, textColor, win.info.c_str());
			}
		}
	}

private:
	std::vector< AnimatedWindow > windows; // Vector of animated windows

	// Method for programmatically removing old windows when there are too many
	void CleanupOldWindows()
	{
		constexpr int MAX_WINDOWS = 10; // Maximum total number of windows in collection

		// If there are more windows than the maximum
		if (windows.size() > MAX_WINDOWS) {
			// Find oldest windows (highest ones) and transition them to fade-out mode
			int windowsToRemove = windows.size() - MAX_WINDOWS;

			// Sort by Y position (topmost/oldest first)
			std::sort(windows.begin(), windows.end(),
				[](const AnimatedWindow& a, const AnimatedWindow& b) { return a.targetPosition.y < b.targetPosition.y; });

			// Transition oldest windows to fade-out mode
			for (int i = 0; i < windowsToRemove && i < windows.size(); i++) {
				if (windows[i].state != FADE_OUT) {
					windows[i].state = FADE_OUT;
					windows[i].elapsed = 0.f;
					windows[i].fadeOutDuration = QUICK_FADEOUT_DURATION; // Use quick fadeout for cleanup
				}
			}
		}
	}
};

WindowDetect myWindowDetect;

bool InCrosshair(float x, float y, float radius)
{
	int width, height;
	interfaces::engine->get_screen_size(width, height);

	float centerX = width / 2;
	float centerY = height / 2;
	float dx = centerX - x;
	float dy = centerY - y;

	return (dx * dx + dy * dy) <= (radius * radius);
}

void Pixelsurf_points(std::vector< vec3_t >& Points)
{
	float delta_time = ImGui::GetIO().DeltaTime;
	const float appearance_duration = 0.5f;
	const float disappearance_duration = 0.5f;
	const float size_animation_speed = 5.0f;
	const float normal_size = 9.f;
	const float in_crosshair_size = 12.f;

	for (auto& ap : animated_pixelsurf_points) {
		ap.is_removing = true;
	}

	for (const auto& p : Points) {
		auto it = std::find_if(animated_pixelsurf_points.begin(), animated_pixelsurf_points.end(), [&p](const AnimatedPoint& ap) { return ap.position == p; });
		if (it == animated_pixelsurf_points.end()) {
			animated_pixelsurf_points.push_back({ p, 0.f, true, false, 0.f });
		}
		else {
			it->is_removing = false;
		}
	}
	for (auto& ap : animated_pixelsurf_points) {
		if (ap.is_appearing) {
			ap.animation_progress += delta_time / appearance_duration;
			if (ap.animation_progress >= 1.f) {
				ap.animation_progress = 1.f;
				ap.is_appearing = false;
			}
		}
		else if (ap.is_removing) {
			ap.animation_progress -= delta_time / disappearance_duration;
			if (ap.animation_progress <= 0.f) {
				ap.animation_progress = 0.f;
			}
		}
		else {
			ap.animation_progress = 1.f;
		}
		ImVec2 screenPos;
		bool in_crosshair = false;
		if (im_render.world_to_screen(ap.position, &screenPos)) {
			in_crosshair = InCrosshair(screenPos.x, screenPos.y, 9.f);
		}
		float target_size;
		if (ap.is_removing) {
			target_size = 0.f;
		}
		else {
			target_size = in_crosshair ? in_crosshair_size : normal_size;
			if (ap.is_appearing) {
				target_size *= ap.animation_progress;
			}
		}
		ap.current_size += (target_size - ap.current_size) * (1.f - std::exp(-size_animation_speed * delta_time));
	}
	animated_pixelsurf_points.erase(std::remove_if(animated_pixelsurf_points.begin(), animated_pixelsurf_points.end(), [](const AnimatedPoint& ap) { return ap.is_removing && ap.animation_progress <= 0.f; }), animated_pixelsurf_points.end());

	static int stop = 0;
	bool point_selected = false;
	for (const auto& ap : animated_pixelsurf_points) {
		ImVec2 screenPos;
		if (im_render.world_to_screen(ap.position, &screenPos)) {
			bool in_crosshair = InCrosshair(screenPos.x, screenPos.y, 9.f);
			m_assist_t.in_crosshair = in_crosshair;


			float filled_alpha = ap.animation_progress * 100.f;
			float outline_alpha = ap.animation_progress * 255.f;

			im_render.drawcirclefilled(screenPos.x, screenPos.y, ap.current_size, 32, color_t(0, 0, 0, static_cast<int>(filled_alpha)));

			im_render.drawcircle(screenPos.x, screenPos.y, ap.current_size, 32, color_t(0, 255, 0, static_cast<int>(outline_alpha)));

			if (!point_selected && in_crosshair && menu::checkkey(c::assist::pixelsurf_point_key, c::assist::pixelsurf_point_key_s)) {
				if (menu::open)
					return;
				if (stop < GetTickCount64()) {
					features::movement::m_pixelsurf_points_check.emplace_back(ap.position, interfaces::engine->get_level_name());
					m_assist_t.set_point = true;
					Points.clear();
					point_selected = true;
					stop = GetTickCount64() + 3000;
				}
			}
		}
	}
}

vec3_t round_pos(const vec3_t& point)
{
	const float epsilon = 0.001f;
	const float z_offset = point.z < 0.0f ? -0.97f : 0.031f;
	return vec3_t(point.x, point.y, floorf(point.z + epsilon) + z_offset);
}
vec3_t round_pos2(vec3_t point)
{
	if (point.z < 0.f)
		return vec3_t(point.x, point.y, (int)point.z - 0.969644f);
	else
		return vec3_t(point.x, point.y, (int)point.z + 0.04f);
}

bool bounce_set_point = false;
std::vector< AnimatedPoint > animated_bounce_points;
void Bounce_points(std::vector< vec3_t >& Points)
{
	ImGuiIO& io = ImGui::GetIO();
	float delta_time = ImGui::GetIO().DeltaTime;
	const float appearance_duration = 0.5f;
	const float disappearance_duration = 0.5f;
	const float size_animation_speed = 5.0f;
	const float normal_size = 9.f;
	const float in_crosshair_size = 12.f;

	for (auto& ap : animated_bounce_points) {
		ap.is_removing = true;
	}

	for (const auto& p : Points) {
		auto it = std::find_if(animated_bounce_points.begin(), animated_bounce_points.end(), [&p](const AnimatedPoint& ap) { return ap.position == p; });
		if (it == animated_bounce_points.end()) {
			animated_bounce_points.push_back({ p, 0.f, true, false, 0.f });
		}
		else {
			it->is_removing = false;
		}
	}
	for (auto& ap : animated_bounce_points) {
		if (ap.is_appearing) {
			ap.animation_progress += delta_time / appearance_duration;
			if (ap.animation_progress >= 1.f) {
				ap.animation_progress = 1.f;
				ap.is_appearing = false;
			}
		}
		else if (ap.is_removing) {
			ap.animation_progress -= delta_time / disappearance_duration;
			if (ap.animation_progress <= 0.f) {
				ap.animation_progress = 0.f;
			}
		}
		else {
			ap.animation_progress = 1.f;
		}
		ImVec2 screenPos;
		bool in_crosshair = false;
		if (im_render.world_to_screen(ap.position, &screenPos)) {
			in_crosshair = InCrosshair(screenPos.x, screenPos.y, 9.f);
		}

		float target_size;
		if (ap.is_removing) {
			target_size = 0.f;
		}
		else {
			target_size = in_crosshair ? in_crosshair_size : normal_size;
			if (ap.is_appearing) {
				target_size *= ap.animation_progress;
			}
		}
		ap.current_size += (target_size - ap.current_size) * (1.f - std::exp(-size_animation_speed * delta_time));
	}

	animated_bounce_points.erase(std::remove_if(animated_bounce_points.begin(), animated_bounce_points.end(),
		[](const AnimatedPoint& ap) { return ap.is_removing && ap.animation_progress <= 0.f; }),
		animated_bounce_points.end());
	static int stop = 0;
	bool point_selected = false;
	for (const auto& ap : animated_bounce_points) {
		ImVec2 screenPos;
		if (im_render.world_to_screen(ap.position, &screenPos)) {
			bool in_crosshair = InCrosshair(screenPos.x, screenPos.y, 9.f);
			m_assist_t.in_crosshair = in_crosshair;

			float filled_alpha = ap.animation_progress * 100.f;
			float outline_alpha = ap.animation_progress * 255.f;

			im_render.drawcirclefilled(screenPos.x, screenPos.y, ap.current_size, 32, color_t(0, 0, 0, static_cast<int>(filled_alpha)));

			im_render.drawcircle(screenPos.x, screenPos.y, ap.current_size, 32, color_t(255, 0, 0, static_cast<int>(outline_alpha)));
			if (!point_selected && in_crosshair && menu::checkkey(c::assist::bounce_point_key, c::assist::bounce_point_key_s)) {
				if (menu::open)
					return;
				if (stop < GetTickCount64()) {
					features::movement::m_bounce_points_check.emplace_back(round_pos2(vec3_t(ap.position.x, ap.position.y, ap.position.z + 1.f)), interfaces::engine->get_level_name());
					bounce_set_point = true;
					Points.clear();
					point_selected = true;
					stop = GetTickCount64() + 3000;
				}
			}
		}
	}
}

void features::movement::check_ps(c_usercmd* cmd)
{
	static vec3_t StartPos, EndPos{ };
	static vec3_t ViewAngles{ };
	static vec3_t pixelsurfPoint{ };
	static vec3_t WallNormal{ };
	static bool redLine = false;
	static std::vector< vec3_t > Points{ };
	static std::vector< vec3_t > Point_bounce{ };
	int to_muchPoints = 0;
	static bool is_displacement = false;

	if (!g::local)
		return;

	if (cmd != nullptr)
	{
		float sv_gravity = interfaces::console->get_convar(("sv_gravity"))->get_float();
		float fTickInterval = interfaces::globals->interval_per_tick;
		float fTickRate = (fTickInterval > 0) ? (1.0f / fTickInterval) : 0.0f;
		float targetZvelo = ((sv_gravity / 2) / fTickRate) * -1.f;
		ViewAngles = vec3_t(cmd->view_angles.x, cmd->view_angles.y, 0.f);
		static bool FirstPoint = FALSE;
		if (menu::checkkey(c::assist::pixelsurf_line_key, c::assist::pixelsurf_line_key_s))
		{
			if (!FirstPoint) {
				if (Points.size() > 0)
					Points.clear();
				if (Point_bounce.size() > 0)
					Point_bounce.clear();
				const auto endPos = g::local->get_eye_pos() + vec3_t::fromAngle(ViewAngles) * 6000;

				trace_t trace = { };
				ray_t ray;
				ray.initialize(g::local->get_eye_pos(), endPos);

				trace_filter flt(g::local);

				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);

				WallNormal = trace.plane.normal;
				is_displacement = strstr(trace.surface.name, "displacement");

				StartPos = g::local->get_eye_pos() + (endPos - g::local->get_eye_pos()) * trace.flFraction;
				EndPos = g::local->get_eye_pos() + (endPos - g::local->get_eye_pos()) * trace.flFraction;
				FirstPoint = true;
			}
			else
			{
				const auto endPos = g::local->get_eye_pos() + vec3_t::fromAngle(ViewAngles) * 6000;
				trace_t trace = { };
				ray_t ray;
				ray.initialize(g::local->get_eye_pos(), endPos);

				trace_filter flt(g::local);

				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);

				vec3_t SecPlane = trace.plane.normal;
				EndPos = vec3_t(StartPos.x, StartPos.y, vec3_t(g::local->get_eye_pos() + (endPos - g::local->get_eye_pos()) * trace.flFraction).z);
			}
		}
		else if (!menu::checkkey(c::assist::pixelsurf_line_key, c::assist::pixelsurf_line_key_s) && FirstPoint)
		{
			if (redLine) {
				redLine = false;
				FirstPoint = false;

				return;
			}
			vec3_t origin = g::local->origin();
			int buttons = cmd->buttons;
			float ForwardMove = cmd->forward_move;
			float Lerp = 0;

			int PredictionFrames = interfaces::prediction->split->commands_predicted - 1;
			float test_al = 15.97803f;
			if (is_displacement)
				test_al = 16.001f;
			auto colid = g::local->collideable();
			for (int v = 0; v < 4; v++) {
				prediction::restore_ent_to_predicted_frame(PredictionFrames);
				bool done_and_break = false;
				for (int i = 0; i < 10000; i++)
				{
					float align = 15.97803f;
					cmd->buttons |= in_jump;
					cmd->buttons |= in_duck;

					vec3_t Angles{ WallNormal.x * -1.f, WallNormal.y * -1.f, 0.f };
					vec3_t to_wall = Angles.to_angle();
					float rotation = deg2rad(to_wall.y - cmd->view_angles.y);
					float cos_rot = cos(rotation);
					float sin_rot = sin(rotation);
					float forwardmove = cos_rot * 10.f;
					float sidemove = -sin_rot * 10.f;
					cmd->forward_move = forwardmove;
					cmd->side_move = sidemove;

					if (g::local->get_velocity().z > 0)
					{
						static float Height = 0.f;
						if (StartPos.z > EndPos.z) {
							if (StartPos.z < 0.f)
								Height = (int)StartPos.z - 0.02f - Lerp - 54.f;
							else
								Height = (int)StartPos.z + 0.02f + Lerp - 54.f;
						}
						else {
							if (EndPos.z < 0.f)
								Height = (int)EndPos.z - 0.02f - Lerp - 54.f;
							else
								Height = (int)EndPos.z + 0.02f + Lerp - 54.f;
						}

						int maxLerp;
						if (StartPos.z > EndPos.z) {
							maxLerp = StartPos.z - EndPos.z;
						}
						else {
							maxLerp = EndPos.z - StartPos.z;
						}

						if (Lerp > maxLerp) {
							cmd->buttons = buttons;
							cmd->forward_move = ForwardMove;
							FirstPoint = false;
							done_and_break = true;
							break;
						}
						else {
							Lerp += 1.f;
						}
						if (WallNormal.x < 0 && WallNormal.y < 0.f) {
							g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y - test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x < 0 && WallNormal.y > 0.f) {
							g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y + test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x > 0 && WallNormal.y < 0.f) {
							g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y - test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x > 0 && WallNormal.y > 0.f) {
							g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y + test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x == 0.f && WallNormal.y > 0.f) {
							g::local->origin() = vec3_t(StartPos.x, StartPos.y + test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x == 0.f && WallNormal.y < 0.f) {
							g::local->origin() = vec3_t(StartPos.x, StartPos.y - test_al, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x < 0 && WallNormal.y == 0.f) {
							g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						else if (WallNormal.x > 0 && WallNormal.y == 0.f) {
							g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y, Height);
							g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
						}
						trace_t trace;
						trace_filter flt(g::local);
						ray_t ray;
						ray.initialize(vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 54.f), vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z - 1000.f));

						interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);
						vec3_t down_point = trace.end;
						trace_t trace2;
						trace_filter flt2(g::local);
						ray_t ray2;
						ray2.initialize(vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 54.f), vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 1000.f));

						interfaces::trace_ray->trace_ray(ray2, MASK_PLAYERSOLID, &flt2, &trace2);
						vec3_t upper_point = trace2.end;

						if (g::local->origin().z + 1.f < down_point.z)
							continue;
						if (g::local->origin().z + 1.f > upper_point.z)
							continue;
					}
					vec3_t un_pred_velo = g::local->get_velocity();

					prediction::begin(cmd);
					prediction::end();

					vec3_t pred_velo = g::local->get_velocity();
					{
						if (g::local->get_velocity().z == targetZvelo) {
							if (WallNormal.x < 0 && WallNormal.y < 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x + test_al,
									g::local->origin().y + test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x < 0 && WallNormal.y > 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x + test_al,
									g::local->origin().y - test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x > 0 && WallNormal.y < 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x - test_al,
									g::local->origin().y + test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x > 0 && WallNormal.y > 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x - test_al,
									g::local->origin().y - test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x == 0.f && WallNormal.y > 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x, g::local->origin().y - test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x == 0.f && WallNormal.y < 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x, g::local->origin().y + test_al,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x < 0 && WallNormal.y == 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x + test_al, g::local->origin().y,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
							else if (WallNormal.x > 0 && WallNormal.y == 0.f) {
								Point_bounce.emplace_back(vec3_t(g::local->origin().x - test_al, g::local->origin().y,
									g::local->origin().z + 54.f));
								to_muchPoints += 1;
								FirstPoint = false;
							}
						}
					}
					if (un_pred_velo.z < 0.f && pred_velo.z < 0.f)
						break;
				}
				if (done_and_break)
					break;
			}
			cmd->buttons = buttons;
			cmd->forward_move = ForwardMove;

			prediction::restore_ent_to_predicted_frame(PredictionFrames);
			Lerp = 0.f;
			for (int i = 0; i < 10000; i++)
			{
				float align = 15.97803f;

				cmd->buttons |= in_jump;
				cmd->buttons |= in_duck;

				vec3_t Angles{ WallNormal.x * -1.f, WallNormal.y * -1.f, 0.f };
				vec3_t to_wall = Angles.to_angle();
				float rotation = deg2rad(to_wall.y - cmd->view_angles.y);
				float cos_rot = cos(rotation);
				float sin_rot = sin(rotation);
				float forwardmove = cos_rot * 10.f;
				float sidemove = -sin_rot * 10.f;
				cmd->forward_move = forwardmove;
				cmd->side_move = sidemove;

				if (g::local->get_velocity().z < 0) {
					static float Height = 0.f;
					if (StartPos.z > EndPos.z) {
						if (StartPos.z < 0.f)
							Height = (int)StartPos.z - 0.972092f - Lerp;
						else
							Height = (int)StartPos.z + 0.0287018f - Lerp;
					}
					else {
						if (EndPos.z < 0.f)
							Height = (int)EndPos.z - 0.972092f - Lerp;
						else
							Height = (int)EndPos.z + 0.0287018f - Lerp;
					}
					int maxLerp;
					if (StartPos.z > EndPos.z) {
						maxLerp = StartPos.z - EndPos.z;
					}
					else {
						maxLerp = EndPos.z - StartPos.z;
					}

					if (Lerp > maxLerp) {
						cmd->buttons = buttons;
						cmd->forward_move = ForwardMove;
						FirstPoint = false;

						break;
					}
					else {
						Lerp += 1.f;
					}

					if (WallNormal.x < 0 && WallNormal.y < 0.f) {
						g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y - test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x < 0 && WallNormal.y > 0.f) {
						g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y + test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x > 0 && WallNormal.y < 0.f) {
						g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y - test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x > 0 && WallNormal.y > 0.f) {
						g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y + test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x == 0.f && WallNormal.y > 0.f) {
						g::local->origin() = vec3_t(StartPos.x, StartPos.y + test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x == 0.f && WallNormal.y < 0.f) {
						g::local->origin() = vec3_t(StartPos.x, StartPos.y - test_al, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x < 0 && WallNormal.y == 0.f) {
						g::local->origin() = vec3_t(StartPos.x - test_al, StartPos.y, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					else if (WallNormal.x > 0 && WallNormal.y == 0.f) {
						g::local->origin() = vec3_t(StartPos.x + test_al, StartPos.y, Height);
						g::local->set_abs_origin(vec3_t(StartPos.x, StartPos.y, Height));
					}
					trace_t trace;
					trace_filter flt(g::local);
					ray_t ray;
					ray.initialize(vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 54.f), vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z - 1000.f));

					interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);
					vec3_t down_point = trace.end;
					trace_t trace2;
					trace_filter flt2(g::local);
					ray_t ray2;
					ray2.initialize(vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 54.f), vec3_t(g::local->origin().x, g::local->origin().y, g::local->origin().z + 1000.f));

					interfaces::trace_ray->trace_ray(ray2, MASK_PLAYERSOLID, &flt2, &trace2);
					vec3_t upper_point = trace2.end;

					if (g::local->origin().z + 1.f < down_point.z)
						continue;
					if (g::local->origin().z + 1.f > upper_point.z)
						continue;
				}
				vec3_t un_pred_velo = g::local->get_velocity();

				prediction::begin(cmd);
				prediction::end();

				vec3_t pred_velo = g::local->get_velocity();
				if (g::local->get_velocity().z == targetZvelo) {
					if (WallNormal.x < 0 && WallNormal.y < 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x + test_al, g::local->origin().y + test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x < 0 && WallNormal.y > 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x + test_al, g::local->origin().y - test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x > 0 && WallNormal.y < 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x - test_al, g::local->origin().y + test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x > 0 && WallNormal.y > 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x - test_al, g::local->origin().y - test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x == 0.f && WallNormal.y > 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x, g::local->origin().y - test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x == 0.f && WallNormal.y < 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x, g::local->origin().y + test_al,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x < 0 && WallNormal.y == 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x + test_al, g::local->origin().y,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
					else if (WallNormal.x > 0 && WallNormal.y == 0.f) {
						Points.emplace_back(vec3_t(g::local->origin().x - test_al, g::local->origin().y,
							g::local->origin().z));
						to_muchPoints += 1;
						FirstPoint = false;
					}
				}
			}
			cmd->buttons = buttons;
			cmd->forward_move = ForwardMove;
			FirstPoint = false;
			if (Points.size() > 0) {
				for (int i = 0; i < Points.size() - 1; ) {
					if ((int)Points[i].z - (int)Points[i + 1].z == 1) {
						Points.erase(Points.begin() + i);
					}
					else {
						++i;
					}
				}
			}
		}
	}
	else {
		if (GImGui == nullptr)
			return;
		Pixelsurf_points(Points);
		Bounce_points(Point_bounce);

		if (menu::checkkey(c::assist::pixelsurf_line_key, c::assist::pixelsurf_line_key_s)) {
			if (!StartPos.is_zero() && !EndPos.is_zero()) {
				ImVec2 start2D, end2D;
				if (im_render.world_to_screen(StartPos, &start2D) && im_render.world_to_screen(EndPos, &end2D)) {
					im_render.drawline(start2D.x, start2D.y, end2D.x, end2D.y, color_t(200, 215, 215, 255 / 1.25), 1.5f);
				}
			}
		}
	}
}

bool check(float a, float b)
{
	int a1 = (int)a;
	int b1 = (int)b;
	if (b < 0) {
		if (a1 == b1) {
			int a2 = (a - a1) * 100;
			int b2 = (b - b1) * 100;
			int a3 = (a - a1) * 1000;
			if (b2 == a2 || a2 + 1 == b2 || a2 + 2 == b2)
				return true;
			else
				return false;
		}
	}
	else {
		if (a1 == b1) {
			int a2 = (a - a1) * 100;
			int b2 = (b - b1) * 100;
			int a3 = (a - a1) * 1000;
			if (b2 == a2 || a2 == b2 - 1 || a2 == b2 - 2)
				return true;
			else
				return false;
		}
	}

	return false;
}

bool seccheckcros = false;
bool float_ground = false;
static bool do_fucking_strafe = false;
int index_continue = -1;
static int cmd_i = 0;

vec3_t aim_1{ };
vec3_t back_ang = { };
vec3_t save_delta{ };
vec3_t save_cur{ };
void features::movement::pixelsurf_assist(c_usercmd* cmd)
{
	seccheckcros = false;
	static bool HitJump{ false };
	static bool HitMiniJump{ false };
	static bool HitLongJump{ false };
	static bool HitCHop{ false };
	static bool HitMiniChop{ false };
	static bool HitJumpBug{ false };

	if (!g::local)
		return;
	back_ang = cmd->view_angles;
	if (!c::assist::pixelsurf_assist)
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!g::local->is_alive())
		return;
	if (m_pixelsurf_data.should_pixel_surf)
		return;

	float sv_gravity2 = interfaces::console->get_convar(("sv_gravity"))->get_float();
	float fTickInterval = interfaces::globals->interval_per_tick;
	float fTickRate = (fTickInterval > 0) ? (1.0f / fTickInterval) : 0.0f;
	float veloZoftick = (((sv_gravity2 / 2) / fTickRate) * -1.f);
	vec3_t ray_shit = g::local->abs_origin() + vec3_t(0.f, 0.f, 64.f);
	if (prediction_backup::velocity.z == veloZoftick)
		return;

	static bool off = false;
	static int timer_to_point = 0;
	if (menu::checkkey(c::assist::pixelsurf_point_key, c::assist::pixelsurf_point_key_s)) {
		if (menu::open)
			off = true;
		if (m_assist_t.set_point)
			off = true;

		if (timer_to_point < GetTickCount64() && !m_assist_t.set_point) {
			if (!off) {
				vec3_t view_angles = vec3_t(cmd->view_angles.x, cmd->view_angles.y, 0.f);
				const auto endPos = g::local->get_eye_pos() + vec3_t::fromAngle(view_angles) * 2000;
				trace_t trace;

				ray_t ray;
				ray.initialize(g::local->get_eye_pos(), endPos);
				trace_filter flt(g::local);

				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);

				features::movement::m_pixelsurf_points_check.emplace_back(
					round_pos(vec3_t(g::local->get_eye_pos() + (endPos - g::local->get_eye_pos()) * trace.flFraction)),
					interfaces::engine->get_level_name());
				myWindowDetect.AddWindow(
					5, std::format("point on: {}", round_pos(vec3_t(g::local->get_eye_pos() +
						(endPos - g::local->get_eye_pos()) * trace.flFraction))
						.z));
				timer_to_point = GetTickCount64() + 3000;
			}
		}
	}
	else {
		timer_to_point = GetTickCount64() + 50;
		off = false;
	}

	static int toggletime = 0;

	if (!menu::checkkey(c::assist::pixelsurf_assist_key, c::assist::pixelsurf_assist_key_s)) {
		assistJb::AlertJB = false;
		HITGODA = false;
		HitJump = false;
		HitMiniJump = false;
		HitLongJump = false;
		HitCHop = false;
		HitJumpBug = false;
		HitMiniChop = false;
		float_ground = false;
		cmd_i = 0;
		do_fucking_strafe = false;
		index_continue = -1;
		return;
	}

	if (features::movement::m_pixelsurf_points_check.size() > 0) {
		std::vector<std::pair<float, int>> points;

		for (int i = 0; i < features::movement::m_pixelsurf_points_check.size(); i++) {
			/*if (i == index_continue) continue;*/
			if (interfaces::engine->get_level_name() == features::movement::m_pixelsurf_points_check.at(i).map && features::movement::m_pixelsurf_points_check.at(i).active) {
				float Distance = g::local->origin().dist_to(features::movement::m_pixelsurf_points_check.at(i).pos);
				if (Distance <= features::movement::m_pixelsurf_points_check.at(i).radius) {
					points.push_back(std::make_pair(Distance, i));
				}
			}
		}

		//not rly optimized but ok
		std::sort(points.begin(), points.end());

		//points limit
		int max = std::min(c::assist::pixelsurf_max_points, (int)points.size());

		for (int point_index = 0; point_index < max; point_index++) {
			int index = points[point_index].second;

			if (index != -1)
			{
				vec3_t Surf = features::movement::m_pixelsurf_points_check.at(index).pos;
				auto isLargeAngleDifference = [](float yaw1, float yaw2) -> bool {
					float yawDifference = std::abs(yaw1 - yaw2);
					if (yawDifference > 180.0f)
						yawDifference = 360.0f - yawDifference;
					return yawDifference >= 90.f;
					};

				int BackupButtons = cmd->buttons;
				float ForwaMove = cmd->forward_move;
				float SideMove = cmd->side_move;

				vec3_t BackupVelo = g::local->get_velocity();
				vec3_t Origin = g::local->origin();
				float stamina = g::local->stamina();
				int BackupPredicted = interfaces::prediction->split->commands_predicted;
				static int timer = 0;
				static int ticks = 0;
				static int ljticks = 0;

				//we dont need to use the bind while pixelsurfing
				if (ps_data.pixelsurfing)
					return;
				if (!c::assist::assist_broke_hop) {
					if (cmd->tick_count > ticks && prediction_backup::flags & 1 && g::local->stamina() != 0.f)
						return;
				}

				//TODO: MAKE IT WORK PROPERLY (IT MAKES A LITTLE MORE THAN ZERO SENSE TO DO IT LIKE THIS)
				if (c::assist::assist_broke_hop) {
					if (cmd->tick_count > ticks && prediction_backup::flags & 1 && g::local->stamina() > c::assist::assist_stamina_value)
						return;
				}
				if (prediction_backup::velocity.z < 0.f)
					ljticks = 0;

				int g = (prediction_backup::flags & 1) ? 2 : 4;
				int hitgroudtick{ };
				vec3_t ChatPositions{ g::local->origin() };
				bool ChatStand{ };
				bool ChatDuck{ };
				static int TicksOut = 0;

				static vec3_t last{ };

				float BackupForwardMove = cmd->forward_move;
				float BackupSideMove = cmd->side_move;
				vec3_t angViewPointBackup = cmd->view_angles;
				float DeltaSalier = 0.5f;

				vec3_t DeltaAngle = (angViewPointBackup - last) * features::movement::m_pixelsurf_points_check.at(index).delta_strafe;
				last = cmd->view_angles;

				for (int v = 0; v < g; v++) {
					cmd->side_move = BackupSideMove;
					cmd->forward_move = BackupForwardMove;
					cmd->buttons = BackupButtons;
					cmd->view_angles = angViewPointBackup;
					bool jb = false;
					bool secjb = false;
					bool thrjb = false;
					bool forthjb = false;
					prediction::restore_ent_to_predicted_frame(BackupPredicted - 1);
					int wait = 0;
					int BackupPredicted2 = 0;
					int once = 0;
					vec3_t Calculate{ };
					float iCalcilate{ };
					int once_fix = 0;
					vec3_t currentAngle = cmd->view_angles;
					if (!HitJump && !HitMiniJump && !HitLongJump && !HitCHop && !HitJumpBug && !HitMiniChop) {
						for (int i = 0; i < c::assist::pixelsurf_assist_ticks; i++) {
							int max_tick_on_ground = 0;
							cmd->side_move = BackupSideMove;
							cmd->forward_move = BackupForwardMove;
							if (!(g::local->flags() & 1)) {
								if (c::assist::pixelsurf_assist_type == 1) {
									if (!isLargeAngleDifference(currentAngle.y, angViewPointBackup.y))
										currentAngle = (currentAngle + DeltaAngle).normalized().clamped();
									cmd->view_angles = currentAngle;
									features::movement::start_movement_fix(cmd);
									cmd->view_angles = angViewPointBackup;
									features::movement::end_movement_fix(cmd);
								}
							}
							if (c::movement::pixel_surf_fix) {
								if (prediction_backup::velocity.length_2d() >= 285.91f && once_fix == 0) {
									auto sv_airAcelerate = interfaces::console->get_convar("sv_airaccelerate");
									float Razn = ((prediction_backup::velocity.length_2d() + 2.f - 285.91f) / 12.f * 64.f);
									vec3_t velocity = prediction_backup::velocity * -1.f;
									velocity.z = 0.f;
									float rotation = deg2rad(velocity.to_angle().y - cmd->view_angles.y);
									float cos_rot = cos(rotation);
									float sin_rot = sin(rotation);

									float forwardmove = cos_rot * Razn;
									float sidemove = -sin_rot * Razn;
									cmd->forward_move = forwardmove;
									cmd->side_move = sidemove;
									once_fix = 1;
								}
							}

							if (g::local->flags() & 1) {
								max_tick_on_ground += 1;
								once += 1;
								trace_t trace;
								ray_t ray_t;

								vec3_t start = g::local->abs_origin();

								ray_t.initialize(start, ray_shit);

								trace_world_only flt;

								interfaces::trace_ray->trace_ray(ray_t, MASK_PLAYERSOLID, &flt, &trace);
								if ((trace.flFraction < 1.f) && (trace.plane.normal.z == 1.f))
									float_ground = true;
							}
							if (once == 1)
								ChatPositions = g::local->origin();

							if (v == 0) {
								if (g::local->flags() & 1)
									cmd->buttons |= in_jump;
							}
							if (v == 1) {
								if (g::local->flags() & 1) {
									cmd->buttons |= in_jump;
									cmd->buttons |= in_duck;
								}
							}

							if (!(g::local->flags() & 1) && v != 2 && v != 3 && v != 4) {
								cmd->buttons &= ~in_duck;
								cmd->buttons &= ~in_jump;
							}
							if (v == 2) {
								if (g::local->flags() & 1) {
									cmd->buttons |= in_jump;
								}
								cmd->buttons |= in_duck;
							}
							if (v == 3) {
								if (i == hitgroudtick) {
									cmd->buttons |= in_duck;
									cmd->buttons &= ~in_jump;
									once += 1;
								}
								else {
									cmd->buttons &= ~in_duck;
									cmd->buttons |= in_jump;
								}
							}

							vec3_t BackupVelo2 = g::local->get_velocity();
							vec3_t Origin2 = g::local->origin();

							int backflags = g::local->flags();
							prediction::begin(cmd);
							prediction::end();

							if (v == 1) {
								if (g::local->flags() & 1 && !(backflags & 1)) {
									if (!jb)
										hitgroudtick = i;
									jb = true;
								}
							}

							if (!(g::local->flags() & 1) && v != 3 && v != 4) {
								cmd->buttons &= ~in_duck;
								cmd->buttons &= ~in_jump;
							}
							int preedflags = g::local->flags();

							if (preedflags & 1)
								max_tick_on_ground += 1;
							if (max_tick_on_ground > 1)
								break;

							vec3_t PredictedLocalPlayerOrigin = g::local->origin();
							vec3_t PredictedLocalPlayerVelocity = g::local->get_velocity();
							iCalcilate = 0.f;
							if (!(v == 2)) {
								if (!iCalcilate && once == 1 && !(backflags & fl_ducking) && !(preedflags & fl_ducking) &&
									BackupVelo2.z > 0.f && PredictedLocalPlayerVelocity.z > 0.f) {
									if (wait > 2) {
										Calculate = Origin2;
										iCalcilate = Origin2.z - PredictedLocalPlayerOrigin.z;
										once += 1;
									}
									else
										wait += 1;
								}
							}
							else {
								if (!iCalcilate && once == 1 && BackupVelo2.z > 0.f && PredictedLocalPlayerVelocity.z > 0.f) {
									if (wait > 2) {
										Calculate = Origin2;
										iCalcilate = Origin2.z - PredictedLocalPlayerOrigin.z;
										once += 1;
									}
									else
										wait += 1;
								}
							}
							NewCmd_assist[i].forwardmove = cmd->forward_move;
							NewCmd_assist[i].sideMove = cmd->side_move;
							NewCmd_assist[i].viewangle = cmd->view_angles;
							save_cur = angViewPointBackup;
							save_delta = DeltaAngle;
							if (iCalcilate) {
								float z = 0;
								for (int k = 0; k < 512; k++) {
									float o = z * (z + 1) / 2;
									float sv_gravity = interfaces::console->get_convar(("sv_gravity"))->get_float();
									float fTickInterval = interfaces::globals->interval_per_tick;
									float fTickRate = (fTickInterval > 0) ? (1.0f / fTickInterval) : 0.0f;
									auto sq_ticks = fTickRate * fTickRate;
									float p = iCalcilate + ((sv_gravity / sq_ticks) * z);
									if (p > 0) {
										if (features::movement::m_pixelsurf_points_check.at(index).jump) {
											if (v == 0) {
												if (check(Calculate.z, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("jump to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (stand)"));
													}
													HitJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_jump) {
											if (v == 0) {
												if (check(Calculate.z + 9.f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("jump to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).minijump) {
											if (v == 1) {
												if (check(Calculate.z, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("mj to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (stand)"));
													}
													HitMiniJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_minijump) {
											if (v == 1) {
												if (check(Calculate.z + 9.f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("mj to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitMiniJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).longjump) {
											if (v == 1) {
												if (check(Calculate.z + 8.9999704f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("lj to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (stand)"));
													}
													HitLongJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_longjump) {
											if (v == 1) {
												if (check(Calculate.z + 9.f + 8.9999704f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("lj to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitLongJump = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).crouch_hop) {
											if (v == 2) {
												if (check(Calculate.z - 9.f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("crouch jump to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (stand)"));
													}
													HitCHop = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_crouch_hop) {
											if (v == 2) {
												if (check(Calculate.z, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("crouch jump to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitCHop = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).jumpbug) {
											if (v == 3) {
												if (check(Calculate.z, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("jb to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (stand)"));
													}
													HitJumpBug = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_jumpbug) {
											if (v == 3) {
												if (check(Calculate.z + 9.f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("jb to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitJumpBug = true;
													ticks = cmd->tick_count + i - 3;
													break;
												}
											}
										}

										if (features::movement::m_pixelsurf_points_check.at(index).c_mini_crouch_hop) {
											if (v == 2) {
												if (check(Calculate.z + 9.f, Surf.z)) {
													if (TicksOut < GetTickCount64()) {
														TicksOut = GetTickCount64() + 500;
														if (c::assist::assist_render)
															myWindowDetect.AddWindow(
																5, std::string("high crouch jump to surf ")
																.append(std::to_string(Surf.z))
																.append(" with ")
																.append(std::to_string(prediction_backup::origin.z))
																.append(" (crouch)"));
													}
													HitMiniChop = true;
													ticks = cmd->tick_count + i + 10;
													break;
												}
											}
										}
									}

									Calculate.z = Calculate.z - p;
									z += 1;
								}
								break;
							}
						}
					}
					else {
						break;
					}
					cmd->buttons = BackupButtons;
					cmd->forward_move = BackupForwardMove;
					cmd->side_move = BackupSideMove;
				}
				cmd->buttons = BackupButtons;
				cmd->forward_move = BackupForwardMove;
				cmd->side_move = BackupSideMove;
				prediction::restore_ent_to_predicted_frame(BackupPredicted - 1);

				if (cmd->tick_count > ticks) {
					ticks = 0;
					HITGODA = false;
					float_ground = false;
					HitJump = false;
					HitMiniJump = false;
					HitLongJump = false;
					HitCHop = false;
					HitJumpBug = false;
					HitMiniChop = false;
					assistJb::AlertJB = false;
					cmd_i = 0;
					do_fucking_strafe = false;
				}
				if (cmd->tick_count < ticks) {
					HITGODA = true;
					if (prediction_backup::velocity.z < 0.f || g::local->get_velocity().z < 0.f) {
						if (cmd_i > 127)
							cmd_i = 0;
						try {
							cmd->forward_move = NewCmd_assist[cmd_i].forwardmove;
							cmd->side_move = NewCmd_assist[cmd_i].sideMove;
							if (c::assist::pixelsurf_assist_type == 1) {
								cmd->view_angles = NewCmd_assist[cmd_i].viewangle;
								features::movement::start_movement_fix(cmd);
								cmd->view_angles = angViewPointBackup;
								features::movement::end_movement_fix(cmd);
							}
							cmd_i += 1;
						}
						catch (...) {
							cmd->forward_move = 0.f;
							cmd->side_move = 0.f;
						}

						do_fucking_strafe = true;
						cmd->buttons = 0;
					}
					else {
						do_fucking_strafe = false;
					}

					if (prediction_backup::velocity.z > 0.f) {
						if (HitJumpBug) {}
						HitCHop = false;
						HitJumpBug = false;
					}

					if (HitCHop) {
						cmd->buttons |= in_duck;
						if (prediction_backup::flags & 1)
							cmd->buttons |= in_jump;
					}
					if (HitJump && prediction_backup::flags & 1) {
						cmd->buttons |= in_jump;
						HitJump = false;
					}
					if (HitMiniJump && prediction_backup::flags & 1) {
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
						HitMiniJump = false;
					}
					if (HitLongJump && prediction_backup::flags & 1) {
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
						ljticks = interfaces::globals->tick_count + 3;
						HitLongJump = false;
					}
					if (HitMiniChop) {
						alert_mini_crouch_hop = true;
					}
					if (ljticks > interfaces::globals->tick_count)
						cmd->buttons |= in_duck;
					else {
						if (prediction_backup::velocity.z > 0)
							ticks = 0;
					}
					assistJb::AlertJB = HitJumpBug;
				}
				else {
					HITGODA = false;
				}
			}
			if (HITGODA || HitJump || HitMiniJump || HitLongJump || HitCHop || HitJumpBug || HitMiniChop) {
				break;
			}
			if (!c::movement::bhopfix) {
				prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			}
		}
	}
}

bool issimilarlowfractionpair(double a, double b)
{
	if (a > 0 && b > 0) {
		double frac_a = a - std::floor(a);
		double frac_b = b - std::floor(b);
		if (frac_a > 0 && frac_a < 0.3 && frac_b > 0 && frac_b < 0.3) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (a < 0 && b < 0) {
		int i_a = (int)a;
		int i_b = (int)b;
		if (i_a != i_b) {
			return false;
		}
		float outofpoint_a = a - i_a;
		float outofpoint_b = b - i_b;
		int full_a = std::abs(outofpoint_a * 1000000.f);
		int full_b = std::abs(outofpoint_b * 1000000.f);
		if (full_b <= full_a && full_a < 990000) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool seccheckcros2 = false;

void features::movement::bounce_assist(c_usercmd* cmd)
{
	seccheckcros2 = false;
	static bool HitJump{ false };
	static bool HitMiniJump{ false };
	static bool HitLongJump{ false };
	static bool HitCHop{ false };
	static bool HitMiniChop{ false };
	static bool HitJumpBug{ false };

	if (!c::assist::bounce_assist)
		return;

	if (!g::local)
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!g::local->is_alive())
		return;
	if (!g::local->collideable())
		return;

	static int timer_to_point = 0;
	static bool off = false;
	if (menu::checkkey(c::assist::bounce_point_key, c::assist::bounce_point_key_s)) {
		if (menu::open)
			off = true;
		if (m_assist_t.set_point)
			off = true;
		if (bounce_set_point)
			off = true;
		if (timer_to_point < GetTickCount64() && !m_assist_t.set_point && !bounce_set_point) {
			if (!off) {
				vec3_t viewangle = vec3_t(cmd->view_angles.x, cmd->view_angles.y, 0.f);
				const auto endPos = g::local->get_eye_pos() + vec3_t::fromAngle(viewangle) * 2000;
				trace_t trace;

				ray_t ray;
				ray.initialize(g::local->get_eye_pos(), endPos);

				trace_filter flt(g::local);

				interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);

				features::movement::m_bounce_points_check.emplace_back(
					round_pos2(vec3_t(g::local->get_eye_pos() + (endPos - g::local->get_eye_pos()) * trace.flFraction)),
					interfaces::engine->get_level_name());
				myWindowDetect.AddWindow(
					5, std::format("point on: {}", round_pos2(vec3_t(g::local->get_eye_pos() +
						(endPos - g::local->get_eye_pos()) * trace.flFraction))
						.z));
				timer_to_point = GetTickCount64() + 3000;
			}
		}
	}
	else {
		timer_to_point = GetTickCount64() + 50;
		off = false;
	}

	static int longjump_ticks = 0;
	static int toggletime = 0;
	static int skip_one_tick = 0;
	static int ljtickshold = 0;
	static int crtickshold = 0;
	static float bfwd = 0.f;
	static float bside = 0.f;
	static int skip_ground = 0;
	static int jumpcrhld = 0;
	static int holdducklj = 0;
	static int holdduckmj = 0;
	static bool ChatStand{ };
	static int skip_mini_jump = 0;
	static int duck_in_jump_tick = 0;

	if (!menu::checkkey(c::assist::bounce_assist_key, c::assist::bounce_assist_key_s)) {
		assistJb::AlertJB2 = false;
		HITGODA2 = false;
		HitJump = false;
		HitMiniJump = false;
		HitLongJump = false;
		HitMiniChop = false;
		HitCHop = false;
		HitJumpBug = false;
		skip_one_tick = 0;
		ljtickshold = 0;
		crtickshold = 0;
		bfwd = 0.f;
		bside = 0.f;
		skip_ground = 0;
		jumpcrhld = 0;
		holdducklj = 0;
		holdduckmj = 0;
		ChatStand = false;
		longjump_ticks = 0;
		skip_mini_jump = 0;
		duck_in_jump_tick = 0;
		return;
	}

	if (features::movement::m_bounce_points_check.size() > 0) {
		int index = -1;
		float Nearest = FLT_MAX;
		std::vector< vec3_t > surf{ };
		std::vector< int > surf_index{ };


		for (int i = 0; i < features::movement::m_bounce_points_check.size(); i++) {
			if (interfaces::engine->get_level_name() == features::movement::m_bounce_points_check.at(i).map &&
				features::movement::m_bounce_points_check.at(i).active) {
				float Distance = g::local->origin().dist_to_2d(features::movement::m_bounce_points_check.at(i).pos);
				if (Distance <= features::movement::m_bounce_points_check.at(i).radius) {
					surf.emplace_back(features::movement::m_bounce_points_check.at(i).pos);
					surf_index.emplace_back(i);
					if (Distance < Nearest) {
						Nearest = Distance;
						index = i;
					}
				}
			}
		}

		if (index != -1)
		{
			vec3_t Surf = features::movement::m_bounce_points_check.at(index).pos;

			vec3_t Calculate{ };
			vec3_t Calculate2{ };
			float iCalcilate{ };

			int BackupButtons = cmd->buttons;
			float ForwaMove = cmd->forward_move;
			float SideMove = cmd->side_move;

			vec3_t BackupVelo = g::local->get_velocity();
			vec3_t Origin = g::local->origin();
			float stamina = g::local->stamina();
			int BackupPredicted = interfaces::prediction->split->commands_predicted;
			static int timer = 0;
			static int ticks = 0;
			static int ljticks = 0;
			static int mjticks = 0;

			static vec3_t last{ };
			vec3_t angViewPointBackup = cmd->view_angles;
			vec3_t DeltaAngle = (angViewPointBackup - last) * features::movement::m_bounce_points_check.at(index).delta_strafe;
			last = cmd->view_angles;

			if (!c::assist::assist_bounce_broke_hop) {
				if (cmd->tick_count > ticks && prediction_backup::flags & 1 && g::local->stamina() != 0.f)
					return;
			}
			if (prediction_backup::velocity.z < 0.f)
				ljticks = 0;

			int g = (prediction_backup::flags & 1) ? 3 : 5;
			int hitgroudtick{ };
			vec3_t ChatPositions{ g::local->origin() };
			bool ChatDuck{ };
			int bheight = 0;

			if (!HITGODA2) {
				for (int v = 0; v < g; v++) {
					cmd->side_move = 0.f;
					cmd->forward_move = 0.f;
					cmd->buttons = BackupButtons;
					bool jb = false;
					bool secjb = false;
					bool thrjb = false;
					bool forthjb = false;
					prediction::restore_ent_to_predicted_frame(BackupPredicted - 1);

					int BackupPredicted2 = 0;
					int once = 0;
					int ticksljinpred = 0;

					if (!HitJump && !HitMiniJump && !HitLongJump && !HitCHop && !HitJumpBug && !(g::local->flags() & fl_ducking) &&
						!HitMiniChop) {
						for (int i = 0; i < 48; i++) {
							if (HitJump || HitMiniJump || HitLongJump || HitCHop || HitJumpBug)
								break;
							int max_ground_ticks = 0;

							if (g::local->flags() & 1) {
								max_ground_ticks += 1;
								once += 1;
							}
							if (once == 1)
								ChatPositions = g::local->origin();

							if (v == 0) {
								if (g::local->flags() & 1)
									cmd->buttons |= in_jump;
							}
							if (v == 1) {
								if (g::local->flags() & 1) {
									cmd->buttons |= in_jump;
									cmd->buttons |= in_duck;
								}
							}
							if (v == 2) {
								if (ticksljinpred > 4)
									ticksljinpred = 0;

								if (ticksljinpred == 0) {
									cmd->buttons &= ~in_duck;
									cmd->buttons &= ~in_jump;
								}

								if (g::local->flags() & 1) {
									cmd->buttons |= in_jump;
									cmd->buttons |= in_duck;
									ticksljinpred += 1;
								}

								if (ticksljinpred > 0) {
									cmd->buttons |= in_jump;
									cmd->buttons |= in_duck;
									ticksljinpred += 1;
								}
							}

							if (!(g::local->flags() & 1) && v != 2 && v != 4) {
								cmd->buttons &= ~in_duck;
								cmd->buttons &= ~in_jump;
							}

							if (v == 3) {
								if (g::local->flags() & 1) {
									cmd->buttons |= in_jump;
								}
								cmd->buttons |= in_duck;
							}
							if (v == 4) {
								if (i == hitgroudtick) {
									cmd->buttons |= in_duck;
									cmd->buttons &= ~in_jump;
									once += 1;
								}
								else {
									cmd->buttons &= ~in_duck;
									cmd->buttons |= in_jump;
								}
							}

							vec3_t BackupVelo2 = g::local->get_velocity();
							vec3_t Origin2 = g::local->origin();

							int backflags = g::local->flags();
							prediction::begin(cmd);
							prediction::end();
							if (g::local->flags() & 1)
								max_ground_ticks += 1;
							if (v == 0) {
								if (g::local->flags() & 1 && !(backflags & 1)) {
									if (!jb)
										hitgroudtick = i;
									jb = true;
								}
							}
							if (max_ground_ticks > 1)
								break;
							if (!(g::local->flags() & 1) && v != 3 && v != 4) {
								cmd->buttons &= ~in_duck;
								cmd->buttons &= ~in_jump;
							}

							vec3_t PredictedLocalPlayerOrigin = g::local->abs_origin();
							vec3_t PredictedLocalPlayerVelocity = g::local->get_velocity();
							int flags = g::local->flags();
							iCalcilate = 0.f;

							Calculate = PredictedLocalPlayerOrigin + g::local->collideable()->maxs() + vec3_t(0.f, 0.f, 0.04f);
							int heightlol = (int)g::local->collideable()->maxs().z;

							if (PredictedLocalPlayerVelocity.z > 0.f) {
								if (once == 1) {
									for (int j = 0; j < surf.size(); j++) {
										auto& point = features::movement::m_bounce_points_check.at(surf_index[j]);

										if (v == 0) {
											if (point.jump && issimilarlowfractionpair(Calculate.z, surf[j].z)) {
												HitJump = true;
												ChatStand = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
											if (point.c_jump && heightlol == 72 && issimilarlowfractionpair(Calculate.z - 9.f, surf[j].z)) {
												HitJump = true;
												ChatStand = false;
												ChatDuck = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z - 9.f).c_str());
												break;
											}
										}
										else if (v == 1) {
											if (point.minijump && issimilarlowfractionpair(Calculate.z, surf[j].z)) {
												HitMiniJump = true;
												ChatStand = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
											if (point.c_minijump && heightlol == 72 && bheight == 72 &&
												issimilarlowfractionpair(Calculate.z - 9.f, surf[j].z)) {
												HitMiniJump = true;
												ChatStand = false;
												ChatDuck = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z - 9.f).c_str());
												break;
											}
											bheight = heightlol;
										}
										else if (v == 3) {
											if (point.crouch_hop && issimilarlowfractionpair(Calculate.z, surf[j].z)) {
												HitCHop = true;
												ChatStand = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
											if (point.c_crouch_hop && heightlol == 54 && issimilarlowfractionpair(Calculate.z + 9.f, surf[j].z)) {
												HitCHop = true;
												ChatStand = false;
												ChatDuck = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z + 9.f).c_str());
												break;
											}
											if (point.c_mini_crouch_hop && issimilarlowfractionpair(Calculate.z + 9.f, surf[j].z)) {
												HitMiniChop = true;
												ChatStand = false;
												ChatDuck = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
										}
										else if (v == 4) {
											if (point.jumpbug && issimilarlowfractionpair(Calculate.z, surf[j].z)) {
												HitJumpBug = true;
												ChatStand = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
										}
										else if (v == 2) {
											if (point.longjump && issimilarlowfractionpair(Calculate.z, surf[j].z)) {
												HitLongJump = true;
												ChatStand = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z).c_str());
												break;
											}
											if (point.c_longjump && heightlol == 72 && issimilarlowfractionpair(Calculate.z - 9.f, surf[j].z)) {
												HitLongJump = true;
												ChatStand = false;
												ChatDuck = true;
												ticks = cmd->tick_count + i + 5;
												interfaces::engine->execute_client_cmd(
													std::format("echo detected: {} \n", Calculate.z - 9.f).c_str());
												break;
											}
										}
									}
								}
							}

							if (HitJump || HitMiniJump || HitLongJump || HitCHop || HitJumpBug || HitMiniChop)
								break;
						}
					}
					else {
						break;
					}
				}
			}

			cmd->buttons = BackupButtons;
			cmd->forward_move = ForwaMove;
			cmd->side_move = SideMove;
			//restoring twice? why?
			//prediction::restore_ent_to_predicted_frame(BackupPredicted - 1);
			prediction::restore_ent_to_predicted_frame(BackupPredicted - 1);
			static int TicksOut = 0;

			if (cmd->tick_count > ticks) {
				ticks = 0;
				HITGODA2 = false;
				skip_one_tick = 0;
				skip_ground = 0;
				HitJump = false;
				jumpcrhld = 0;
				holdducklj = 0;
				holdduckmj = 0;
				HitMiniJump = false;
				HitMiniChop = false;
				ChatStand = false;
				longjump_ticks = 0;
				skip_mini_jump = 0;
				duck_in_jump_tick = 0;
			}
			if (cmd->tick_count < ticks) {
				HITGODA2 = true;
				if (skip_ground == 0) {
					cmd->forward_move = 0.f;
					cmd->side_move = 0.f;
					cmd->buttons = 0;
					cmd->mouse_dx = 0;
					cmd->mouse_dy = 0;
				}

				if (prediction_backup::flags & 1)
					skip_ground += 1;
				if (prediction_backup::velocity.z > 0.f) {
					if (HitJumpBug) {
						if (!ChatPositions.is_zero()) {
							if (TicksOut < GetTickCount64()) {
								TicksOut = GetTickCount64() + 500;
								if (c::assist::bounce_assist_render)
									myWindowDetect.AddWindow(5, std::string("jb to head bounce ")
										.append(std::to_string(Surf.z))
										.append(" with ")
										.append(std::to_string(prediction_backup::origin.z))
										.append(!ChatStand ? " (crouch)" : " (stand)"));
							}
						}
					}
					HitJumpBug = false;
				}

				if (HitJump && ChatStand) {
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("jump to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
					}
				}
				if (HitJump && !ChatStand) {
					if (duck_in_jump_tick > interfaces::globals->tick_count)
						cmd->buttons |= in_duck;
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("jump to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
						duck_in_jump_tick = interfaces::globals->tick_count + 32;
					}
				}
				if (HitMiniJump && ChatStand) {
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("mj to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
					}
				}
				if (HitMiniJump && !ChatStand) {
					if (skip_mini_jump > 1)
						cmd->buttons |= in_duck;
					if (skip_mini_jump > 0)
						skip_mini_jump += 1;
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("mj to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
						skip_mini_jump += 1;
					}
				}
				if (HitLongJump && ChatStand) {
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("lj to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
						longjump_ticks = interfaces::globals->tick_count + 2;
					}
					if (longjump_ticks > interfaces::globals->tick_count)
						cmd->buttons |= in_duck;
				}
				if (HitLongJump && !ChatStand) {
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("lj to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
						cmd->buttons |= in_duck;
						longjump_ticks = interfaces::globals->tick_count + 32;
					}
					if (longjump_ticks > interfaces::globals->tick_count)
						cmd->buttons |= in_duck;
				}

				if (HitCHop && ChatStand) {
					if (prediction_backup::flags & 1) {
						cmd->buttons |= in_jump;
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("crouch jump to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
					}
					cmd->buttons |= in_duck;
				}
				if (HitCHop && !ChatStand) {
					if (prediction_backup::flags & 1) {
						if (c::assist::bounce_assist_render)
							myWindowDetect.AddWindow(5, std::string("crouch jump to head bounce ")
								.append(std::to_string(Surf.z))
								.append(" with ")
								.append(std::to_string(prediction_backup::origin.z))
								.append(!ChatStand ? " (crouch)" : " (stand)"));
						cmd->buttons |= in_jump;
					}
					if (!(prediction_backup::flags & 1) && prediction_backup::velocity.z < 0.f) {
						cmd->buttons |= in_duck;
					}
				}

				if (HitMiniChop) {
					if (c::assist::bounce_assist_render && !bounce_alert_mini_crouch_hop)
						myWindowDetect.AddWindow(5, std::string("high crouch jump to head bounce ")
							.append(std::to_string(Surf.z))
							.append(" with ")
							.append(std::to_string(prediction_backup::origin.z))
							.append(!ChatStand ? " (crouch)" : " (stand)"));
					bounce_alert_mini_crouch_hop = true;
				}
				assistJb::AlertJB2 = HitJumpBug;

				if (skip_ground != 0) {
					auto colidable = g::local->collideable();

					if (!colidable)
						return;

					trace_t trace;
					float step = std::numbers::pi_v< float > *2.0f / 16.f;
					float start_circle = 0.f;

					vec3_t wall_position{ };

					for (float a = start_circle; a < std::numbers::pi_v< float > *2.0f; a += step) {
						vec3_t wishdir = vec3_t(cosf(a), sinf(a), 0.f);
						const auto start_pos = g::local->abs_origin();
						const auto end_pos = start_pos + wishdir;
						trace_filter flt(g::local);
						ray_t ray;
						ray.initialize(start_pos, end_pos, colidable->mins(), colidable->maxs());

						interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &flt, &trace);

						if (trace.flFraction < 1.f && trace.plane.normal.z == 0.f) {
							vec3_t NormalPlane = vec3_t(trace.plane.normal.x * -1.f, trace.plane.normal.y * -1.f, 0.f);
							vec3_t WallAngle = NormalPlane.to_angle();

							float rotation = deg2rad(WallAngle.y - (cmd->view_angles.y + DeltaAngle.y));
							float cos_rot = cos(rotation);
							float sin_rot = sin(rotation);
							float multiplayer = 6.f;

							float forwardmove = cos_rot * multiplayer;
							float sidemove = -sin_rot * multiplayer;
							cmd->forward_move = forwardmove;
							cmd->side_move = sidemove;
						}
					}
				}
			}
			else {
				HITGODA2 = false;
			}
			if (!c::movement::bhopfix) {
				prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			}
		}
	}
}

void features::movement::assist_render()
{
	if (!c::assist::pixelsurf_assist && !c::assist::bounce_assist)
		return;

	if (!g::local)
		return;
	if (!interfaces::engine->is_in_game())
		return;
	if (!g::local->is_alive())
		return;

	static bool isBackspacePressed = false;

	if (GetAsyncKeyState(VK_BACK) & 0x8000) {
		isBackspacePressed = true;
	}
	else {
		isBackspacePressed = false;
	}

	if (features::movement::m_pixelsurf_points_check.size() > 0 || features::movement::m_bounce_points_check.size() > 0) {}
}

float LerpFloat(float a, float b, float t)
{
	return a + (b - a) * t;
}

bool isEnterPressed()
{
	static bool keyWasDown = false;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
		keyWasDown = true;
	}
	else if (keyWasDown) {
		keyWasDown = false;
		return true;
	}
	return false;
}

void RenderPoints(std::vector< features::movement::points_check_t >& points, const vec3_t& playerPos, const std::string& currentMap)
{
	if (GImGui == nullptr)
		return;
	float dt = ImGui::GetIO().DeltaTime;
	const float scaleSpeed = 10.0f;
	static bool isBackspacePressed = false;

	if (GetAsyncKeyState(VK_BACK) & 0x8000) {
		isBackspacePressed = true;
	}
	else {
		isBackspacePressed = false;
	}

	auto accentclr = ImVec4(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2], 1.f);			//?????????????????????? //lobommymktmy  tk prod kurai

	if (c::assist::pixelsurf_assist) {
		for (size_t i = 0; i < points.size(); ++i) {
			features::movement::points_check_t& point = points[i];

			if (point.map != currentMap)
				continue;

			ImVec2 screenPos;
			if (!im_render.world_to_screen(point.pos, &screenPos))
				continue;

			float distance = playerPos.dist_to(point.pos);
			if (distance > 1500.f)
				continue;

			float baseRadius = 11.0f;
			if (distance > 300.0f) {
				float t = std::clamp((distance - 300.0f) / 300.0f, 0.0f, 1.0f);
				baseRadius = LerpFloat(11.0f, 4.0f, t);
			}
			baseRadius = std::max(baseRadius, 3.0f);

			float outlineThickness = 0.5f;

			float alphaMultiplier = 1.0f;
			if (distance > 1400.0f) {
				alphaMultiplier = std::clamp((1500.0f - distance) / 100.0f, 0.0f, 1.0f);
			}


			color_t outlineColor;
			if (point.active) {
				int outlineR = static_cast<int>(accentclr.x * 255.f);
				int outlineG = static_cast<int>(accentclr.y * 255.f);
				int outlineB = static_cast<int>(accentclr.z * 255.f);
				int outlineA = static_cast<int>(255.0f * alphaMultiplier);
				outlineColor = color_t(outlineR, outlineG, outlineB, outlineA);
			}
			else {
				int gray = 100;
				int outlineA = static_cast<int>(255.0f * alphaMultiplier);
				outlineColor = color_t(gray, gray, gray, outlineA);
			}

			int fillAlpha = static_cast<int>(100.0f * alphaMultiplier);
			auto fillColor = color_t(0, 0, 0, fillAlpha);

			bool isHovered = InCrosshair(screenPos.x, screenPos.y, baseRadius);
			float targetScale = isHovered ? 1.5f : 1.0f;
			point.currentScale = LerpFloat(point.currentScale, targetScale, dt * scaleSpeed);
			float effectiveRadius = baseRadius * point.currentScale;

			if (isHovered) {
				if (!point.open_settings) {
					if (isEnterPressed()) {
						gloabal_settings_timer = GetTickCount64() + 500;
						point.open_settings = true;
					}
				}
			}
			else {
				point.open_settings = false;
			}

			im_render.drawcirclefilled(screenPos.x, screenPos.y, effectiveRadius, 32, fillColor);

			//outline circle which looks 1000 times better
			im_render.drawcircle(screenPos.x, screenPos.y, effectiveRadius, 32, outlineColor, outlineThickness);

			float animSpeed = 2.0f;
			float anim = (std::sin(ImGui::GetTime() * animSpeed) + 1.0f) / 2.0f;
			float maxCrossAlpha = 200.0f;
			if (distance > 300.0f) {
				float fade = std::clamp((distance - 300.0f) / 300.0f, 0.0f, 1.0f);
				maxCrossAlpha = 200.0f * (1.0f - fade);
			}
			int crossAlpha = static_cast<int>(maxCrossAlpha * anim);

			color_t crossColor;
			if (point.active) {
				crossColor = color_t(int(accentclr.x * 255.f), int(accentclr.y * 255.f), int(accentclr.z * 255.f), crossAlpha);
			}
			else {
				int gray = 150;
				crossColor = color_t(gray, gray, gray, crossAlpha);
			}

			float baseLineLength = 4.0f;
			float effectiveLineLength = baseLineLength * point.currentScale;

			auto screenDistance = [](const ImVec2& a, const ImVec2& b) -> float {
				float dx = a.x - b.x;
				float dy = a.y - b.y;
				return std::sqrt(dx * dx + dy * dy);
				};

			auto rotateVector = [&](const vec3_t& v) -> vec3_t {
				float x1 = v.x * cos(0.f) + v.z * sin(0.f);
				float y1 = v.y;
				float z1 = -v.x * sin(0.f) + v.z * cos(0.f);
				float x2 = x1;
				float y2 = y1 * cos(0.f) - z1 * sin(0.f);
				float z2 = y1 * sin(0.f) + z1 * cos(0.f);
				return vec3_t(x2, y2, z2);
				};

			float maxProjectedOffset = 0.0f;
			auto getScreenOffset = [&](const vec3_t& offsetVec) -> float {
				ImVec2 projected;
				if (im_render.world_to_screen(point.pos + offsetVec, &projected))
					return screenDistance(screenPos, projected);
				return 0.0f;
				};

			{
				vec3_t off = rotateVector(vec3_t(effectiveLineLength, 0, 0));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}
			{
				vec3_t off = rotateVector(vec3_t(0, effectiveLineLength, 0));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}
			{
				vec3_t off = rotateVector(vec3_t(0, 0, effectiveLineLength));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}

			if (maxProjectedOffset > effectiveRadius && effectiveRadius > 0.0f) {
				float scaleFactor = effectiveRadius / maxProjectedOffset;
				effectiveLineLength *= scaleFactor;
			}

			vec3_t offsetX = rotateVector(vec3_t(effectiveLineLength, 0, 0));
			vec3_t offsetY = rotateVector(vec3_t(0, effectiveLineLength, 0));
			vec3_t offsetZ = rotateVector(vec3_t(0, 0, effectiveLineLength));

			vec3_t center = point.pos;
			vec3_t xPos = center + offsetX;
			vec3_t xNeg = center - offsetX;
			vec3_t yPos = center + offsetY;
			vec3_t yNeg = center - offsetY;
			vec3_t zPos = center + offsetZ;
			vec3_t zNeg = center - offsetZ;

			ImVec2 screenX1, screenX2, screenY1, screenY2, screenZ1, screenZ2;
			bool validX1 = im_render.world_to_screen(xPos, &screenX1);
			bool validX2 = im_render.world_to_screen(xNeg, &screenX2);
			bool validY1 = im_render.world_to_screen(yPos, &screenY1);
			bool validY2 = im_render.world_to_screen(yNeg, &screenY2);
			bool validZ1 = im_render.world_to_screen(zPos, &screenZ1);
			bool validZ2 = im_render.world_to_screen(zNeg, &screenZ2);

			if (validX1 && validX2) {
				im_render.drawline(screenX1.x, screenX1.y, screenX2.x, screenX2.y, crossColor);
			}
			if (validY1 && validY2) {
				im_render.drawline(screenY1.x, screenY1.y, screenY2.x, screenY2.y, crossColor);
			}
			if (validZ1 && validZ2) {
				im_render.drawline(screenZ1.x, screenZ1.y, screenZ2.x, screenZ2.y, crossColor);
			}

			if (isHovered && isBackspacePressed) {
				if (point.open_settings) {
					point.open_settings = false;
					features::movement::m_opened = false;
				}
				points.erase(points.begin() + i);
				break;
			}
		}
		bool omaga = false;
		for (const auto& point : points) {
			if (point.map == currentMap && point.open_settings) {
				omaga = true;
				break;
			}
		}
		if (!omaga) {
			features::movement::m_opened = false;
		}
	}

	if (c::assist::bounce_assist) {
		for (size_t i = 0; i < features::movement::m_bounce_points_check.size(); ++i) {
			features::movement::points_check_t& point = features::movement::m_bounce_points_check[i];

			if (point.map != currentMap)
				continue;

			ImVec2 screenPos;
			if (!im_render.world_to_screen(point.pos, &screenPos))
				continue;

			float distance = playerPos.dist_to(point.pos);
			if (distance > 1500.f)
				continue;

			float baseRadius = 11.0f;
			if (distance > 300.0f) {
				float t = std::clamp((distance - 300.0f) / 300.0f, 0.0f, 1.0f);
				baseRadius = LerpFloat(11.0f, 4.0f, t);
			}
			baseRadius = std::max(baseRadius, 5.0f);

			float outlineThickness = 0.5f;

			float alphaMultiplier = 1.0f;
			if (distance > 1400.0f) {
				alphaMultiplier = std::clamp((1500.0f - distance) / 100.0f, 0.0f, 1.0f);
			}

			color_t outlineColor;
			if (point.active) {
				int outlineA = static_cast<int>(255.0f * alphaMultiplier);
				outlineColor = color_t(255, 0, 0, outlineA);
			}
			else {
				int gray = 100;
				int outlineA = static_cast<int>(255.0f * alphaMultiplier);
				outlineColor = color_t(gray, gray, gray, outlineA);
			}

			int fillAlpha = static_cast<int>(100.0f * alphaMultiplier);
			color_t fillColor = color_t(0, 0, 0, fillAlpha);

			bool isHovered = InCrosshair(screenPos.x, screenPos.y, baseRadius);
			if (isHovered) {
				if (!point.open_settings) {
					if (isEnterPressed()) {
						gloabal_settings_timer = GetTickCount64() + 500;
						point.open_settings = true;
					}
				}
			}
			else {
				point.open_settings = false;
			}

			float targetScale = isHovered ? 1.5f : 1.0f;
			point.currentScale = LerpFloat(point.currentScale, targetScale, dt * scaleSpeed);
			float effectiveRadius = baseRadius * point.currentScale;

			im_render.drawcirclefilled(screenPos.x, screenPos.y, effectiveRadius, 32, fillColor);

			im_render.drawcircle(screenPos.x, screenPos.y, effectiveRadius, 32, outlineColor, outlineThickness);

			float animSpeed = 2.0f;
			float anim = (std::sin(ImGui::GetTime() * animSpeed) + 1.0f) / 2.0f;
			float maxCrossAlpha = 200.0f;
			if (distance > 300.0f) {
				float fade = std::clamp((distance - 300.0f) / 300.0f, 0.0f, 1.0f);
				maxCrossAlpha = 200.0f * (1.0f - fade);
			}
			int crossAlpha = static_cast<int>(maxCrossAlpha * anim);

			color_t crossColor;
			if (point.active) {
				crossColor = color_t(int(255.f), int(0.f), int(0.f), crossAlpha);
			}
			else {
				int gray = 150;
				crossColor = color_t(gray, gray, gray, crossAlpha);
			}

			float baseLineLength = 4.0f;
			float effectiveLineLength = baseLineLength * point.currentScale;

			auto screenDistance = [](const ImVec2& a, const ImVec2& b) -> float {
				float dx = a.x - b.x;
				float dy = a.y - b.y;
				return std::sqrt(dx * dx + dy * dy);
				};

			auto rotateVector = [&](const vec3_t& v) -> vec3_t {
				float x1 = v.x * cos(0.f) + v.z * sin(0.f);
				float y1 = v.y;
				float z1 = -v.x * sin(0.f) + v.z * cos(0.f);
				float x2 = x1;
				float y2 = y1 * cos(0.f) - z1 * sin(0.f);
				float z2 = y1 * sin(0.f) + z1 * cos(0.f);
				return vec3_t(x2, y2, z2);
				};

			float maxProjectedOffset = 0.0f;
			auto getScreenOffset = [&](const vec3_t& offsetVec) -> float {
				ImVec2 projected;
				if (im_render.world_to_screen(point.pos + offsetVec, &projected))
					return screenDistance(screenPos, projected);
				return 0.0f;
				};

			{
				vec3_t off = rotateVector(vec3_t(effectiveLineLength, 0, 0));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}
			{
				vec3_t off = rotateVector(vec3_t(0, effectiveLineLength, 0));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}
			{
				vec3_t off = rotateVector(vec3_t(0, 0, effectiveLineLength));
				maxProjectedOffset = std::max(maxProjectedOffset, getScreenOffset(off));
			}

			if (maxProjectedOffset > effectiveRadius && effectiveRadius > 0.0f) {
				float scaleFactor = effectiveRadius / maxProjectedOffset;
				effectiveLineLength *= scaleFactor;
			}

			vec3_t offsetX = rotateVector(vec3_t(effectiveLineLength, 0, 0));
			vec3_t offsetY = rotateVector(vec3_t(0, effectiveLineLength, 0));
			vec3_t offsetZ = rotateVector(vec3_t(0, 0, effectiveLineLength));

			vec3_t center = point.pos;
			vec3_t xPos = center + offsetX;
			vec3_t xNeg = center - offsetX;
			vec3_t yPos = center + offsetY;
			vec3_t yNeg = center - offsetY;
			vec3_t zPos = center + offsetZ;
			vec3_t zNeg = center - offsetZ;

			ImVec2 screenX1, screenX2, screenY1, screenY2, screenZ1, screenZ2;
			bool validX1 = im_render.world_to_screen(xPos, &screenX1);
			bool validX2 = im_render.world_to_screen(xNeg, &screenX2);
			bool validY1 = im_render.world_to_screen(yPos, &screenY1);
			bool validY2 = im_render.world_to_screen(yNeg, &screenY2);
			bool validZ1 = im_render.world_to_screen(zPos, &screenZ1);
			bool validZ2 = im_render.world_to_screen(zNeg, &screenZ2);

			if (validX1 && validX2) {
				im_render.drawline(screenX1.x, screenX1.y, screenX2.x, screenX2.y, crossColor);
			}
			if (validY1 && validY2) {
				im_render.drawline(screenY1.x, screenY1.y, screenY2.x, screenY2.y, crossColor);
			}
			if (validZ1 && validZ2) {
				im_render.drawline(screenZ1.x, screenZ1.y, screenZ2.x, screenZ2.y, crossColor);
			}

			if (isHovered && isBackspacePressed) {
				if (point.open_settings) {
					point.open_settings = false;
					features::movement::m_opened = false;
				}
				features::movement::m_bounce_points_check.erase(features::movement::m_bounce_points_check.begin() + i);
				break;
			}
		}
		bool omaga = false;
		for (const auto& point : points) {
			if (point.map == currentMap && point.open_settings) {
				omaga = true;
				break;
			}
		}
		if (!omaga) {
			features::movement::m_opened = false;
		}
	}
}

void features::movement::RenderPoints()
{
	if (g::local && interfaces::engine->is_in_game() && g::local->is_alive())
	{
		RenderPoints(features::movement::m_pixelsurf_points_check, g::local->origin(), interfaces::engine->get_level_name());
	}
}

void features::movement::assist_createmove(c_usercmd* cmd)
{
	if (g::local && g::local->is_alive()) {
		if (c::assist::assist) {
			//prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			features::movement::pixelsurf_assist(cmd);
			//prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			features::movement::bounce_assist(cmd);

			//prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
			float sv_gravity2 = interfaces::console->get_convar(("sv_gravity"))->get_float();
			float fTickInterval = interfaces::globals->interval_per_tick;
			float fTickRate = (fTickInterval > 0) ? (1.0f / fTickInterval) : 0.0f;
			float veloZoftick = (((sv_gravity2 / 2) / fTickRate) * -1.f);
			static bool disable_pxs = false;
			if (menu::checkkey(c::assist::pixelsurf_assist_key, c::assist::pixelsurf_assist_key_s)) {
				if (alert_mini_crouch_hop) {
					if (disable_pxs) {
						if (prediction_backup::velocity.z == veloZoftick) {
							disable_pxs = false;
							alert_mini_crouch_hop = false;
						}
						if (g::local->flags() & fl_onground) {
							disable_pxs = false;
							alert_mini_crouch_hop = false;
						}
					}
					if (g::local->flags() & fl_onground) {
						cmd->buttons &= ~in_duck;
						cmd->buttons |= in_jump;
						disable_pxs = true;
					}
					else {
						cmd->buttons |= in_duck;
					}
				}

			}
			else {
				alert_mini_crouch_hop = false;
				disable_pxs = false;
			}
			static bool disable_bounce = false;
			if (menu::checkkey(c::assist::bounce_assist_key, c::assist::bounce_assist_key_s)) {
				if (bounce_alert_mini_crouch_hop) {
					if (disable_bounce) {
						if (prediction_backup::velocity.z == veloZoftick) {
							disable_bounce = false;
							bounce_alert_mini_crouch_hop = false;
						}
						if (g::local->flags() & fl_onground) {
							disable_bounce = false;
							bounce_alert_mini_crouch_hop = false;
						}
					}
					if (g::local->flags() & fl_onground) {
						cmd->buttons &= ~in_duck;
						cmd->buttons |= in_jump;
						disable_bounce = true;
					}
					else {
						cmd->buttons |= in_duck;
					}
				}
			}
			else {
				bounce_alert_mini_crouch_hop = false;
				disable_bounce = false;
			}

			if (assistJb::AlertJB || assistJb::AlertJB2)
			{
				cmd->buttons |= in_jump;
			}

			m_assist_t.set_point = false;
		}

		if (!c::assist::assist) {
			features::movement::m_opened = false;
		}

		//...
		if (c::movement::px_selection == 1 && c::movement::pixel_surf) {
			if (m_pixelsurf_data.should_pixel_surf)
				cmd->buttons |= in_duck;
			if (g::local->flags() & fl_onground)
				m_pixelsurf_data.should_pixel_surf = false;
		}
	}
	else {
		features::movement::m_opened = false;
	}
}

void features::movement::assist_endscene()
{
	for (size_t i = 0; i < features::movement::m_bounce_points_check.size(); ++i) {
		features::movement::points_check_t& point = features::movement::m_bounce_points_check[i];
		if (point.open_settings) {
			features::movement::m_opened = true;
			int width, height;
			interfaces::engine->get_screen_size(width, height);
			ImGui::SetNextWindowPos({ static_cast<float>(width) / 2, static_cast<float>(height) / 2 });
			ImGui::Begin("bounce point settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			const auto window = ImGui::GetCurrentWindow();
			constexpr auto background_height = 25.f;
			constexpr auto title_text = "bounce point settings";
			const auto title_text_size = fonts::assist_font->CalcTextSizeA(fonts::assist_font->FontSize, FLT_MAX, 0.f, title_text);
			const auto draw_list = window->DrawList;
			const auto size = window->Size;
			const auto position = window->Pos;

			[&]() {
				ImGui::PushClipRect(ImVec2(position.x, position.y), ImVec2(position.x + size.x, position.y + background_height), false);
				draw_list->AddRectFilled(ImVec2(position.x, position.y), ImVec2(position.x + size.x, position.y + background_height),
					ImColor(25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f), ImGui::GetStyle().WindowRounding,
					ImDrawFlags_RoundCornersTop);
				ImGui::PopClipRect();

				/* render gradient */
				RenderFadedGradientLine(draw_list, ImVec2(position.x, position.y + background_height - 1.f), ImVec2(size.x, 1.f),
					static_cast<ImColor>(ImGui::GetColorU32(ImGuiCol_::ImGuiCol_Border)));

				draw_list->AddText(
					ImVec2(position.x + ((size.x - title_text_size.x) / 2.f), position.y + ((background_height - title_text_size.y) / 2.f)),
					ImColor(1.f, 1.f, 1.f), title_text);

				ImGui::PushClipRect(ImVec2(position.x + 1.f, position.y + 1.f), ImVec2(position.x + size.x - 1.f, position.y + size.y - 1.f),
					false);
				draw_list->AddRect(ImVec2(position.x + 1.f, position.y + 1.f), ImVec2(position.x + size.x - 1.f, position.y + size.y - 1.f),
					ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f), ImGui::GetStyle().WindowRounding);
				ImGui::PopClipRect();
				}();

			ImGui::SetCursorPosY(30.f);

			if (point.active) {
				if (ImGui::Button("make incative", ImVec2(-1.f, 25.f))) {
					point.active = !point.active;
				}
			}
			else {
				if (ImGui::Button("make active", ImVec2(-1.f, 25.f))) {
					point.active = !point.active;
				}
			}

			ImGui::Text(("radius"));
			ImGui::SetNextItemWidth(-1.f);
			ImGui::SliderFloat("##radius2", &point.radius, 50.f, 1000.f, "%.1f");

			static bool delta_strafe_enabled = false;
			ImGui::Checkbox("delta strafe", &delta_strafe_enabled);
			if (delta_strafe_enabled) {
				ImGui::SetNextItemWidth(-1.f);
				ImGui::SliderFloat("##DeltaStrafe", &point.delta_strafe, 0.0f, 5.0f, "%.2f");
			}

			ImGui::Separator();

			ImGui::Checkbox("jump (stand)", &point.jump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("jump (crouch)", &point.c_jump);

			ImGui::Checkbox("minijump (stand)", &point.minijump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("minijump (crouch)", &point.c_minijump);

			ImGui::Checkbox("longjump (stand)", &point.longjump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("longjump (crouch)", &point.c_longjump);

			ImGui::Checkbox("jumpbug (stand)", &point.jumpbug);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("jumpbug (crouch)", &point.c_jumpbug);

			ImGui::Checkbox("crouch hop (stand)", &point.crouch_hop);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("crouch hop (crouch)", &point.c_crouch_hop);

			if (isEnterPressed()) {
				point.open_settings = false;
				features::movement::m_opened = false;
			}
			ImGui::End();
		}
	}
	for (size_t i = 0; i < features::movement::m_pixelsurf_points_check.size(); ++i) {
		features::movement::points_check_t& point = features::movement::m_pixelsurf_points_check[i];
		if (point.open_settings) {
			features::movement::m_opened = true;
			int width, height;
			interfaces::engine->get_screen_size(width, height);
			ImGui::SetNextWindowPos({ static_cast<float>(width) / 2, static_cast<float>(height) / 2 });
			ImGui::Begin("pixelsurf point settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
			const auto window = ImGui::GetCurrentWindow();
			constexpr auto background_height = 25.f;
			constexpr auto title_text = "pixelsurf point settings";
			const auto title_text_size = fonts::assist_font->CalcTextSizeA(
				fonts::assist_font->FontSize, FLT_MAX, 0.f, title_text);
			const auto draw_list = window->DrawList;

			const auto size = window->Size;
			const auto position = window->Pos;

			[&]() {

				ImGui::PushClipRect(ImVec2(position.x, position.y), ImVec2(position.x + size.x, position.y + background_height), false);
				draw_list->AddRectFilled(ImVec2(position.x, position.y), ImVec2(position.x + size.x, position.y + background_height),
					ImColor(25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f), ImGui::GetStyle().WindowRounding,
					ImDrawFlags_RoundCornersTop);
				ImGui::PopClipRect();

				RenderFadedGradientLine(draw_list, ImVec2(position.x, position.y + background_height - 1.f), ImVec2(size.x, 1.f),
					static_cast<ImColor>(ImGui::GetColorU32(ImGuiCol_::ImGuiCol_Border)));

				draw_list->AddText(
					ImVec2(position.x + ((size.x - title_text_size.x) / 2.f), position.y + ((background_height - title_text_size.y) / 2.f)),
					ImColor(1.f, 1.f, 1.f), title_text);

				ImGui::PushClipRect(ImVec2(position.x + 1.f, position.y + 1.f), ImVec2(position.x + size.x - 1.f, position.y + size.y - 1.f),
					false);
				draw_list->AddRect(ImVec2(position.x + 1.f, position.y + 1.f), ImVec2(position.x + size.x - 1.f, position.y + size.y - 1.f),
					ImColor(50 / 255.f, 50 / 255.f, 50 / 255.f), ImGui::GetStyle().WindowRounding);
				ImGui::PopClipRect();
				}();

			ImGui::SetCursorPosY(30.f);

			if (point.active) {
				if (ImGui::Button("make incative", ImVec2(-1.f, 25.f))) {
					point.active = !point.active;
				}
			}
			else {
				if (ImGui::Button("make active", ImVec2(-1.f, 25.f))) {
					point.active = !point.active;
				}
			}
			ImGui::Text(("radius"));
			ImGui::SetNextItemWidth(-1.f);
			ImGui::SliderFloat("##radius1", &point.radius, 50.f, 1000.f, "%.1f");

			static bool delta_strafe_enabled = false;
			ImGui::Checkbox("delta strafe", &delta_strafe_enabled);
			if (delta_strafe_enabled) {
				ImGui::SetNextItemWidth(-1.f);
				ImGui::SliderFloat("##DeltaStrafe", &point.delta_strafe, 0.0f, 5.0f, "%.2f");
			}

			ImGui::Separator();

			ImGui::Checkbox("jump (stand)", &point.jump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("jump (crouch)", &point.c_jump);

			ImGui::Checkbox("minijump (stand)", &point.minijump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("minijump (crouch)", &point.c_minijump);

			ImGui::Checkbox("longjump (stand)", &point.longjump);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("longjump (crouch)", &point.c_longjump);

			ImGui::Checkbox("jumpbug (stand)", &point.jumpbug);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("jumpbug (crouch)", &point.c_jumpbug);

			ImGui::Checkbox("crouch hop (stand)", &point.crouch_hop);
			ImGui::SameLine(130.f);
			ImGui::Checkbox("crouch hop (crouch)", &point.c_crouch_hop);

			ImGui::SetCursorPosX(5.f);
			if (isEnterPressed()) {
				features::movement::m_opened = false;
				point.open_settings = false;
			}
			ImGui::End();
		}
	}

	if (c::assist::assist_render || c::assist::bounce_assist_render) {
		if (GImGui == nullptr)
			return;
		float deltaTime = ImGui::GetIO().DeltaTime;
		myWindowDetect.Update(deltaTime);
		ImDrawList* draw = ImGui::GetBackgroundDrawList();
		myWindowDetect.Draw(draw, ImGui::GetIO().DisplaySize);
	}
}
