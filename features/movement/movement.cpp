#define NOMINMAX
#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"
#include "lobotomy_eb.h"

void features::movement::bhop(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (!c::movement::bhop) {
		return;
	}
	if (c::movement::bhopmiss) {
		static int perfect_hops = 0;
		static bool force_miss_next = false;
		static int ground_ticks = 0;
		static bool was_on_ground = false;


		const int MAX_PERFECT_HOPS = c::movement::whathopmiss;
		const int PERFECT_HOP_THRESHOLD = 2;

		bool on_ground = g::local->flags() & fl_onground;

		if (force_miss_next && on_ground) {
			cmd->buttons &= ~in_jump;
			force_miss_next = false;
			perfect_hops = 0;

			return;
		}
		if (on_ground) {
			if (!was_on_ground) {
				ground_ticks = 1;
			}
			else {
				ground_ticks++;
			}
			if (ground_ticks <= PERFECT_HOP_THRESHOLD && ground_ticks > 0) {
				perfect_hops++;

				if (perfect_hops >= MAX_PERFECT_HOPS) {
					force_miss_next = true;
				}
			}
			else if (ground_ticks > 5) {
				perfect_hops = 0;
			}

		}
		else {
			if (!GetAsyncKeyState(c::movement::jump_bug_key) &&
				g::local->move_type() != movetype_ladder) {

				if (cmd->buttons & in_jump) {
					cmd->buttons &= ~in_jump;
				}
			}
		}

		was_on_ground = on_ground;
	}
	else
	{
		if (!GetAsyncKeyState(c::movement::jump_bug_key) && g::local->move_type() != movetype_ladder)
			if (!(g::local->flags() & fl_onground) && cmd->buttons & (in_jump)) {
				cmd->buttons &= ~(in_jump);
			}
	}
}


void features::movement::delay_hop(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::delay_hop && menu::checkkey(c::movement::delay_hop_key, c::movement::delay_hop_key_s)) {

		static int ticks = 0;
		const int ticks_to_wait = c::movement::dh_tick;

		const int move_type = g::local->move_type();
		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
			return;

		if (g::local->flags() & fl_onground) {
			if (cmd->tick_count > ticks + ticks_to_wait) {
				cmd->buttons |= in_jump;
				ticks = cmd->tick_count;
			}
		}
		else {
			ticks = cmd->tick_count;
		}
	}
}

void features::movement::crouch_bug(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::crouch_bug && menu::checkkey(c::movement::crouch_bug_key, c::movement::crouch_bug_key_s)) {
		if (!(prediction_backup::flags & (fl_onground)) && g::local->flags() & (fl_onground)) {
			cmd->buttons |= (in_duck);
		}
	}
}

void features::movement::edge_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	const int move_type = g::local->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
		return;
	}

	if (c::movement::edge_jump && menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s)) {
		if (prediction_backup::flags & (fl_onground) && !(g::local->flags() & fl_onground)) {
			if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[0]) {
				if (c::movement::lj_null[0]) {
					cmd->buttons &= ~in_forward;
					interfaces::engine->execute_cmd(xs("-forward"));
				}
				if (c::movement::lj_null[1]) {
					cmd->buttons &= ~in_back;
					interfaces::engine->execute_cmd(xs("-back"));
				}
				if (c::movement::lj_null[2]) {
					cmd->buttons &= ~in_moveleft;
					interfaces::engine->execute_cmd(xs("-moveleft"));
				}
				if (c::movement::lj_null[3]) {
					cmd->buttons &= ~in_moveright;
					interfaces::engine->execute_cmd(xs("-moveright"));

				}
			}
			should_ej = true;
			cmd->buttons |= in_jump;
		}
		else {
			should_ej = false;
		}
	}
}

static int saved_tick_count;
void features::movement::long_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::long_jump_on_edge) {
		const int move_type = g::local->move_type();
		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
			return;
		}
		if (g::local->flags() & fl_onground) {
			saved_tick_count = interfaces::globals->tick_count;
		}
		if (interfaces::globals->tick_count - saved_tick_count > 2) {
			do_long_jump = false;
		}
		else {
			do_long_jump = true;
		}
		if (menu::checkkey(c::movement::long_jump_key, c::movement::long_jump_key_s)) {
			if (do_long_jump && !(g::local->flags() & fl_onground)) {
				if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[3]) {
					if (c::movement::lj_null[0]) {
						cmd->buttons &= ~in_forward;
						interfaces::engine->execute_cmd(xs("-forward"));
					}
					if (c::movement::lj_null[1]) {
						cmd->buttons &= ~in_back;
						interfaces::engine->execute_cmd(xs("-back"));
					}
					if (c::movement::lj_null[2]) {
						cmd->buttons &= ~in_moveleft;
						interfaces::engine->execute_cmd(xs("-moveleft"));
					}
					if (c::movement::lj_null[3]) {
						cmd->buttons &= ~in_moveright;
						interfaces::engine->execute_cmd(xs("-moveright"));
					}
				}
				cmd->buttons |= in_duck;
				should_lj = true;
			}
			else {
				should_lj = false;
			}
		}
	}
}

static int saved_tick_count_mj;
void features::movement::mini_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	const int move_type = g::local->move_type();
	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer) {
		return;
	}

	if (c::movement::mini_jump && menu::checkkey(c::movement::mini_jump_key, c::movement::mini_jump_key_s)) {
		if (prediction_backup::flags & (fl_onground) && !(g::local->flags() & fl_onground)) {
			if (c::movement::adaptive_key_cancelling && c::movement::adaptive_key_for[1]) {
				cmd->buttons &= ~in_forward;
				interfaces::engine->execute_cmd(xs("-forward"));
			}
			if (c::movement::lj_null[1]) {
				cmd->buttons &= ~in_back;
				interfaces::engine->execute_cmd(xs("-back"));
			}
			if (c::movement::lj_null[2]) {
				cmd->buttons &= ~in_moveleft;
				interfaces::engine->execute_cmd(xs("-moveleft"));
			}
			if (c::movement::lj_null[3]) {
				cmd->buttons &= ~in_moveright;
				interfaces::engine->execute_cmd(xs("-moveright"));
			}
			should_mj = true;

			if (c::movement::mini_jump_ej)
				cmd->buttons |= in_jump;

			cmd->buttons |= in_duck;
		}
		else {
			should_mj = false;
		}
	}
}

void features::movement::ladder_bug(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::ladder_bug) {

		if (!menu::checkkey(c::movement::ladder_bug_key, c::movement::ladder_bug_key_s))
			return;

		if (g::local->move_type() & movetype_ladder) {
			cmd->up_move = 450.f;
			cmd->buttons |= in_jump;
			should_lb = true;
		}
		else {
			should_lb = false;
		}
	}
}

