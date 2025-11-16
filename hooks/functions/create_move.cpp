#include "../../features/movement/prediction/prediction.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/panorama/scaleform.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/math/math.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"
#include "../../features/movement/lobotomy_eb.h"

bool __fastcall sdk::hooks::create_move::create_move(registers, float sampletime, c_usercmd* cmd ) {
	bool ret = sdk::hooks::create_move::ofunc(ecx, edx, sampletime, cmd);

	if (!cmd || !cmd->command_number)
		return ret;

	g::local = static_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	g::cmd = cmd;
	features::movement::first_viewangles = cmd->view_angles;
	features::movement::previous_tick = interfaces::globals->tick_count;
	i_net_channel* net_channel = interfaces::client_state->net_channel;

	features::misc::fix_mouse_delta(cmd);
	features::visuals::jump_trail();
	features::misc::clantag_spammer();
	features::misc::reveal_server_ranks(cmd);
	panorama::scaleform_tick(g::local);

	features::movement::auto_align(cmd);
	if (c::movement::px_selection == 0) {
		features::movement::pixel_surf_fix_del(cmd);
	}
	features::movement::bhop(cmd);

	features::movement::delay_hop(cmd);
	features::movement::gather_vel_graph_data();
	features::movement::gather_stam_graph_data();

	if (c::movement::fastduck) cmd->buttons |= in_bullrush;

	features::grenade_prediction::run(cmd);

	prediction::backup_originals(cmd);

	lobotomy_eb::PrePredictionEdgeBug(cmd);

	//dont even call this shit if not enabled
	if (c::assist::assist) {
		features::movement::check_ps(cmd);
	}

	features::movement::assist_createmove(cmd);
	if ((c::movement::auto_align) && (c::movement::align_selection == 1) && !(prediction_backup::flags & 1)) {
		features::movement::auto_align_lb(cmd);
	}
	features::movement::fast_ladder(cmd);
	features::movement::air_stuck(cmd);
	features::movement::jump_bug_crouch(cmd);

	prediction::begin(cmd); {

		features::movement::null_strafing(cmd);
		g_Aimbot.run(cmd);
		lobotomy_eb::edgebug_detect(cmd);
		triggerbot::run(cmd);
		backtrack.on_move(cmd);

	}prediction::end();

	features::visuals::run_freecam(cmd, features::movement::first_viewangles);
	features::movement::crouch_bug(cmd);
	features::movement::edge_jump(cmd);
	features::movement::long_jump(cmd);
	features::movement::mini_jump(cmd);
	features::misc::jumpstats::jumpstats(cmd);
	features::movement::ladder_jump(cmd);
	features::movement::ladder_bug(cmd);
	features::movement::jump_bug(cmd);

	features::movement::auto_strafe(cmd, features::movement::first_viewangles);
	features::movement::strafe_optimizer(cmd);

	features::misc::kz_practice_logic(cmd);

	//lb pixelsurf
	if (c::movement::px_selection == 1) {
		features::movement::on_create_move_post(cmd);
	}

	features::movement::fire_man(cmd);
	features::movement::auto_duck(cmd);
	features::movement::avoid_collision(cmd);

	if (!c::movement::movement_fix && !menu::checkkey(c::movement::air_stuck_key, c::movement::air_stuck_key_s)) {
		features::movement::fix_movement(cmd, features::movement::first_viewangles);
	}

	if (c::movement::px_selection == 0) {
		features::movement::pixel_surf_del(cmd);
		features::movement::pixel_surf_lock(cmd);
	}

	features::movement::edge_bug(cmd);

	lobotomy_eb::EdgeBugPostPredict(cmd);

	if (interfaces::prediction->split->commands_predicted > 1)
				prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

	if (net_channel != nullptr) {
		if (c::backtrack::fake_latency)
			fakeping.update_sequence(net_channel);
		else
			fakeping.clear_sequence();
	}

	if (c::misc::anti_untrusted) {
		cmd->view_angles.x = std::clamp(cmd->view_angles.x, -90.f + FLT_EPSILON, 90.f - FLT_EPSILON);
		cmd->view_angles.y = math::normalize_yaw(cmd->view_angles.y);
		cmd->view_angles.z = 0.f;
	}

	recorder->create_move(cmd);

	features::movement::last_tick_y = cmd->view_angles.y;

	return false;
}