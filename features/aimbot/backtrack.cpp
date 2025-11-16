#include "aimbot.hpp"
#include "../../sdk/math/math.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/prediction/prediction.hpp"

float calc_fov(vec3_t viewAngle, vec3_t aimAngle) {
	vec3_t delta = aimAngle - viewAngle;
	math::fix_angles(delta);

	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

void backtrack_c::setup_records() {
	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
		auto player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (player->is_not_target()) {
			if (data.count(i) > 0)
				data.erase(i);
			continue;
		}

		if (!player->is_enemy()) {
			if (data.count(i) > 0)
				data.erase(i);
			continue;
		}

		auto& cur_data = data[i];
		if (!cur_data.empty()) {
			auto& front = cur_data.front();
			if (front.sim_time == player->simulation_time())
				continue;

			while (!cur_data.empty()) {
				auto& back = cur_data.back();
				float delta_time = std::clamp(correct_time, 0.f, cvar::sv_maxunlag) - (interfaces::globals->cur_time - back.sim_time);
				if (std::fabsf(delta_time) <= (200 / 1000.f))
					break;

				cur_data.pop_back();
			}
		}

		auto model = player->model();
		if (!model)
			continue;

		auto hdr = interfaces::model_info->get_studio_model(model);
		if (!hdr)
			continue;

		backtrack_data bd;

		bd.head = player->get_hitbox_position(hitbox_head);
		bd.chest = player->get_hitbox_position(hitbox_chest);
		bd.pelvis = player->get_hitbox_position(hitbox_pelvis);
		bd.neck = player->get_hitbox_position(hitbox_neck);
		bd.lchest = player->get_hitbox_position(hitbox_lower_chest);

		auto hitbox_set = hdr->hitbox_set(player->hitbox_set());
		auto hitbox_headd = hitbox_set->hitbox(hitbox_head);
		auto hitbox_center = (hitbox_headd->mins + hitbox_headd->maxs) * 0.5f;

		bd.hitboxset = hitbox_set;
		bd.sim_time = player->simulation_time();
		bd.player = player;

		*(vec3_t*)((uintptr_t)player + 0xA0) = player->origin();
		*(int*)((uintptr_t)player + 0xA68) = 0;
		*(int*)((uintptr_t)player + 0xA30) = 0;

		player->inv_bone_cache();
		player->setup_bones(bd.m_matrix, 128, 256, interfaces::globals->cur_time);

		math::vector_transform(hitbox_center, bd.m_matrix[hitbox_headd->bone], bd.m_headpos);
		data[i].push_front(bd);
	}
}

void backtrack_c::on_move(c_usercmd* cmd) {
	if (!interfaces::engine->is_in_game() || !g::local || !g::local->is_alive()) {
		data.clear();
		return;
	}

	auto weapon = g::local->active_weapon();
	if (!weapon) {
		data.clear();
		return;
	}

	if (!c::backtrack::backtrack) {
		data.clear();
		return;
	}

	if (!(cmd->buttons & in_attack))
		return;

	lerp_time = std::fmaxf(cvar::cl_interp, cvar::cl_interp_ratio / cvar::updateRate);
	latency = interfaces::engine->get_net_channel_info()->get_latency(FLOW_INCOMING) + interfaces::engine->get_net_channel_info()->get_latency(FLOW_OUTGOING);
	correct_time = latency + lerp_time;

	vec3_t l_eye_pos = g::local->get_eye_pos();
	vec3_t angles;

	int tick_count = -1;
	float best_fov = 255.0f;
	auto view = cmd->view_angles;

	for (auto& node : data) {
		auto& cur_data = node.second;

		if (cur_data.empty())
			continue;

		for (int i = 0; i < cur_data.size(); i++) {
			auto bd = cur_data[i];
			float delta_time = std::clamp(correct_time, 0.f, cvar::sv_maxunlag) - (interfaces::globals->cur_time - bd.sim_time);

			if (std::fabsf(delta_time) > 0.2f)
				continue;

			math::vector_angles(bd.m_headpos - l_eye_pos, angles);
			math::fix_angles(angles);

			float fov = calc_fov(cmd->view_angles, angles);
			if (best_fov > fov) {
				best_fov = fov;
				tick_count = time_to_ticks(bd.sim_time + lerp_time);
				selected_tick = i;
				sel_ent_index = bd.player->index();
			}
		}
	}
	if (tick_count != -1) {
		cmd->tick_count = tick_count;
	}
}

backtrack_c backtrack;

void fake_latency::update_sequence(i_net_channel* net_channel) {
	if (net_channel == nullptr)
		return;

	if (m_last_incoming_sequence_number == 0)
		m_last_incoming_sequence_number = net_channel->in_sequence_nr;

	if (net_channel->in_sequence_nr > m_last_incoming_sequence_number) {
		m_last_incoming_sequence_number = net_channel->in_sequence_nr;
		sequences.emplace_front(incoming_sequence_t(net_channel->in_reliable_state, net_channel->out_reliable_state, net_channel->in_sequence_nr, interfaces::globals->realtime));
	}

	if (sequences.size() > 2048U)
		sequences.pop_back();
}

void fake_latency::clear_sequence() {
	if (!sequences.empty()) {
		m_last_incoming_sequence_number = 0;
		sequences.clear();
	}
}

void fake_latency::add_latency(i_net_channel* net_channel, float latency) {
	for (auto& seq : sequences) {
		if (interfaces::globals->realtime - seq.m_current_time >= latency) {
			net_channel->in_reliable_state = seq.m_in_reliable_state;
			net_channel->in_sequence_nr = seq.m_sequence_nr;
			break;
		}
	}
}

fake_latency fakeping;