void features::movement::ladder_jump(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	static int saved_tick;

	if (c::movement::edge_jump && menu::checkkey(c::movement::edge_jump_key, c::movement::edge_jump_key_s)) {
		if (c::movement::edge_jump_on_ladder) {
			const int pre = g::local->move_type();

			prediction::begin(cmd);
			prediction::end();

			const int post = g::local->move_type();
			if (pre == movetype_ladder) {
				if (post != movetype_ladder) {
					saved_tick = interfaces::globals->tick_count;
					cmd->buttons |= in_jump;
					cmd->forward_move = 0.f;
					cmd->side_move = 0.f;
					cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
				}
			}

			if (interfaces::globals->tick_count - saved_tick > 1 && interfaces::globals->tick_count - saved_tick < 15) {
				cmd->forward_move = 0.f;
				cmd->side_move = 0.f;
				cmd->buttons = cmd->buttons & ~(in_forward | in_back | in_moveright | in_moveleft);
				cmd->buttons |= in_duck;
			}
		}
	}
}

void features::movement::null_strafing(c_usercmd* cmd) {
	if (!g::local || g::local->flags() & fl_onground) {
		return;
	}

	if (c::movement::null_strafing) {
		if (cmd->mouse_dx > 0 && cmd->buttons & in_moveright && cmd->buttons & in_moveleft)
			cmd->side_move = -450.f;
		else if (cmd->mouse_dx < 0 && cmd->buttons & in_moveleft && cmd->buttons & in_moveright)
			cmd->side_move = 450.f;
	}
}

void features::movement::blockbot(c_usercmd* cmd) {

}

void features::movement::auto_strafe(c_usercmd* cmd, vec3_t& current_angle) {
	if (!c::movement::auto_strafe || !menu::checkkey(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s))
		return;

	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip)
		return;

	if (g::local->flags() & fl_onground)
		return;

	if (g::local->get_velocity().length_2d() <= 5.f)
		return;

	static float angle = 0.f;

	const bool back = cmd->buttons & in_back;
	const bool forward = cmd->buttons & in_forward;
	const bool right = cmd->buttons & in_moveright;
	const bool left = cmd->buttons & in_moveleft;

	if (back) {
		angle = -180.f;
		if (left)
			angle -= 45.f;
		else if (right)
			angle += 45.f;
	}
	else if (left) {
		angle = 90.f;
		if (back)
			angle += 45.f;
		else if (forward)
			angle -= 45.f;
	}
	else if (right) {
		angle = -90.f;
		if (back)
			angle -= 45.f;
		else if (forward)
			angle += 45.f;
	}
	else {
		angle = 0.f;
	}

	current_angle.y += angle;

	cmd->forward_move = 0.f;
	cmd->side_move = 0.f;

	auto velocity = g::local->get_velocity();
	const auto delta = math::normalize_yaw(current_angle.y - math::rad2deg(std::atan2(velocity.y, velocity.x)));

	cmd->side_move = delta > 0.f ? -450.f : 450.f;

	current_angle.y = math::normalize_yaw(current_angle.y - delta);
}

void features::movement::fix_movement(c_usercmd* cmd, vec3_t& angle) {
	if (!g::local)
		return;

	vec3_t move, dir;
	vec3_t move_angle;
	float delta, len;

	if (!(g::local->flags() & fl_onground) && cmd->view_angles.z != 0 && cmd->buttons & in_attack)
		cmd->side_move = 0;

	move = { cmd->forward_move, cmd->side_move, 0 };

	len = move.normalize_movement();

	if (!len)
		return;

	math::vectorangles(move, move_angle);

	delta = (cmd->view_angles.y - angle.y);

	move_angle.y += delta;

	math::angle_vectors(move_angle, &dir);

	dir *= len;

	if (g::local->move_type() == movetype_ladder) {
		if (cmd->view_angles.x >= 45 && angle.x < 45 && std::abs(delta) <= 65)
			dir.x = -dir.x;

		cmd->forward_move = dir.x;
		cmd->side_move = dir.y;

		if (cmd->forward_move > 200)
			cmd->buttons |= in_forward;

		else if (cmd->forward_move < -200)
			cmd->buttons |= in_back;

		if (cmd->side_move > 200)
			cmd->buttons |= in_moveright;

		else if (cmd->side_move < -200)
			cmd->buttons |= in_moveleft;
	}
	else {
		if (cmd->view_angles.x < -90 || cmd->view_angles.x > 90)
			dir.x = -dir.x;

		cmd->forward_move = dir.x;
		cmd->side_move = dir.y;
	}

	cmd->forward_move = std::clamp(cmd->forward_move, -450.f, 450.f);
	cmd->side_move = std::clamp(cmd->side_move, -450.f, 450.f);
	cmd->up_move = std::clamp(cmd->up_move, -320.f, 320.f);

	if (g::local->move_type() != movetype_ladder)
		cmd->buttons &= ~(in_forward | in_back | in_moveright | in_moveleft);
}

void features::movement::strafe_optimizer(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::strafe_optimizer) {
		if (menu::checkkey(c::misc::freecam_key, c::misc::freecam_key_s))
			return;

		const int move_type = g::local->move_type();

		if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
			return;

		static auto m_yaw = interfaces::console->get_convar("m_yaw");
		static auto sensitivity = interfaces::console->get_convar("sensitivity");
		static float previous_view_angles_yaw = cmd->view_angles.y;

		if (cmd->forward_move == 0 && !(g::local->flags() & fl_onground)) if (auto velocity = g::local->velocity(); velocity.length_2d() > c::movement::strafe_optimizer_required_speed) {
			interfaces::prediction->update(interfaces::client_state->delta_tick,
				interfaces::client_state->delta_tick > 0, interfaces::client_state->last_command_ack,
				interfaces::client_state->last_outgoing_command + interfaces::client_state->choked_commands);

			float mouse_yaw_factor = m_yaw->get_float();
			float mouse_sensitivity = sensitivity->get_float();
			float mouse_yaw_step = mouse_sensitivity * mouse_yaw_factor;

			if (cmd->side_move < 0) {
				if (previous_view_angles_yaw - cmd->view_angles[1] < 0) {
					float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_desired_gain / 100;
					if (strafe_angle < -mouse_yaw_step) {
						if (strafe_angle < -180) {
							strafe_angle = -180;
						}
						cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
						cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));
						interfaces::engine->set_view_angles(cmd->view_angles);
					}
				}
			}
			else {
				if (cmd->side_move > 0) {
					if (previous_view_angles_yaw - cmd->view_angles[1] > 0) {
						float strafe_angle = remainderf(cmd->view_angles[1] - atan2f(velocity[1], velocity[0]) * 180 / m_pi, 360) * c::movement::strafe_optimizer_desired_gain / 100;
						if (strafe_angle > mouse_yaw_step) {
							if (strafe_angle > 180) {
								strafe_angle = 180;
							}
							cmd->view_angles[1] = remainderf(cmd->view_angles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
							cmd->mouse_dx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->view_angles[1], 360) / sqrtf(mouse_yaw_step)));
							interfaces::engine->set_view_angles(cmd->view_angles);

						}
					}
				}
			}
		}
		previous_view_angles_yaw = cmd->view_angles[1];
	}
}

