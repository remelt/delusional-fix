#include "../prediction/prediction.hpp"
#include "../movement.hpp"

void prediction::begin(c_usercmd* cmd) {
	if (!g::local || !cmd)
		return;

	*g::local->current_command() = cmd;
	g::local->last_command() = *cmd;

	if (!prediction_random_seed)
		prediction_random_seed = *reinterpret_cast<int**>(find_pattern("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);

	*prediction_random_seed = cmd->random_seed & 0x7FFFFFFF;

	origin = g::local->origin();
	velocity = g::local->velocity();

	old_cur_time = interfaces::globals->cur_time;
	old_frame_time = interfaces::globals->frame_time;
	old_tick_count = interfaces::globals->tick_count;

	in_first = interfaces::prediction->is_first_time_predicted;
	in_pred = interfaces::prediction->in_prediction;

	interfaces::globals->cur_time = g::local->get_tick_base() * interfaces::globals->interval_per_tick;
	interfaces::globals->frame_time = interfaces::prediction->engine_paused ? 0.f : tick_interval;

	in_prediction = true;

	interfaces::move_helper->set_host(g::local);
	interfaces::game_movement->start_track_prediction_errors(g::local);

	if (cmd->impulse)
		*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 0x320C) = cmd->impulse;

	interfaces::prediction->check_moving_ground(g::local, interfaces::globals->frame_time);

	interfaces::prediction->set_local_view_angles(cmd->view_angles);

	if (g::local->physics_run_think(0))
		g::local->pre_think();

	static int last_team = -1;
	static bool team_changing = false;
	static int team_change_delay = 0;
	static int stable_frames = 0;
	int current_team = g::local->team();

	const auto next_think_tick = static_cast<int>(*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 0xFC));

	if (g::local->is_alive() && g::local->flags()) {

		//team change fix
		if (last_team != -1 && current_team != last_team) {
			team_changing = true;
			team_change_delay = 0;
			stable_frames = 0;
			last_team = current_team;
			return;
		}

		if (last_team == -1) {
			last_team = current_team;
		}

		if (team_changing) {
			team_change_delay++;

			if (current_team == last_team) {
				stable_frames++;
			}
			else {
				stable_frames = 0;
				last_team = current_team;
			}

			if (stable_frames > 10 && team_change_delay > 5) {
				team_changing = false;
				team_change_delay = 0;
				stable_frames = 0;
			}

			return;
		}

		if (next_think_tick > 0 && next_think_tick <= static_cast<int>(*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 0x3440))) {

			*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 0xFC) = -1;
			(*(void(__thiscall**)(player_t*))(*reinterpret_cast<std::uint32_t*>(uintptr_t(g::local) + 556)))(g::local);
		}
	}

	interfaces::prediction->setup_move(g::local, cmd, interfaces::move_helper, &data);
	interfaces::move_helper->set_host(g::local);
	interfaces::game_movement->process_movement(g::local, &data);
	interfaces::prediction->finish_move(g::local, cmd, &data);
	interfaces::move_helper->process_impacts();

	post_think();

	interfaces::prediction->in_prediction = in_pred;
	interfaces::prediction->is_first_time_predicted = in_first;
}

void prediction::restore_ent_to_predicted_frame(int i) {
	using restore_entity_to_predicted_frame_fn = void(__stdcall*)(int, int);
	static auto fn = reinterpret_cast<restore_entity_to_predicted_frame_fn>(find_pattern("client.dll", "55 8B EC 8B 4D ? 56 E8 ? ? ? ? 8B 75"));
	fn(0, i);
}

void prediction::repredict() {
	restore_ent_to_predicted_frame(interfaces::prediction->predicted_commands() - 1);
	interfaces::prediction->previous_start_frame() = -1;
	interfaces::prediction->predicted_commands() = 0;
}

void prediction::post_think() {
	interfaces::mdl_cache->begin_lock();

	if (g::local->is_alive()) {
		g::local->update_collision_bounds();

		if (g::local->flags() & fl_onground)
			g::local->fall_velocity() = 0.f;

		g::local->studio_frame_advance();

		g::local->post_think_v_physics();
	}
	g::local->simulate_player_simulated_entities();

	interfaces::mdl_cache->end_lock();
}

void prediction::end() {
	if (!g::local) 
		return;

	interfaces::game_movement->finish_track_prediction_errors(g::local);
	interfaces::prediction->in_prediction = false;

	interfaces::move_helper->set_host(nullptr);

	in_prediction = false;

	interfaces::globals->cur_time = old_cur_time;
	interfaces::globals->frame_time = old_frame_time;

	*g::local->current_command() = NULL;

	*prediction_random_seed = -1;

	interfaces::game_movement->reset();

	if (!interfaces::prediction->engine_paused && interfaces::globals->frame_time > 0.f) {
		++g::local->get_tick_base();
	}
}
