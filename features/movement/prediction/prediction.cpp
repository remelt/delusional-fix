#include "../prediction/prediction.hpp"
#include "../movement.hpp"

//s/o https://github.com/clearlyst/bhop-cheat/commit/fde106fd39897615a38c14ecbc73294d5c8f690c#diff-61a95352f3c16c2b0e0f8ca73b99cdd82d83885d06d222715808f7fce3d2477e
void prediction::updatepacket()
{
	if (!g::local)
	{
		return;
	}

	if (interfaces::client_state->delta_tick > 0)
	{
		interfaces::prediction->update(
			interfaces::client_state->delta_tick,
			interfaces::client_state->delta_tick > 0,
			interfaces::client_state->last_command_ack,
			interfaces::client_state->last_outgoing_command + interfaces::client_state->choked_commands);
	}
}

void prediction::begin(c_usercmd* cmd) {
	if (!g::local || !cmd)
		return;

	*g::local->current_command() = cmd;
	g::local->last_command() = *cmd;

	if (!prediction_player_real)
	{
		prediction_player_real = *reinterpret_cast<player_t***>(find_pattern("client.dll", "89 35 ? ? ? ? F3 0F 10 48") + 0x2);
	}

	if (!prediction_random_seed)
	{
		prediction_random_seed = *reinterpret_cast<int**>(find_pattern("client.dll", "A3 ? ? ? ? 66 0F 6E 86") + 0x1);
	}

	if (!md5_pseudo_random)
	{
		md5_pseudo_random = reinterpret_cast<md5_pseudo_random_fn>(find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 70 6A"));
	}

	*prediction_random_seed = md5_pseudo_random(cmd->command_number) & 0x7FFFFFFF;

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

	// fixed + hacked
	if (g::local->next_think_tick() != -1 && g::local->next_think_tick() > 0 && g::local->next_think_tick() <= g::local->get_tick_base())
	{
		g::local->next_think_tick() = -1;
		g::local->think();
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
	interfaces::move_helper->set_host(nullptr);
	interfaces::prediction->in_prediction = false;

	in_prediction = false;

	interfaces::globals->cur_time = old_cur_time;
	interfaces::globals->frame_time = old_frame_time;
	interfaces::globals->tick_count = old_tick_count;

	*g::local->current_command() = NULL;

	*prediction_random_seed = -1;
	*prediction_player_real = nullptr;
	*prediction_random_seed = -1;

	interfaces::game_movement->reset();

	*reinterpret_cast<c_usercmd**>(uint32_t(g::local) + 0x3348) = NULL;

	if (!interfaces::prediction->engine_paused && interfaces::globals->frame_time > 0.f) {
		++g::local->get_tick_base();
	}
}