void features::movement::mouse_strafe_limiter(float* x, float* y) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::mouse_strafe_limiter) {
		if (menu::checkkey(c::movement::mouse_strafe_limiter_key, c::movement::mouse_strafe_limiter_key_s))
			return;

		const int move_type = g::local->move_type();
		if (move_type == movetype_ladder || move_type == movetype_noclip)
			return;

		if (!(g::local->flags() & fl_onground))
			return;

		const auto config_value = c::movement::mouse_strafe_limiter_value;
		*x = std::clamp<float>(*x, -config_value, config_value);
		if (c::movement::mouse_strafe_limiter_affect_pitch)
			*y = std::clamp<float>(*y, -config_value, config_value);
	}
}

void features::movement::jump_bug(c_usercmd* cmd) {
	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (c::movement::jump_bug ) {

		if (!menu::checkkey(c::movement::jump_bug_key, c::movement::jump_bug_key_s))
			return;

		if (!(prediction_backup::flags & (fl_onground)) && g::local->flags() & (fl_onground)) {
			cmd->buttons |= in_duck;
		}

		if (g::local->flags() & fl_onground) {
			cmd->buttons &= ~in_jump;
		}

		detected_normal_jump_bug = (c::movement::jump_bug
			&& !should_edge_bug
			&& g::local->velocity().z > prediction_backup::velocity.z
			&& !(prediction_backup::flags & fl_onground )
			&& !( g::local->flags() & 1 )
			&& g::local->move_type() != movetype_ladder
			&& g::local->move_type() != movetype_noclip
			&& g::local->move_type() != movetype_observer) ? true : false;

		if (detected_normal_jump_bug && c::movement::jump_bug_detection_printf) {
			interfaces::chat_element->chatprintf("#delusional#_print_jumpbugged");
		}
	}
}

std::vector<vec3_t> ebpos;
bool check_edge_bug(c_usercmd* cmd, bool& brk) {
	if (!g::local)
		return false;

	vec3_t unpredicted_velocity = prediction_backup::velocity;
	vec3_t predicted_velocity = g::local->velocity();
	int predicted_flags = g::local->flags();

	static convar* Sv_gravity = interfaces::console->get_convar(("sv_gravity"));
	auto sv_gravity = Sv_gravity->get_float();

	float z_vel_pred = round(predicted_velocity.z);
	if (z_vel_pred >= 0.f || (predicted_flags & fl_onground)) {
		brk = true;
		return false;
	}
	else if (unpredicted_velocity.z < 0.f && predicted_velocity.z > unpredicted_velocity.z && predicted_velocity.z < 0.f) {
		int z_vel = predicted_velocity.z;

		prediction::begin(cmd);
		prediction::end();

		float rounded_vel = round(-sv_gravity * interfaces::globals->interval_per_tick) + z_vel;
		float unpredicted_vel = round(g::local->velocity().z);

		if (rounded_vel == unpredicted_vel || (unpredicted_vel == 0.f && (g::local->flags() & fl_onground))) {
			return true;
		}
		else {
			brk = true;
			return false;
		}
	}
	return false;
}

vec3_t originalAngle1;
float originalForwardMove1, originalSideMove1;

void start_movement_fix1(c_usercmd* cmd)
{
	originalAngle1 = cmd->view_angles;
	originalForwardMove1 = cmd->forward_move;
	originalSideMove1 = cmd->side_move;
}

void end_movement_fix1(c_usercmd* cmd)
{
	float deltaViewAngles;
	float f1;
	float f2;

	if (originalAngle1.y < 0.f)
		f1 = 360.0f + originalAngle1.y;
	else
		f1 = originalAngle1.y;

	if (cmd->view_angles.y < 0.0f)
		f2 = 360.0f + cmd->view_angles.y;
	else
		f2 = cmd->view_angles.y;

	if (f2 < f1)
		deltaViewAngles = abs(f2 - f1);
	else
		deltaViewAngles = 360.0f - abs(f1 - f2);

	deltaViewAngles = 360.0f - deltaViewAngles;

	cmd->forward_move = cos(math::deg2rad(deltaViewAngles)) * originalForwardMove1 + cos(math::deg2rad(deltaViewAngles + 90.f)) * originalSideMove1;
	cmd->side_move = sin(math::deg2rad(deltaViewAngles)) * originalForwardMove1 + sin(math::deg2rad(deltaViewAngles + 90.f)) * originalSideMove1;
}

