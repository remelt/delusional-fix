#include "lobotomy_eb.h"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"

//PASTED FROM LOBOTOMY V2
//RECODED SOME LOGIC
//CAN BE IMPROVED BUT IM TOO LAZY TO FIX ALL THAT STUFF

vec3_t BackupVelocity{ };
vec3_t BackupOrigin;
int BackupMoveType;
int BackupFlags;
vec3_t BackupQAngle;
int EdgeBug_Ticks;
int TicksForDetect;
#define M_PI 3.14159265358979323846
vec3_t edgebug_velocity_backup;

void lobotomy_eb::PrePredictionEdgeBug(c_usercmd* cmd)
{
	if (!g::local)
		return;
	if (!g::local->is_alive())
		return;
	BackupVelocity = g::local->get_velocity();
	BackupOrigin = g::local->abs_origin();
	BackupMoveType = g::local->move_type();
	BackupFlags = g::local->flags();
	BackupQAngle = cmd->view_angles;
	edgebug_velocity_backup = g::local->get_velocity();
}

bool breaklol = false;

bool lobotomy_eb::EdgeBugCheck(c_usercmd* cmd)
{
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		return false;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return false;
	}
	static auto sv_gravity = interfaces::console->get_convar("sv_gravity")->get_float();

	float gravity_vel = (sv_gravity * 0.5f * interfaces::globals->interval_per_tick);

	if (edgebug_velocity_backup.z < -gravity_vel && round(g::local->get_velocity().z) == -round(gravity_vel)) {
		return true;
	}
	float sv_gravity2 = interfaces::console->get_convar(("sv_gravity"))->get_float();
	float fTickInterval = interfaces::globals->interval_per_tick;
	float fTickRate = (fTickInterval > 0) ? (1.0f / fTickInterval) : 0.0f;
	float veloZoftick = (((sv_gravity2 / 2) / fTickRate) + 0.25f);
	float targetZvelo = (veloZoftick * 6.0f / 6.25f) * -1.f;
	if (edgebug_velocity_backup.z < targetZvelo && g::local->get_velocity().z > edgebug_velocity_backup.z &&
		g::local->get_velocity().z < targetZvelo) {
		float velocty_before_engineprediction = g::local->get_velocity().z;
		vec3_t CheckEbOrigin = g::local->abs_origin();
		CheckEbOrigin.z += 200.f;
		prediction::begin(cmd);
		prediction::end();

		return true;

	}

	return false;
}
int edgebug_tick;
int edgebug_prediction_ticks = 0;
int edgebug_prediction_timestamp = 0;
int edgebug_mouse_offset = 0;
void  lobotomy_eb::ReStorePrediction()
{
	prediction::restore_ent_to_predicted_frame(interfaces::prediction->split->commands_predicted - 1);
}
struct NewCmd3 {
	vec3_t viewangle;
	float forwardmove;
	float sideMove;
	int buttons;
	vec3_t origin;
};
NewCmd3 NewCmd[128];

