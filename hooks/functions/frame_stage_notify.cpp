#include "../hooks.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/skins/skins.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/movement/prediction/prediction.hpp"
#include "../../features/movement/lobotomy_eb.h"

void __stdcall sdk::hooks::frame_stage_notify::frame_stage_notify(int stage) {

	lobotomy_eb::frame_stage(stage);
	recorder->frame_stage(stage);

	if (stage == frame_render_start) {
		features::misc::force_crosshair();
		features::visuals::flashalpha();
		features::visuals::skybox_changer();
		features::visuals::nosmoke();
		features::weather::update_weather();
		features::visuals::apply_zoom();
		backtrack.setup_records();
		features::visuals::console();
		features::visuals::shadows();
	}
	else if (stage == frame_render_end) {
		features::visuals::fog();
	}
	else if (stage == frame_net_update_postdataupdate_start) {
		features::skins::agent_changer();
		features::skins::knife_changer();
		features::skins::gloves_changer();
		features::skins::full_update();
	}
	else if (stage == frame_net_update_end) {

	}
	else if (stage == frame_start) {
		if (features::movement::detect_data.ticks_left && features::movement::detect_data.strafing) {
			vec3_t edgebugva = vec3_t{ features::movement::first_viewangles.x, features::movement::detect_data.startingyaw , features::movement::first_viewangles.z };

			float to_eb_time = ticks_to_time(features::movement::detect_data.edgebugtick) - ticks_to_time(features::movement::detect_data.detecttick);
			float from_detect_time = interfaces::globals->cur_time - ticks_to_time(features::movement::detect_data.detecttick);

			float addedyaw = math::normalize_yaw(features::movement::detect_data.yawdelta * (features::movement::detect_data.eblength * (from_detect_time / to_eb_time)));
			edgebugva.y += addedyaw;

			//CHILLWARE EXPLOIT
			//SOLD FOR 10k
			if (!c::movement::silent_eb_hacked) {
				interfaces::engine->set_view_angles(edgebugva);
			}
		}
	}

	if (!interfaces::engine->is_in_game()) {
		fakeping.clear_sequence();
		return ofunc(interfaces::client, stage);
	}

	ofunc(interfaces::client, stage);
}