void features::movement::edge_bug(c_usercmd* cmd) {
	if (!c::movement::edge_bug || !menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || !g::local || c::movement::edgebug_type == 1) {
		detect_data.detecttick = 0;
		detect_data.edgebugtick = 0;
		detect_data.ticks_left = 0;
		return;
	}

	const int move_type = g::local->move_type();
	if ((prediction_backup::flags & fl_onground) || move_type == movetype_noclip || move_type == movetype_observer || move_type == movetype_ladder) {
		detect_data.detecttick = 0;
		detect_data.edgebugtick = 0;
		detect_data.ticks_left = 0;
		return;
	}

	int BackupButtons = cmd->buttons;
	float BackupForwardMove = cmd->forward_move;
	float BackupSideMove = cmd->side_move;
	vec3_t angViewPointBackup = cmd->view_angles;

	if (!detect_data.ticks_left) {
		convar* get_yaw = interfaces::console->get_convar("m_yaw");
		convar* get_sens = interfaces::console->get_convar("sensitivity");
		const float m_yaw = get_yaw->get_float();
		const float sensitivity = get_sens->get_float();
		int ticklimit = c::movement::edge_bug_ticks;
		float yawdelta = std::clamp(cmd->mouse_dx * m_yaw * sensitivity, -30.f, 30.f);
		int pred_rounds = (c::movement::edge_bug_strafe && yawdelta != 0.f) ? 4 : 2;
		float originalfmove = cmd->forward_move;
		float originalsmove = cmd->side_move;
		vec3_t originalangles = cmd->view_angles;
		detect_data.startingyaw = originalangles.y;
		detect_data.origv = cmd->view_angles;

		ebpos.clear();
		ebpos.push_back(g::local->origin());

		for (int round = 0; round < pred_rounds; ++round){
			if (round > 1) {
				float max_delta = yawdelta;
				for (yawdelta = max_delta / c::movement::edge_bug_rape; abs(yawdelta) <= abs(max_delta); yawdelta += max_delta / c::movement::edge_bug_rape) {
					c_usercmd predictcmd = *cmd;
					if (round == 2) {
						detect_data.crouched = true;
						predictcmd.buttons |= in_duck;
						detect_data.strafing = true;
						predictcmd.view_angles.y = originalangles.y;
						predictcmd.forward_move = originalfmove;
						predictcmd.side_move = originalsmove;
					}
					else if (round == 3) {
						detect_data.crouched = false;
						predictcmd.buttons &= ~in_duck;
						detect_data.strafing = true;
						predictcmd.view_angles.y = originalangles.y;
						predictcmd.forward_move = originalfmove;
						predictcmd.side_move = originalsmove;
					}

					prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

					for (int t = 1; t <= ticklimit; ++t) {
						if (round > 1)
							predictcmd.view_angles.y = math::normalize_yaw(originalangles.y + (yawdelta * t));

						if (c::movement::edge_bug_rape == 0 || c::movement::edge_bug_angle_limit == 0)
							break;

						if (abs(predictcmd.view_angles.y - detect_data.startingyaw) > c::movement::edge_bug_angle_limit)
							break;

						prediction::begin(&predictcmd);
						prediction::end();

						if (g::local->velocity().z > 0.f || g::local->velocity().length_2d() == 0.f || g::local->move_type() == movetype_ladder)
							break;

						ebpos.push_back(g::local->origin());

						bool br = false;

						if (check_edge_bug(&predictcmd, br)) {
							detect_data.ticks_left = t;
							detect_data.eblength = t;
							detect_data.edgebugtick = interfaces::globals->tick_count + t;
							detect_data.detecttick = interfaces::globals->tick_count;
							detect_data.forwardmove = predictcmd.forward_move;
							detect_data.sidemove = predictcmd.side_move;
							detect_data.yawdelta = yawdelta;
							break;
						}

						if (br)
							break;
					}
				}
			}
			else {
				c_usercmd predictcmd = *cmd;
				if (round == 0) {
					detect_data.crouched = true;
					predictcmd.buttons |= in_duck;
					detect_data.strafing = false;
					predictcmd.forward_move = 0.f;
					predictcmd.side_move = 0.f;
				}
				else if (round == 1) {
					detect_data.crouched = false;
					predictcmd.buttons &= ~in_duck;
					detect_data.strafing = false;
					predictcmd.forward_move = 0.f;
					predictcmd.side_move = 0.f;
				}

				prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

				for (int t = 1; t <= ticklimit; ++t) {

					prediction::begin(&predictcmd);
					prediction::end();

					if (g::local->velocity().z > 0.f || g::local->velocity().length_2d() == 0.f ||g::local->move_type() == movetype_ladder)
						break;

					ebpos.push_back(g::local->origin());

					bool br = false;

					if (check_edge_bug(&predictcmd, br)) {
						detect_data.ticks_left = t;
						detect_data.eblength = t;
						detect_data.edgebugtick = interfaces::globals->tick_count + t;
						detect_data.detecttick = interfaces::globals->tick_count;
						detect_data.forwardmove = predictcmd.forward_move;
						detect_data.sidemove = predictcmd.side_move;
						detect_data.yawdelta = yawdelta;
						break;
					}

					if (br)
						break;
				}
			}

			if (detect_data.ticks_left)
				break;
		}
	}

	if (detect_data.ticks_left) {
		if (detect_data.ticks_left == 1) {
			if (c::movement::edge_bug_detection_printf) {
				interfaces::chat_element->chatprintf("#delusional#_print_edgebugged");
			}
			switch (c::movement::edge_bug_detection_sound) {
			case 0: break;
			case 1: interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav"); break;
			case 2: interfaces::surface->play_sound("buttons\\button22.wav"); break;
			case 3: interfaces::surface->play_sound("survival\\money_collect_01.wav"); break;
			case 4: interfaces::surface->play_sound("Ui\\beep07.wav"); break;
			}
			detected_normal_edge_bug = true;
		}

		should_edge_bug = true;

		if (detect_data.crouched)
			cmd->buttons |= in_duck;
		else
			cmd->buttons &= ~in_duck;

		if (detect_data.strafing) {
			cmd->forward_move = detect_data.forwardmove;
			cmd->side_move = detect_data.sidemove;
			cmd->view_angles.y = math::normalize_yaw(detect_data.startingyaw + (detect_data.yawdelta * (detect_data.eblength - (detect_data.ticks_left - 1))));
			if (c::movement::silent_eb_hacked) {
				start_movement_fix1(cmd);
				cmd->view_angles = angViewPointBackup;
				end_movement_fix1(cmd);
			}
		}
		else {
			cmd->forward_move = 0.f;
			cmd->side_move = 0.f;
		}
		detect_data.ticks_left--;
	}
	else {
		should_edge_bug = false;
		detected_normal_edge_bug = false;
	}
}