void CorrectMovement(c_usercmd* cmd, vec3_t wish_angle, vec3_t old_angles)
{
	if (old_angles.x != wish_angle.x || old_angles.y != wish_angle.y || old_angles.z != wish_angle.z) {
		vec3_t wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

		auto viewangles = old_angles;
		auto movedata = vec3_t(cmd->forward_move, cmd->side_move, cmd->up_move);
		viewangles.normalize();

		if (!(g::local->flags() & 1) && viewangles.z != 0.f)
			movedata.y = 0.f;

		math::angle_vectors(wish_angle, &wish_forward, &wish_right, &wish_up);
		math::angle_vectors(viewangles, &cmd_forward, &cmd_right, &cmd_up);

		auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y),
			v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

		vec3_t wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
			wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f), wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

		auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y),
			v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

		vec3_t cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
			cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
			cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

		auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x,
			v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y,
			v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

		vec3_t correct_movement;
		correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25 +
			(cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28) +
			(cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
		correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25 +
			(cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28) +
			(cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
		correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25 +
			(cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28) +
			(cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

		correct_movement.x = std::clamp(correct_movement.x, -450.f, 450.f);
		correct_movement.y = std::clamp(correct_movement.y, -450.f, 450.f);
		correct_movement.z = std::clamp(correct_movement.z, -320.f, 320.f);

		cmd->forward_move = correct_movement.x;
		cmd->side_move = correct_movement.y;
		cmd->up_move = correct_movement.z;

		cmd->buttons &= ~(in_moveright | in_moveleft | in_back | in_forward);
		if (cmd->side_move != 0.0) {
			if (cmd->side_move <= 0.0)
				cmd->buttons |= in_moveleft;
			else
				cmd->buttons |= in_moveright;
		}

		if (cmd->forward_move != 0.0) {
			if (cmd->forward_move <= 0.0)
				cmd->buttons |= in_back;
			else
				cmd->buttons |= in_forward;
		}
	}
}

int sec_LastType = 0;
vec3_t aim{ };

void lobotomy_eb::EdgeBugPostPredict(c_usercmd* cmd)
{
	if (!c::movement::edge_bug || !menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || !g::local || c::movement::edgebug_type == 0) {
		EdgeBug_Founded = false;
		EdgeBug_Ticks = 0;
		DuckEd = false;
		return;
	}
	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		EdgeBug_Founded = false;
		EdgeBug_Ticks = 0;
		return;
	}
	if (!g::local) {
		EdgeBug_Founded = false;
		EdgeBug_Ticks = 0;
		return;
	}
	if (!g::local->is_alive())
		return;

	if (DuckEd) {
		if (!(BackupFlags & 1))
			cmd->buttons &= ~in_jump;
		else
			cmd->buttons |= in_jump;
		cmd->buttons |= in_duck;
	}
	if (BackupFlags & 1) {
		DuckEd = false;
		return;
	}
	if (!EdgeBug_Founded && DuckEd && BackupVelocity.z < -40.f)
		DuckEd = false;
	if (c::movement::MegaEdgeBug)
		c::movement::deltascaler = 179.f;

	auto isLargeAngleDifference = [](float yaw1, float yaw2) -> bool {
		float yawDifference = std::abs(yaw1 - yaw2);

		if (yawDifference > 180.0f)
			yawDifference = 360.0f - yawDifference;

		return yawDifference >= c::movement::deltascaler;
		};
	static int LastType = 0;

	static vec3_t NewAngle;
	int BackupButtons = cmd->buttons;
	float BackupForwardMove = cmd->forward_move;
	float BackupSideMove = cmd->side_move;
	vec3_t angViewPointBackup = cmd->view_angles;
	float DeltaSalier = 0.f;
	if (c::movement::DeltaType == 0)
		DeltaSalier = 1.f;
	if (c::movement::DeltaType == 1)
		DeltaSalier = 0.8f;
	if (c::movement::DeltaType == 2)
		DeltaSalier = 0.5f;
	if (c::movement::DeltaType == 3)
		DeltaSalier = 0.25f;
	if (c::movement::MegaEdgeBug)
		DeltaSalier = 1.f;
	vec3_t DeltaAngle = (angViewPointBackup - NewAngle) * DeltaSalier;
	NewAngle = cmd->view_angles;

	if (!EdgeBug_Founded) {

		lbebpos.clear();
		lbebpos.push_back(g::local->origin());

		for (int v = 0; v < c::movement::EdgeBugCircle; v++) {
			if (LastType) {
				v = LastType;
				LastType = 0;
			}
			ReStorePrediction();
			edgebug_velocity_backup = g::local->get_velocity();

			cmd->view_angles = angViewPointBackup;
			cmd->forward_move = BackupForwardMove;
			cmd->side_move = BackupSideMove;
			cmd->buttons = BackupButtons;
			vec3_t currentAngle = cmd->view_angles;
			for (int i = 0; i < c::movement::EdgeBugTicks; i++) {
				if (EdgeBug_Founded || g::local->flags() & 1 || g::local->get_velocity().z > 0.f)
					break;
				if (c::movement::MegaEdgeBug) {
					if (v == 0) {
						cmd->buttons &= ~in_duck;
						cmd->forward_move = 0;
						cmd->side_move = 0;
					}
					if (v == 1) {
						cmd->buttons |= in_duck;
						cmd->forward_move = 0;
						cmd->side_move = 0;
					}
					if (v == 2) {
						cmd->buttons &= ~in_duck;

						{
							cmd->forward_move = BackupForwardMove;
							cmd->side_move = BackupSideMove;

							if (!isLargeAngleDifference(currentAngle.y, angViewPointBackup.y))
								currentAngle = (currentAngle + DeltaAngle).normalized().clamped();
							cmd->view_angles = currentAngle;
							if (c::movement::SiletEdgeBug) {
								features::movement::start_movement_fix(cmd);
								cmd->view_angles = angViewPointBackup;
								features::movement::end_movement_fix(cmd);
							}
						}
					}
					if (v == 3) {
						cmd->buttons |= in_duck;

						{
							cmd->forward_move = BackupForwardMove;
							cmd->side_move = BackupSideMove;
							if (!isLargeAngleDifference(currentAngle.y, angViewPointBackup.y))
								currentAngle = (currentAngle + DeltaAngle).normalized().clamped();
							cmd->view_angles = currentAngle;
							if (c::movement::SiletEdgeBug) {
								features::movement::start_movement_fix(cmd);
								cmd->view_angles = angViewPointBackup;
								features::movement::end_movement_fix(cmd);
							}
						}
					}
					if (v == 4) {
						cmd->buttons &= ~in_duck;

						{
							static auto side = 1.f;
							side = -side;

							auto velocity = g::local->get_velocity();

							vec3_t wish_angle = cmd->view_angles;

							auto speed = velocity.length_2d();
							auto ideal_strafe = std::clamp(math::rad2deg(atan(15.f / speed)), 0.f, 90.f);

							cmd->forward_move = 0.f;

							static auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");

							static float old_yaw = 0.f;
							auto yaw_delta = std::remainderf(wish_angle.y - old_yaw, 360.f);
							auto abs_yaw_delta = abs(yaw_delta);
							old_yaw = wish_angle.y;

							const auto cl_sidespeed_value = cl_sidespeed->get_float();

							if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f) {
								vec3_t velocity_direction;
								math::angle_vectors(velocity, velocity_direction);
								auto velocity_delta = std::remainderf(wish_angle.y - velocity_direction.y, 360.0f);
								auto retrack = std::clamp(math::rad2deg(atan(30.f / speed)), 0.f, 90.f) * 2.f;
								if (velocity_delta <= retrack || speed <= 15.f) {
									if (-retrack <= velocity_delta || speed <= 15.0f) {
										wish_angle.y += side * ideal_strafe;
										cmd->side_move = cl_sidespeed_value * side;
									}
									else {
										wish_angle.y = velocity_direction.y - retrack;
										cmd->side_move = cl_sidespeed_value;
									}
								}
								else {
									wish_angle.y = velocity_direction.y + retrack;
									cmd->side_move = -cl_sidespeed_value;
								}

								CorrectMovement(cmd, wish_angle, cmd->view_angles);
							}
							else if (yaw_delta > 0.f)
								cmd->side_move = -cl_sidespeed_value;
							else
								cmd->side_move = cl_sidespeed_value;
						}
					}
					if (v == 5) {
						cmd->buttons |= in_duck;

						{
							static auto side = 1.f;
							side = -side;

							auto velocity = g::local->get_velocity();

							vec3_t wish_angle = cmd->view_angles;

							auto speed = velocity.length_2d();
							auto ideal_strafe = std::clamp(math::rad2deg(atan(15.f / speed)), 0.f, 90.f);

							cmd->forward_move = 0.f;

							static auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");

							static float old_yaw = 0.f;
							auto yaw_delta = std::remainderf(wish_angle.y - old_yaw, 360.f);
							auto abs_yaw_delta = abs(yaw_delta);
							old_yaw = wish_angle.y;

							const auto cl_sidespeed_value = cl_sidespeed->get_float();

							if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f) {
								vec3_t velocity_direction;
								math::angle_vectors(velocity, velocity_direction);
								auto velocity_delta = std::remainderf(wish_angle.y - velocity_direction.y, 360.0f);
								auto retrack = std::clamp(math::rad2deg(atan(30.f / speed)), 0.f, 90.f) * 2.f;
								if (velocity_delta <= retrack || speed <= 15.f) {
									if (-retrack <= velocity_delta || speed <= 15.0f) {
										wish_angle.y += side * ideal_strafe;
										cmd->side_move = cl_sidespeed_value * side;
									}
									else {
										wish_angle.y = velocity_direction.y - retrack;
										cmd->side_move = cl_sidespeed_value;
									}
								}
								else {
									wish_angle.y = velocity_direction.y + retrack;
									cmd->side_move = -cl_sidespeed_value;
								}

								CorrectMovement(cmd, wish_angle, cmd->view_angles);
							}
							else if (yaw_delta > 0.f)
								cmd->side_move = -cl_sidespeed_value;
							else
								cmd->side_move = cl_sidespeed_value;
						}
					}
				}
				else {
					if (v == 0) {
						cmd->buttons &= ~in_duck;
						cmd->forward_move = 0;
						cmd->side_move = 0;
					}
					if (v == 1) {
						cmd->buttons |= in_duck;
						cmd->forward_move = 0;
						cmd->side_move = 0;
					}
					if (v == 2) {
						cmd->buttons &= ~in_duck;
						if (c::movement::AutoStrafeEdgeBug)
						{
							static auto side = 1.f;
							side = -side;

							auto velocity = g::local->get_velocity();

							vec3_t wish_angle = cmd->view_angles;

							auto speed = velocity.length_2d();
							auto ideal_strafe = std::clamp(math::rad2deg(atan(15.f / speed)), 0.f, 90.f);

							cmd->forward_move = 0.f;

							static auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");

							static float old_yaw = 0.f;
							auto yaw_delta = std::remainderf(wish_angle.y - old_yaw, 360.f);
							auto abs_yaw_delta = abs(yaw_delta);
							old_yaw = wish_angle.y;

							const auto cl_sidespeed_value = cl_sidespeed->get_float();

							if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f) {
								vec3_t velocity_direction;
								math::angle_vectors(velocity, velocity_direction);
								auto velocity_delta = std::remainderf(wish_angle.y - velocity_direction.y, 360.0f);
								auto retrack = std::clamp(math::rad2deg(atan(30.f / speed)), 0.f, 90.f) * 2.f;
								if (velocity_delta <= retrack || speed <= 15.f) {
									if (-retrack <= velocity_delta || speed <= 15.0f) {
										wish_angle.y += side * ideal_strafe;
										cmd->side_move = cl_sidespeed_value * side;
									}
									else {
										wish_angle.y = velocity_direction.y - retrack;
										cmd->side_move = cl_sidespeed_value;
									}
								}
								else {
									wish_angle.y = velocity_direction.y + retrack;
									cmd->side_move = -cl_sidespeed_value;
								}

								CorrectMovement(cmd, wish_angle, cmd->view_angles);
							}
							else if (yaw_delta > 0.f)
								cmd->side_move = -cl_sidespeed_value;
							else
								cmd->side_move = cl_sidespeed_value;
						}
						else {
							cmd->forward_move = BackupForwardMove;
							cmd->side_move = BackupSideMove;

							if (!isLargeAngleDifference(currentAngle.y, angViewPointBackup.y))
								currentAngle = (currentAngle + DeltaAngle).normalized().clamped();
							cmd->view_angles = currentAngle;
							if (c::movement::SiletEdgeBug) {
								features::movement::start_movement_fix(cmd);
								cmd->view_angles = angViewPointBackup;
								features::movement::end_movement_fix(cmd);
							}
						}
					}
					if (v == 3) {
						cmd->buttons |= in_duck;
						if (c::movement::AutoStrafeEdgeBug)
						{
							static auto side = 1.f;
							side = -side;

							auto velocity = g::local->get_velocity();

							vec3_t wish_angle = cmd->view_angles;

							auto speed = velocity.length_2d();
							auto ideal_strafe = std::clamp(math::rad2deg(atan(15.f / speed)), 0.f, 90.f);

							cmd->forward_move = 0.f;

							static auto cl_sidespeed = interfaces::console->get_convar("cl_sidespeed");

							static float old_yaw = 0.f;
							auto yaw_delta = std::remainderf(wish_angle.y - old_yaw, 360.f);
							auto abs_yaw_delta = abs(yaw_delta);
							old_yaw = wish_angle.y;

							const auto cl_sidespeed_value = cl_sidespeed->get_float();

							if (abs_yaw_delta <= ideal_strafe || abs_yaw_delta >= 30.f) {
								vec3_t velocity_direction;
								math::angle_vectors(velocity, velocity_direction);
								auto velocity_delta = std::remainderf(wish_angle.y - velocity_direction.y, 360.0f);
								auto retrack = std::clamp(math::rad2deg(atan(30.f / speed)), 0.f, 90.f) * 2.f;
								if (velocity_delta <= retrack || speed <= 15.f) {
									if (-retrack <= velocity_delta || speed <= 15.0f) {
										wish_angle.y += side * ideal_strafe;
										cmd->side_move = cl_sidespeed_value * side;
									}
									else {
										wish_angle.y = velocity_direction.y - retrack;
										cmd->side_move = cl_sidespeed_value;
									}
								}
								else {
									wish_angle.y = velocity_direction.y + retrack;
									cmd->side_move = -cl_sidespeed_value;
								}

								CorrectMovement(cmd, wish_angle, cmd->view_angles);
							}
							else if (yaw_delta > 0.f)
								cmd->side_move = -cl_sidespeed_value;
							else
								cmd->side_move = cl_sidespeed_value;

						}
						else {
							cmd->forward_move = BackupForwardMove;
							cmd->side_move = BackupSideMove;
							if (!isLargeAngleDifference(currentAngle.y, angViewPointBackup.y))
								currentAngle = (currentAngle + DeltaAngle).normalized().clamped();
							cmd->view_angles = currentAngle;
							if (c::movement::SiletEdgeBug) {
								features::movement::start_movement_fix(cmd);
								cmd->view_angles = angViewPointBackup;
								features::movement::end_movement_fix(cmd);
							}
						}
					}
				}
				NewCmd[i].forwardmove = cmd->forward_move;
				NewCmd[i].sideMove = cmd->side_move;
				NewCmd[i].buttons = cmd->buttons;
				NewCmd[i].viewangle = cmd->view_angles;
				NewCmd[i].origin = g::local->origin();
				vec3_t OldVelocity = g::local->get_velocity();

				if (EdgeBug_Founded || g::local->flags() & 1 || g::local->get_velocity().z > 0.f || g::local->move_type() == movetype_ladder || g::local->move_type() == movetype_noclip)
					break;

				prediction::begin(cmd);
				prediction::end();

				lbebpos.push_back(g::local->origin());

				EdgeBug_Founded = EdgeBugCheck(cmd);
				if (breaklol) {
					breaklol = false;
					break;
				}
				edgebug_velocity_backup = g::local->get_velocity();

				if (EdgeBug_Founded) {
					EdgeBug_Ticks = i;
					LastType = v;
					sec_LastType = v;
					edgebug_tick = interfaces::globals->tick_count + (i + 1);
					edgebug_prediction_ticks = i;
					edgebug_prediction_timestamp = interfaces::globals->tick_count;
					edgebug_mouse_offset = std::abs(cmd->mouse_dx);
				}
			}
			cmd->view_angles = angViewPointBackup;
			cmd->forward_move = BackupForwardMove;
			cmd->side_move = BackupSideMove;
			cmd->buttons = BackupButtons;
		}
	}
	if (EdgeBug_Founded) {
		ReStorePrediction();
		static int Hits = 0;
		vec3_t org = g::local->origin();
		float dist = org.distance(NewCmd[Hits].origin);

		if (Hits > EdgeBug_Ticks || dist > 1.f) {
			Hits = 0;
			EdgeBug_Founded = false;
			sec_LastType = 0;
			return;
		}
		if (NewCmd[Hits].buttons & in_duck) {
			cmd->buttons |= in_duck;
			DuckEd = true;
		}
		else
			cmd->buttons &= ~in_duck;
		if (BackupFlags & 1)
			cmd->buttons |= in_jump;
		else
			cmd->buttons &= ~in_jump;
		cmd->forward_move = NewCmd[Hits].forwardmove;
		cmd->side_move = NewCmd[Hits].sideMove;
		cmd->view_angles = NewCmd[Hits].viewangle;
		if (c::movement::SiletEdgeBug) {
			features::movement::start_movement_fix(cmd);
			cmd->view_angles = angViewPointBackup;
			features::movement::end_movement_fix(cmd);
		}
		aim = cmd->view_angles;
		Hits += 1;
	}
}

