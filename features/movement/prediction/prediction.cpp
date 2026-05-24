#include "../prediction/prediction.hpp"
#include "../movement.hpp"

entity_t** m_prediction_player = nullptr;

void prediction::update()
{
	auto nci = interfaces::engine->get_net_channel_info();
	if (nci) {
		g::max_allocation = time_to_ticks(interfaces::console->get_convar("sv_maxunlag")->get_float());


		if (c::backtrack::backtrack)
			g::max_allocation += time_to_ticks(nci->get_latency(FLOW_INCOMING) + nci->get_latency(FLOW_OUTGOING));
	}
	else
		g::max_allocation = 0;

	prediction::backup_originals(g::cmd);

	if (const bool valid = interfaces::client_state->delta_tick > 0; valid)
		interfaces::prediction->update(interfaces::client_state->delta_tick, valid, interfaces::client_state->last_command_ack,
			interfaces::client_state->last_outgoing_command + interfaces::client_state->choked_commands);
}


void prediction::begin(c_usercmd* cmd) {
	if (!interfaces::move_helper)
		return;

	*g::local->current_command() = cmd;
	g::local->last_command() = *cmd;

	if (static bool once = false; !once) {

		prediction_random_seed = *reinterpret_cast<unsigned int**>(find_pattern("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2);

		m_prediction_player = *reinterpret_cast<entity_t***>(find_pattern("client.dll", "89 35 ? ? ? ? F3 0F 10 48 20") + 0x2);
		once = true;

	}

	*prediction_random_seed = cmd->random_seed;
	*m_prediction_player = g::local;

	old_cur_time = interfaces::globals->cur_time;
	old_frame_time = interfaces::globals->frame_time;
	old_tick_count = interfaces::globals->tick_count;

	const bool old_is_first_prediction = interfaces::prediction->is_first_time_predicted;
	const bool old_in_prediction = interfaces::prediction->in_prediction;

	interfaces::globals->cur_time = static_cast<float>(g::local->get_tick_base()) * tick_interval;

	interfaces::globals->frame_time =
		interfaces::prediction->engine_paused ? 0.f : tick_interval;

	interfaces::globals->tick_count = g::local->get_tick_base();

	interfaces::prediction->is_first_time_predicted = false;
	interfaces::prediction->in_prediction = true;

	in_prediction = true;

	cmd->buttons |= g::local->get_button_forced();
	cmd->buttons &= ~(g::local->get_button_disabled());

	interfaces::game_movement->start_track_prediction_errors(g::local);

	prediction::update_button_state(cmd);

	interfaces::prediction->check_moving_ground(g::local, interfaces::globals->frame_time);

	interfaces::prediction->set_local_view_angles(cmd->view_angles);

	if (g::local->physics_run_think(0))
		g::local->pre_think();

	if (int* next_think_tick = g::local->next_think_tick(); *next_think_tick > 0 && *next_think_tick <= get_corrected_tick_base(cmd)) {
		*next_think_tick = -1;

		g::local->set_next_think(0);

		g::local->think();
	}

	interfaces::move_helper->set_host(g::local);

	interfaces::prediction->setup_move(g::local, cmd, interfaces::move_helper, &data);
	interfaces::game_movement->process_movement(g::local, &data);
	interfaces::prediction->finish_move(g::local, cmd, &data);

	interfaces::move_helper->process_impacts();

	post_think();

	interfaces::prediction->in_prediction = old_in_prediction;
	interfaces::prediction->is_first_time_predicted = old_is_first_prediction;
}

void prediction::restore_ent_to_predicted_frame(int i) {
	using restore_entity_to_predicted_frame_fn = void(__stdcall*)(int, int);
	static auto fn = reinterpret_cast<restore_entity_to_predicted_frame_fn>(find_pattern("client.dll", "55 8B EC 8B 4D ? 56 E8 ? ? ? ? 8B 75"));
	fn(0, i);
}

void prediction::repredict() {
	//restore_ent_to_predicted_frame(interfaces::prediction->predicted_commands() - 1);
	//interfaces::prediction->previous_start_frame() = -1;
	//interfaces::prediction->predicted_commands() = 0;
}

void prediction::post_think() {
	//static auto post_think_vphysics = reinterpret_cast<bool(__thiscall*)(player_t*)>(
	//	find_pattern("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));

	//static auto simulate_player_simulated_entities = reinterpret_cast<void(__thiscall*)(player_t*)>(
	//	find_pattern("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74"));

	interfaces::mdl_cache->begin_lock();

	if (g::local->is_alive()) {
		g::local->update_collision_bounds();

		if (g::local->flags() & fl_onground)
			*g::local->fall_velocity() = 0.f;

		if (g::local->get_sequence() == -1)
			g::local->set_sequence(0);

		g::local->studio_frame_advance();
		g::local->post_think_v_physics();
	}
	g::local->simulate_player_simulated_entities();

	interfaces::mdl_cache->end_lock();
}

void prediction::end() {
	if (!interfaces::move_helper)
		return;

	interfaces::game_movement->finish_track_prediction_errors(g::local);
	interfaces::move_helper->set_host(nullptr);

	in_prediction = false;

	interfaces::globals->cur_time = old_cur_time;
	interfaces::globals->frame_time = old_frame_time;
	interfaces::globals->tick_count = old_tick_count;

	*g::local->current_command() = NULL;

	*prediction_random_seed = -1;
	*m_prediction_player = nullptr;

	interfaces::game_movement->reset();
}

int prediction::get_corrected_tick_base(c_usercmd* cmd)
{
	static int corrected_tick_base = 0;
	static c_usercmd* last_cmd = nullptr;

	if (cmd) {
		if (!last_cmd || last_cmd->has_been_predicted)
			corrected_tick_base = g::local->get_tick_base();
		else
			corrected_tick_base++;

		last_cmd = cmd;
	}

	return corrected_tick_base;
}

void prediction::update_button_state(c_usercmd* cmd)
{
	const int buttons = cmd->buttons;
	const int local_buttons = *g::local->buttons();
	const int buttons_changed = buttons ^ local_buttons;

	g::local->button_last() = local_buttons;
	*g::local->buttons() = buttons;
	g::local->button_pressed() = buttons_changed & buttons;
	g::local->button_released() = buttons_changed & (~buttons);
};