void features::movement::auto_align(c_usercmd* cmd) {
	if (!c::movement::auto_align) {
		return;
	}

	if (!g::local || !g::local->is_alive() || g::local->flags() & fl_onground) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	float max_radias = m_pi * 2.f;
	float step = max_radias / 16.f;
	vec3_t start_pos = g::local->abs_origin();
	const auto mins = g::local->collideable()->mins();
	const auto maxs = g::local->collideable()->maxs();
	trace_world_only fil;

	for (float a = 0.f; a < max_radias; a += step) {
		vec3_t end_pos;
		end_pos.x = cos(a) + start_pos.x;
		end_pos.y = sin(a) + start_pos.y;
		end_pos.z = start_pos.z;
		trace_t trace;
		ray_t ray;
		ray.initialize(start_pos, end_pos, mins, maxs);

		// traceraying the whole circle around us trying to find a wall
		interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &fil, &trace);
		if ((trace.flFraction < 1.f) && (trace.plane.normal.z == 0.f)) {
			// found a wall
			//interfaces::console->console_printf(std::string("wall").append(" \n").c_str());
			vec3_t angles{ trace.plane.normal.x * -0.005f, trace.plane.normal.y * -0.005f, 0.f };
			const vec3_t end_pos2 = start_pos + angles;
			trace_t trace2;
			// traceraying towards the wall we found to see if we're aligned or not
			ray_t ray2;
			ray2.initialize(start_pos, end_pos2, mins, maxs);
			interfaces::trace_ray->trace_ray(ray2, MASK_PLAYERSOLID, &fil, &trace2);
			if (trace2.flFraction == 1.f) {
				vec3_t to_wall = angles.to_angle();
				to_wall.normalize();
				vec3_t velo = g::local->velocity();
				velo.z = 0.f;
				// checking if your velocity if not zero
				if (velo.length_2d() > 0.f) {
					vec3_t velo_ang = velo.to_angle();
					vec3_t delta = velo_ang - to_wall;
					delta.normalize();
					// comparing our velocity with a vector towards the wall to see if we actually want to align
					if (fabsf(delta.z) > 90.f)
						break;
				}
				// calculating the direction towards the wall to forward and sidemove
				float rotation = math::deg2rad(to_wall.y - cmd->view_angles.y);
				float cos_rot = cos(rotation);
				float sin_rot = sin(rotation);
				bool should_align = false;

				for (int i = 1; i <= 5; i++) {
					// try different forward and sidemove variations to find one that alignes in 1 tick
					float forwardmove = cos_rot * i * 9;
					float sidemove = -sin_rot * i * 9;
					c_usercmd fakecmd = *cmd;
					fakecmd.forward_move = forwardmove;
					fakecmd.side_move = sidemove;
					prediction::begin(&fakecmd);
					prediction::end();

					vec3_t start_pos2 = g::local->abs_origin();
					const vec3_t end_pos3 = start_pos2 + angles;
					trace_t trace3;
					ray_t ray3;
					ray3.initialize(start_pos2, end_pos3, mins, maxs);
					interfaces::trace_ray->trace_ray(ray3, MASK_PLAYERSOLID, &fil, &trace3);
					if (trace3.flFraction < 1.f) {
						cmd->forward_move = forwardmove;
						cmd->side_move = sidemove;
						should_align = true;
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
						break;
					}
					prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
				}
				if (!should_align) {
					for (int i = 1; i <= 5; i++) {
						// same shit here but in 2 ticks bc it might not be able to align in one
						float forwardmove = cos_rot * i * 9;
						float sidemove = -sin_rot * i * 9;
						vec3_t absorigin = g::local->abs_origin();
						c_usercmd fakecmd2 = *cmd;
						fakecmd2.forward_move = forwardmove;
						fakecmd2.side_move = sidemove;
						prediction::begin(&fakecmd2);
						prediction::end();
						prediction::begin(&fakecmd2);
						prediction::end();

						vec3_t start_pos2 = g::local->abs_origin();
						start_pos2.z = absorigin.z;
						const vec3_t end_pos3 = start_pos2 + angles;
						trace_t trace3;
						ray_t ray4;
						ray4.initialize(start_pos2, end_pos3, mins, maxs);
						interfaces::trace_ray->trace_ray(ray4, MASK_PLAYERSOLID, &fil, &trace3);
						if (trace3.flFraction < 1.f) {
							cmd->forward_move = forwardmove;
							cmd->side_move = sidemove;
							should_align = true;
							prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
							break;
						}
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
					}
				}
			}
			else {
				// this part of code works if we are already aligned
				for (int i = 45.f; i > 0.f; i -= 5.f) {
					// finding good forward and sidemove values to keep us stable on the surf
					float forwardmove = 0.f;
					float sidemove = 0.f;
					// direct it towards the direction we're pressing
					if (cmd->buttons & in_forward)
						forwardmove = i;
					if (cmd->buttons & in_back)
						forwardmove = -i;
					if (cmd->buttons & in_moveleft)
						sidemove = -i;
					if (cmd->buttons & in_moveright)
						sidemove = i;

					if (!forwardmove && !sidemove && c::movement::freelook_surf
						&& menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s)) {
						// OR direct it towards the wall when we're not pressing anything and freelook is enabled
						vec3_t to_wall = angles.to_angle();
						to_wall.normalize();
						float rotation = math::deg2rad(to_wall.y - cmd->view_angles.y);
						float cos_rot = cos(rotation);
						float sin_rot = sin(rotation);
						forwardmove = cos_rot * i;
						sidemove = -sin_rot * i;
					}

					// checking if the value is good to keep stable and using it
					c_usercmd fakecmd5 = *cmd;
					fakecmd5.forward_move = forwardmove;
					fakecmd5.side_move = sidemove;
					prediction::begin(&fakecmd5);
					prediction::end();

					float zvelo = g::local->velocity().z;
					if (zvelo == -6.25f) {
						prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
						cmd->forward_move = forwardmove;
						cmd->side_move = sidemove;
						break;
					}
					prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
				}
			}
			break;
		}
	}
}

void features::movement::pixel_surf_lock(c_usercmd* cmd) {
	if (!c::movement::pixel_surf || !menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
		return;

	if (!g::local || !g::local->is_alive()) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (ps_data.pixelsurfing) {

		prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

		if (!ps_data.predicted_ps) {
			if (ps_data.pixeltick < cmd->tick_count) {

				if (c::movement::pixel_surf_detection_printf) {
					interfaces::chat_element->chatprintf("#delusional#_print_pixelsurfed");
				}

				detected_normal_pixel_surf = true;
				ps_data.predicted_ps = true;
			}
			else {
				detected_normal_pixel_surf = false;
			}

			cmd->buttons = ps_data.pixelsurf_cmd->buttons;
			cmd->view_angles = ps_data.pixelsurf_cmd->view_angles;
			cmd->side_move = ps_data.pixelsurf_cmd->side_move;
			cmd->forward_move = ps_data.pixelsurf_cmd->forward_move;
			cmd->up_move = ps_data.pixelsurf_cmd->up_move;
		}

		if (ps_data.pixelducking)
			cmd->buttons |= in_duck;
		else
			cmd->buttons &= ~in_duck;

		if (ps_data.predicted_ps && !(g::local->velocity().z == -6.25)) {
			should_ps = false;
			ps_data.pixelsurfing = false;
			ps_data.predicted_ps = false;
			return;
		}
		return;
	}
}

void features::movement::pixel_surf(c_usercmd* cmd) {
	if (!c::movement::pixel_surf || !menu::checkkey(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s))
		return;

	if (!g::local || !g::local->is_alive() || g::local->flags() & fl_onground) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (ps_data.pixelsurfing)
		return;

	for (int s = 0; s < 2; s++) {
		if (ps_data.pixelsurfing)
			break;

		prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);

		int flags = g::local->flags();
		vec3_t velocity = g::local->velocity();

		for (int i = 0; i < 16; i++) { 
			c_usercmd* predictcmd = new c_usercmd(*cmd);

			if (s == 0)
				predictcmd->buttons |= in_duck;
			else 
				predictcmd->buttons &= ~in_duck;

			prediction::begin(predictcmd);

			if (flags & fl_onground)
				break;

			if (g::local->velocity().z == -6.25f && velocity.z == -6.25f) {
				ps_data.pixeltick = cmd->tick_count + i;
				ps_data.pixelsurf_cmd = predictcmd;
				ps_data.pixelducking = (s == 0);
				ps_data.pixelsurfing = true;
				should_ps = true;
				break;
			}

			flags = g::local->flags();
			velocity = g::local->velocity();
		}
		prediction::end();
	}	
}

void features::movement::pixel_surf_fix(c_usercmd* cmd) {
	if (!c::movement::pixel_surf_fix) {
		return;
	}

	if (!g::local->is_alive())
		return;

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (cmd->buttons & in_duck)
		return;

	vec3_t velo = g::local->velocity();

	if ((velo.length_2d() <= 285.92f) || (velo.z >= 0.f))
		return;

	prediction::begin(cmd);
	prediction::end();

	if (g::local->flags() & fl_onground) {
		int tickrate = 1 / interfaces::globals->interval_per_tick;
		float airaccelerate = interfaces::console->get_convar("sv_airaccelerate")->get_float();
		float wishdelta = (velo.length_2d() - 285.92f) * tickrate / airaccelerate;
		velo = velo * -1.f;
		vec3_t velo_ang = velo.to_angle();
		velo_ang.normalize();
		float rotation = math::deg2rad(velo_ang.y - cmd->view_angles.y);
		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);
		cmd->forward_move = cos_rot * wishdelta;
		cmd->side_move = -sin_rot * wishdelta;
	}
	prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
}

void features::movement::auto_duck(c_usercmd* cmd) {
	if (!menu::checkkey(c::movement::auto_duck_key, c::movement::auto_duck_key_s))
		return;

}