struct DATAFORDETECT {
	vec3_t velocity;
	bool onground;
};

std::deque< DATAFORDETECT > VelocitiesForDetection;

void lobotomy_eb::edgebug_detect(c_usercmd* cmd)
{

	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (const auto fg = g::local->flags(); fg == fl_inwater || fg == fl_waterjump) {
		return;
	}

	if (!c::movement::edge_bug || !menu::checkkey(c::movement::edge_bug_key, c::movement::edge_bug_key_s) || c::movement::edgebug_type == 0) {
		return;
	}

	if (EdgeBug_Founded && edgebug_prediction_timestamp + EdgeBug_Ticks == interfaces::globals->tick_count)
	{
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
	}
}

void  lobotomy_eb::edgebug_lock(float& x, float& y)
{
	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected()) {
		return;
	}

	if (const auto fg = g::local->flags(); fg == fl_inwater || fg == fl_waterjump) {
		return;
	}

	if (const auto mt = g::local->move_type(); mt == movetype_ladder || mt == movetype_noclip) {
		return;
	}

	if (EdgeBug_Founded) {
		if (x != 0.0) {
			float v3 = (float)(edgebug_prediction_timestamp + edgebug_prediction_ticks - interfaces::globals->tick_count) / x;
			float v4 = 100.0 / (atan2f(edgebug_prediction_ticks, v3) + 100.0 + (float)(c::movement::EdgeBugMouseLock/10 * edgebug_mouse_offset));
			if (!isnan(v4))
				x = x * v4;
		}
	}
}