void features::movement::fake_backwards(c_usercmd* cmd) {
	if (!c::movement::fakebackwards) {
		return;
	}

	if (!interfaces::engine->is_in_game() || !g::local->is_alive()) 
		return;



}

auto interpolate(const color_t first_color, const color_t& second_color, const float multiplier) {
	return color_t(first_color.r() + std::clamp(multiplier, 0.f, 1.f) * (second_color.r() - first_color.r()),
		first_color.g() + std::clamp(multiplier, 0.f, 1.f) * (second_color.g() - first_color.g()),
		first_color.b() + std::clamp(multiplier, 0.f, 1.f) * (second_color.b() - first_color.b()),
		first_color.a() + std::clamp(multiplier, 0.f, 1.f) * (second_color.a() - first_color.a()));
}

void features::movement::velocity_indicator( ) {
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game()) {
		return;
	}

	if (c::movement::velocity_indicator) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		y -= c::movement::velocity_indicator_position;

		static bool on_ground = false;
		static auto take_off = 0;
		static auto take_off_time = 0.0f;
		static auto last_on_ground = false;

		int vel = round(g::local->velocity().length_2d());
		on_ground = g::local->flags() & fl_onground;

		if (last_on_ground && !on_ground) {
			take_off = vel;
			take_off_time = interfaces::globals->cur_time + (c::movement::velocity_indicator_disable_ong_show_pre ? 0.0f : 3.5f);
		}

		last_on_ground = on_ground;

		static float velocity_alpha = 0.0f;
		velocity_alpha = 1.f * (float(round(vel)) / 255.f);

		auto velocity_color = [](int velocity) -> color_t {
			if (velocity > 0)
				return color_t(c::movement::velocity_indicator_positive_clr[0], c::movement::velocity_indicator_positive_clr[1], c::movement::velocity_indicator_positive_clr[2], 255.f);
			else if (velocity < 0)
				return color_t(c::movement::velocity_indicator_negative_clr[0], c::movement::velocity_indicator_negative_clr[1], c::movement::velocity_indicator_negative_clr[2],255.f);
			else if (velocity > -1 && velocity < 1)
				return color_t(c::movement::velocity_indicator_neutral_clr[0], c::movement::velocity_indicator_neutral_clr[1], c::movement::velocity_indicator_neutral_clr[2], 255.f);
		};

		color_t velocity_clr_int = color_t(c::movement::velocity_indicator_fade_clr3[0], c::movement::velocity_indicator_fade_clr3[1], c::movement::velocity_indicator_fade_clr3[2], c::movement::velocity_indicator_fade_clr3[3]);
		color_t velocity_clr_custom = color_t(c::movement::velocity_indicator_custom_clr[0], c::movement::velocity_indicator_custom_clr[1], c::movement::velocity_indicator_custom_clr[2], c::movement::velocity_indicator_custom_clr[3]);

		int tick_count = interfaces::globals->tick_count;
		last_delta = vel - last_vel;

		const auto should_draw_takeoff =
			(!on_ground || (take_off_time > interfaces::globals->cur_time)) && c::movement::velocity_indicator_show_pre;

		std::string place_holder = "";

		if (should_draw_takeoff)
			place_holder = std::to_string(vel) + " (" + std::to_string(take_off) + ")";
		else
			place_holder = std::to_string(vel);

		im_render.text(x / 2, y, c::fonts::indi_size, fonts::indicator_font, place_holder.c_str(), true, c::movement::velocity_indicator_custom_color ? interpolate(velocity_clr_int, velocity_clr_custom, velocity_alpha) : velocity_color(last_delta), c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);

		if (fabs(tick_count - last_tick) > 5) {
			last_tick = tick_count;
			last_vel = vel;
		}
	}
}

void features::movement::stamina_indicator( ) {
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game()) {
		return;
	}

	if (c::movement::stamina_indicator) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);
		y -= c::movement::stamina_indicator_position;

		static float last_speed = 0, last_jump_speed = 0;
		static float last_vel_update = 0.0f;
		static bool last_onground = false;
		static auto take_off_time = 0.0f;
		bool current_onground = g::local->flags() & fl_onground;
		float stamina = g::local->stamina();
		float current_speed = stamina;

		if (last_onground && !current_onground) {
			last_jump_speed = current_speed;
			take_off_time = interfaces::globals->cur_time + (c::movement::stamina_indicator_disable_ong_show_pre ? 0.0f : 3.5f);
		}

		static float stamina_alpha = 0.f;
		stamina_alpha = 1.f * (float(stamina) / 25.f);

		color_t stamina_clr = current_speed < last_speed ? color_t(c::movement::stamina_indicator_clr[0], c::movement::stamina_indicator_clr[1], c::movement::stamina_indicator_clr[2], c::movement::stamina_indicator_clr[3]) : color_t(c::movement::stamina_indicator_clr[0], c::movement::stamina_indicator_clr[1], c::movement::stamina_indicator_clr[2], c::movement::stamina_indicator_clr[3]);
		color_t stamina_clr_int = current_speed < last_speed ? color_t(c::movement::stamina_fade_clr[0], c::movement::stamina_fade_clr[1], c::movement::stamina_fade_clr[2], c::movement::stamina_fade_clr[3]) : color_t(c::movement::stamina_fade_clr[0], c::movement::stamina_fade_clr[1], c::movement::stamina_fade_clr[2], c::movement::stamina_fade_clr[3]);

		std::string value_str;

		std::stringstream ss;
		ss << std::fixed << std::setprecision(current_speed > 0.0f ? 1 : 1) << current_speed;
		value_str = ss.str();

		std::string str = value_str;

		std::stringstream ss1;
		ss1 << std::fixed << std::setprecision(1) << last_jump_speed;
		std::string value_str2 = ss1.str();

		const auto should_draw_takeoff =
			(!current_onground || (take_off_time > interfaces::globals->cur_time)) && c::movement::stamina_indicator_show_pre;

		if (should_draw_takeoff)
			str += " (" + value_str2 + ")";
		else
			str = value_str;

		im_render.text(x / 2, y, c::fonts::indi_size, fonts::indicator_font, str.c_str(), true, c::movement::stamina_indicator_fade ? interpolate(stamina_clr_int, stamina_clr, stamina_alpha) : stamina_clr, c::fonts::indi_font_flag[9], c::fonts::indi_font_flag[10]);


		if (interfaces::globals->cur_time > last_vel_update) {
			last_speed = current_speed;
			last_vel_update = interfaces::globals->cur_time + 0.05f;
		}
		last_onground = current_onground;
	}
}

void render_indicator(int key, int key_s, int& alpha, color_t& clr, const char* text, bool tick_count, int should_detect, bool allow_detection_clr, int& position, int saved_tick = 0) {
	int x, y;
	interfaces::engine->get_screen_size(x, y);
	y -= c::movement::indicators_position;

    color_t default_clr(c::movement::velocity_indicator_custom_clr2[0], c::movement::velocity_indicator_custom_clr2[1], c::movement::velocity_indicator_custom_clr2[2]);

	if (allow_detection_clr) {
		if (tick_count)
			clr = interfaces::globals->tick_count - saved_tick < c::movement::detection_saved_tick ? color_t(c::movement::indicator_detect_clr[0], c::movement::indicator_detect_clr[1], c::movement::indicator_detect_clr[2]) : default_clr;
		else
			clr = should_detect ? color_t(c::movement::indicator_detect_clr[0], c::movement::indicator_detect_clr[1], c::movement::indicator_detect_clr[2]) : default_clr;
	}
	else
		clr = default_clr;

	float fading_speed = (c::movement::indicators_fading_speed*4.f)/100.f;

	if (menu::checkkey(key, key_s)) {
		alpha = std::clamp(alpha + fading_speed, 0.f, 255.f);
		if (!c::movement::indicators_allow_animation) {
			im_render.text(x / 2, y + position, c::fonts::sub_indi_size, fonts::sub_indicator_font, text, true, clr, c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
			position += (24 + c::movement::indicators_gap);
		}
	}
	else {
		alpha = std::clamp(alpha - fading_speed, 0.f, 255.f);
		if (tick_count)
			should_detect = false;
	}

	if (c::movement::indicators_allow_animation && alpha >= 1) {
		im_render.text(x / 2, y + position, c::fonts::sub_indi_size, fonts::sub_indicator_font, text, true, clr.get_color_edit_alpha_int(alpha), c::fonts::sub_indi_font_flag[9], c::fonts::sub_indi_font_flag[10]);
		position += (24 + c::movement::indicators_gap);
	}
}

void features::movement::indicators() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !g::local || !g::local->is_alive()) {
		saved_tick_jb = 0;
		saved_tick_lj = 0;
		saved_tick_ej = 0;
		saved_tick_mj = 0;
		return;
	}

	if (detected_normal_jump_bug)
		saved_tick_jb = interfaces::globals->tick_count;

	if (should_ej)
		saved_tick_ej = interfaces::globals->tick_count;

	if (should_lj)
		saved_tick_lj = interfaces::globals->tick_count;

	if (should_mj)
		saved_tick_mj = interfaces::globals->tick_count;

	color_t ps_clr, al_clr, sh_clr, eb_clr, jb_clr, ej_clr, lj_clr, mj_clr, lg_clr, as_clr;
	static int p_alpha, al_alpha, sh_alpha, eb_alpha, jb_alpha, ej_alpha, lj_alpha, mj_alpha, lb_alpha, as_alpha = 0;
	int position = 0;

	if (c::movement::indicators_show[0]) {
		if (c::movement::edgebug_type == 0) {
			render_indicator(c::movement::edge_bug_key, c::movement::edge_bug_key_s, eb_alpha, eb_clr, "eb", false, should_edge_bug, c::movement::detection_clr_for[0], position);
		}
		else {
			render_indicator(c::movement::edge_bug_key, c::movement::edge_bug_key_s, eb_alpha, eb_clr, "eb", false, lobotomy_eb::EdgeBug_Founded, c::movement::detection_clr_for[0], position);
		}
	}
	if (c::movement::indicators_show[2])
		render_indicator(c::movement::long_jump_key, c::movement::long_jump_key_s, lj_alpha, lj_clr, "lj", true, should_lj, c::movement::detection_clr_for[2], position, saved_tick_lj);

	if (c::movement::indicators_show[5])
		render_indicator(c::movement::edge_jump_key, c::movement::edge_jump_key_s, ej_alpha, ej_clr, "ej", true, should_ej, c::movement::detection_clr_for[6], position, saved_tick_ej);

	if (c::movement::indicators_show[3])
		render_indicator(c::movement::mini_jump_key, c::movement::mini_jump_key_s, mj_alpha, mj_clr, "mj", true, should_mj, c::movement::detection_clr_for[3], position, saved_tick_mj);

	if (c::movement::indicators_show[4])
		render_indicator(c::movement::pixel_surf_key, c::movement::pixel_surf_key_s, p_alpha, ps_clr, "ps", false, should_ps, c::movement::detection_clr_for[4], position);
	
	if (c::movement::indicators_show[1])
		render_indicator(c::movement::jump_bug_key, c::movement::jump_bug_key_s, jb_alpha, jb_clr, "jb", true, detected_normal_jump_bug, c::movement::detection_clr_for[1], position, saved_tick_jb);
	
	if (c::movement::indicators_show[6])
		render_indicator(c::movement::ladder_bug_key, c::movement::ladder_bug_key_s, lb_alpha, lg_clr, "lb", false, should_lb, c::movement::detection_clr_for[6], position);

	if (c::movement::indicators_show[7])
		render_indicator(c::movement::auto_strafe_key, c::movement::auto_strafe_key_s, as_alpha, as_clr, "autostrafing", false, false, c::movement::detection_clr_for[7], position);
}

features::movement::velocity_data_t current_vel_data;
static std::deque<std::pair<float, bool>> stamina_data;
void features::movement::gather_vel_graph_data( ) {
	if (!g::local || !g::local->is_alive() || !c::movement::velocity_graph)
		return;

	if (!interfaces::engine->is_connected())
		return;

	float speed = g::local->velocity().length_2d();
	int flags = g::local->flags();
	bool on_ground = flags & fl_onground;

	current_vel_data.speed = speed;
	current_vel_data.on_ground = on_ground;
	current_vel_data.edge_bug = detected_normal_edge_bug;
	current_vel_data.jump_bug = detected_normal_jump_bug;
	current_vel_data.pixel_surf = detected_normal_pixel_surf;

	velocity_data.insert(velocity_data.begin(), current_vel_data);

	if (velocity_data.size() > 185)
		velocity_data.pop_back();
}

void features::movement::gather_stam_graph_data( ) {
	if (!g::local || !g::local->is_alive() || !c::movement::stamina_graph)
		return;

	if (!interfaces::engine->is_connected())
		return;

	stamina_data.push_front({g::local->stamina(), !(g::local->flags() & fl_onground) });

	if (stamina_data.size() > 185)
		stamina_data.pop_back();
}