void lobotomy_eb::frame_stage(int stage)
{
	if (stage == frame_start) {
		static vec3_t aim_to = aim;

		if (EdgeBug_Founded) {
			if (!c::movement::SiletEdgeBug && sec_LastType > 1 && !c::movement::AutoStrafeEdgeBug) {
				const float smoothingFactor = 0.1f;
				const float maxDeltaAngle = 10.f;

				auto normalizeDelta = [](float delta) -> float {
					while (delta > 180.f)
						delta -= 360.f;
					while (delta < -180.f)
						delta += 360.f;
					return delta;
					};

				float deltaPitch = normalizeDelta(aim.x - aim_to.x);
				if (fabs(deltaPitch) > maxDeltaAngle)
					deltaPitch = (deltaPitch > 0 ? maxDeltaAngle : -maxDeltaAngle);
				aim_to.x += deltaPitch * smoothingFactor;

				float deltaYaw = normalizeDelta(aim.y - aim_to.y);
				if (fabs(deltaYaw) > maxDeltaAngle)
					deltaYaw = (deltaYaw > 0 ? maxDeltaAngle : -maxDeltaAngle);
				aim_to.y += deltaYaw * smoothingFactor;

				float deltaRoll = normalizeDelta(aim.z - aim_to.z);
				if (fabs(deltaRoll) > maxDeltaAngle)
					deltaRoll = (deltaRoll > 0 ? maxDeltaAngle : -maxDeltaAngle);
				aim_to.z += deltaRoll * smoothingFactor;

				aim_to.normalize();
				interfaces::engine->set_view_angles(aim_to);
			}
		}
		else {
			aim_to = BackupQAngle;
		}
	}
}