void features::movement::velocity_graph_draw( ) {
	if (!g::local->is_alive() || !g::local)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		velocity_data.clear();
		return;
	}

	if (c::movement::velocity_graph) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);

		if (velocity_data.size() == 185) {
			for (auto i = 0; i < velocity_data.size() - 2; i++) {
				auto& curdata = velocity_data[i];//
				auto& nextdata = velocity_data[i + 1];
				auto cur = curdata.speed;
				auto next = nextdata.speed;
				auto landed = !curdata.on_ground && nextdata.on_ground;
				auto eb = curdata.edge_bug;
				auto jb = curdata.jump_bug;
				auto ps = curdata.pixel_surf;
				int alpha = 255;

				switch (c::movement::graph_fade) {
				case 0:
					//no faded
					break;
				case 1:
					//faded start and end
					if (i < 70) {
						float a = ((i + 1) / 70.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > velocity_data.size() - 71) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 70.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 2:
					//faded by vel
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}

					alpha *= std::clamp(((cur + next) / 2) / c::movement::velocity_tofade, 0.f, 1.f);
					break;
				case 3:
					//faded end
					if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 4:
					//faded start
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				}

				alpha = std::clamp(alpha, 0, 255);

				color_t linecolor = color_t(c::movement::velocity_graph_color[0], c::movement::velocity_graph_color[1], c::movement::velocity_graph_color[2], (alpha / 255.f));

				im_render.drawline(
					((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i - 1) * c::movement::velocity_linewidth),
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(cur, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					x / 2 + (((velocity_data.size() / 2) * c::movement::velocity_linewidth)) - i * c::movement::velocity_linewidth,
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					linecolor, 0);

				if (landed && c::movement::velocity_graph_show_landed_speed && !eb)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 30 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 12, fonts::esp_misc, std::to_string((int)round(next)).c_str(), true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);

				if (eb && c::movement::velocity_graph_show_edge_bug)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 10 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 12, fonts::esp_misc, "eb", true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);

				if (jb  && c::movement::velocity_graph_show_jump_bug)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 10 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 12, fonts::esp_misc, "jb", true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);

				if (ps && c::movement::velocity_graph_show_pixel_surf)
					im_render.text(((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i + 1) * c::movement::velocity_linewidth), y * c::movement::graph_xscreenpos - 10 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height), 12, fonts::esp_misc, "ps", true, color_t(1.f, 1.f, 1.f, (alpha / 255.f)), true);
			}
		}
	}
}

void features::movement::stamina_graph_draw( ) {
	if (!g::local->is_alive() || !g::local)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		stamina_data.clear();
		return;
	}

	if (c::movement::stamina_graph) {
		int x, y;
		interfaces::engine->get_screen_size(x, y);

		if (stamina_data.size() == 185) {
			for (auto i = 0; i < stamina_data.size() - 2; i++) {
				auto& curdata = stamina_data[i];//
				auto& nextdata = stamina_data[i + 1];
				auto cur = curdata.first;
				auto next = nextdata.first;
				auto landed = curdata.second && !nextdata.second;
				int alpha = 255;

				switch (c::movement::graph_fade) {
				case 0:
					//no faded
					break;
				case 1:
					//faded start and end
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > stamina_data.size() - 66) {
						int todivide = stamina_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 2:
					//faded by vel
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					else if (i > stamina_data.size() - 66) {
						int todivide = stamina_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}

					alpha *= std::clamp(((cur + next) / 2) / 1.f, 0.f, 1.f);//ee
					break;
				case 3:
					//faded end
					if (i > velocity_data.size() - 66) {
						int todivide = velocity_data.size() - i;
						float a = (todivide / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				case 4:
					//faded start
					if (i < 65) {
						float a = ((i + 1) / 65.f) * 255.f;
						alpha = (int)a;
					}
					break;
				}

				alpha = std::clamp(alpha, 0, 255);

				color_t linecolor = color_t(c::movement::stamina_graph_color[0], c::movement::stamina_graph_color[1], c::movement::stamina_graph_color[2], (alpha / 255.f));

				im_render.drawline(
					((x / 2) + (((velocity_data.size() / 2) * c::movement::velocity_linewidth))) - ((i - 1) * c::movement::velocity_linewidth),
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(cur, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					x / 2 + (((velocity_data.size() / 2) * c::movement::velocity_linewidth)) - i * c::movement::velocity_linewidth,
					y * c::movement::graph_xscreenpos - 15 - ((std::clamp(next, 0.f, 450.f) * 75 / 320) * c::movement::velocity_height),
					linecolor, 0);

			}
		}
	}
}

void features::movement::visualize_eb() {
	if (!g::local->is_alive() || !g::local)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		return;
	}

	if (c::movement::visualize_edge_bug && detect_data.ticks_left && c::movement::edgebug_type == 0) {

		for (int i = 0; i < ebpos.size() - 1; i++) {
			vec3_t cur;
			vec3_t next;
			if (interfaces::debug_overlay->world_to_screen(ebpos.at(i), cur) && interfaces::debug_overlay->world_to_screen(ebpos.at(i + 1), next)) {
				im_render.drawline(cur.x, cur.y, next.x, next.y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			}
		}

		vec3_t endpoints[4];
		if (interfaces::debug_overlay->world_to_screen(vec3_t{ ebpos.at(ebpos.size() - 1).x - 16.f, ebpos.at(ebpos.size() - 1).y - 16.f, ebpos.at(ebpos.size() - 1).z }, endpoints[0]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ ebpos.at(ebpos.size() - 1).x - 16.f, ebpos.at(ebpos.size() - 1).y + 16.f, ebpos.at(ebpos.size() - 1).z }, endpoints[1]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ ebpos.at(ebpos.size() - 1).x + 16.f, ebpos.at(ebpos.size() - 1).y - 16.f, ebpos.at(ebpos.size() - 1).z }, endpoints[2]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ ebpos.at(ebpos.size() - 1).x + 16.f, ebpos.at(ebpos.size() - 1).y + 16.f, ebpos.at(ebpos.size() - 1).z }, endpoints[3]))
		{
			im_render.drawline(endpoints[0].x, endpoints[0].y, endpoints[1].x, endpoints[1].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[1].x, endpoints[1].y, endpoints[3].x, endpoints[3].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[3].x, endpoints[3].y, endpoints[2].x, endpoints[2].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[2].x, endpoints[2].y, endpoints[0].x, endpoints[0].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
		}
	}
	else if (c::movement::visualize_edge_bug && lobotomy_eb::EdgeBug_Founded && c::movement::edgebug_type == 1) {
		for (int i = 0; i < lobotomy_eb::lbebpos.size() - 1; i++) {
			vec3_t cur;
			vec3_t next;
			if (interfaces::debug_overlay->world_to_screen(lobotomy_eb::lbebpos.at(i), cur) && interfaces::debug_overlay->world_to_screen(lobotomy_eb::lbebpos.at(i + 1), next)) {
				im_render.drawline(cur.x, cur.y, next.x, next.y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			}
		}

		vec3_t endpoints[4];
		if (interfaces::debug_overlay->world_to_screen(vec3_t{ lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).x - 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).y - 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).z }, endpoints[0]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).x - 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).y + 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).z }, endpoints[1]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).x + 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).y - 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).z }, endpoints[2]) &&
			interfaces::debug_overlay->world_to_screen(vec3_t{ lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).x + 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).y + 16.f, lobotomy_eb::lbebpos.at(lobotomy_eb::lbebpos.size() - 1).z }, endpoints[3]))
		{
			im_render.drawline(endpoints[0].x, endpoints[0].y, endpoints[1].x, endpoints[1].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[1].x, endpoints[1].y, endpoints[3].x, endpoints[3].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[3].x, endpoints[3].y, endpoints[2].x, endpoints[2].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
			im_render.drawline(endpoints[2].x, endpoints[2].y, endpoints[0].x, endpoints[0].y, color_t(c::movement::visualize_edge_bug_clr[0], c::movement::visualize_edge_bug_clr[1], c::movement::visualize_edge_bug_clr[2]));
		}
	}
}