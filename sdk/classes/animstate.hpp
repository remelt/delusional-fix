#pragma once
#include <cstdint>
#include "../math/vec3.hpp"

class player_t;
class weapon_t;

class c_anim_state_pose_param_cache {
	std::uint8_t pad_0x0[0x4]{}; //0x0
public:
	std::uint32_t m_idx{}; //0x4 
	char* m_name{}; //0x8
};

class procedural_foot_t {
public:
	vec3_t m_pos_anim{};
	vec3_t m_pos_anim_last{};
	vec3_t m_pos_plant{};
	vec3_t m_plant_vel{};
	float m_lock_amount{};
	float m_last_plant_time{};
};

class c_anim_state {
	std::uint8_t pad_0x0000[0x4]{}; //0x0000
public:
	bool m_force_update{}; //0x0005 
private:
	std::uint8_t pad_0x0006[0x5A]{}; //0x0006
public:
	player_t* m_player{}; //0x0060 
	weapon_t* m_weapon{}; //0x0064 
	weapon_t* m_last_weapon{}; //0x0068 
	float m_last_clientside_anim_update{}; //0x006C 
	uint32_t m_last_clientside_anim_framecount{}; //0x0070 
	float m_last_clientside_anim_update_time_delta{}; //0x0074 
	float m_eye_yaw{}; //0x0078 
	float m_pitch{}; //0x007C 
	float m_abs_yaw{}; //0x0080 
	float m_feet_yaw{}; //0x0084 
	float m_body_yaw{}; //0x0088 
	float m_body_yaw_clamped{}; //0x008C 
	float m_feet_vel_dir_delta{}; //0x0090 
private:
	uint8_t pad_0x0094[0x4]{}; //0x0094
public:
	float m_feet_cycle{}; //0x0098 
	float m_feet_yaw_rate{}; //0x009C 
private:
	uint8_t pad_0x00A0[0x4]{}; //0x00A0
public:
	float m_duck_amount{}; //0x00A4 
	float m_landing_duck_additive{}; //0x00A8 
private:
	uint8_t pad_0x00AC[0x4]{}; //0x00AC
public:
	vec3_t m_origin{};
	vec3_t m_old_origin{};
	vec3_t m_velocity{};
	vec3_t m_velocity_normalized{};
	vec3_t m_last_accelerating_vel{};
	float m_speed2d{};
	float m_abs_velocity{};
	float m_speed_normalized{};
	float m_run_speed{}; //0x00F8 
	float m_duck_speed{}; //0x00FC 
	float m_time_since_move{}; //0x0100 
	float m_time_since_stop{}; //0x0104 
	bool m_on_ground{}; //0x0108 
	bool m_hit_ground{}; //0x0109 
private:
	uint8_t pad_0x010A[0x4]{}; //0x010A
public:
	float m_time_in_air{}; //0x0110 
private:
	uint8_t pad_0x0114[0x6]{}; //0x0114
public:
	float m_ground_fraction{}; //0x011C 
private:
	uint8_t pad_0x0120[0x2]{}; //0x0120
public:
	float m_unk_fraction{}; //0x0124 
private:
	uint8_t pad_0x0128[0xC]{}; //0x0128
public:
	bool m_moving{}; //0x0134
private:
	uint8_t pad_0x0135[0x7B]{}; //0x0135
public:
	c_anim_state_pose_param_cache m_lean_yaw_pose{}; //0x1B0
	c_anim_state_pose_param_cache m_speed_pose{}; //0x01BC
	c_anim_state_pose_param_cache m_ladder_speed_pose{}; //0x01C8
	c_anim_state_pose_param_cache m_ladder_yaw_pose{}; //0x01D4
	c_anim_state_pose_param_cache m_move_yaw_pose{}; //0x01E0
	c_anim_state_pose_param_cache m_run_pose{}; //0x01EC 
	c_anim_state_pose_param_cache m_body_yaw_pose{}; //0x01F8
	c_anim_state_pose_param_cache m_body_pitch_pose{}; //0x0204
	c_anim_state_pose_param_cache m_dead_yaw_pose{}; //0x0210
	c_anim_state_pose_param_cache m_stand_pose{}; //0x021C
	c_anim_state_pose_param_cache m_jump_fall_pose{}; //0x0228
	c_anim_state_pose_param_cache m_aim_blend_stand_idle_pose{}; //0x0234
	c_anim_state_pose_param_cache m_aim_blend_crouch_idle_pose{}; //0x0240
	c_anim_state_pose_param_cache m_strafe_yaw_pose{}; //0x024C
	c_anim_state_pose_param_cache m_aim_blend_stand_walk_pose{}; //0x0258
	c_anim_state_pose_param_cache m_aim_blend_stand_run_pose{}; //0x0264
	c_anim_state_pose_param_cache m_aim_blend_crouch_walk_pose{}; //0x0270
	c_anim_state_pose_param_cache m_move_blend_walk_pose{}; //0x027C
	c_anim_state_pose_param_cache m_move_blend_run_pose{}; //0x0288
	c_anim_state_pose_param_cache m_move_blend_crouch_pose{}; //0x0294
	float m_duration_move_weight_is_too_high{};
	float m_static_approach_speed{};
	int m_previous_move_state{};
	float m_stutter_step{};
	float m_action_weight_bias_remainder{};
	procedural_foot_t m_foot_left;
	procedural_foot_t m_foot_right;
	float m_camera_smooth_height{};
	bool m_smooth_height_valid{};
	float m_last_time_velocity_over_ten{};
	float m_min_yaw{}; //0x0330 
	float m_max_yaw{}; //0x0334 
	float m_max_pitch{}; //0x0338 
	float m_min_pitch{}; //0x033C
};

enum anim_event
{
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY = 0,
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_OPT, // an optional primary attack for variation in animation. For example, the knife toggles between left AND right slash animations.
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_SPECIAL1,
	PLAYERANIMEVENT_FIRE_GUN_PRIMARY_OPT_SPECIAL1, // an optional primary special attack for variation in animation.
	PLAYERANIMEVENT_FIRE_GUN_SECONDARY,
	PLAYERANIMEVENT_FIRE_GUN_SECONDARY_SPECIAL1,
	PLAYERANIMEVENT_GRENADE_PULL_PIN,
	PLAYERANIMEVENT_THROW_GRENADE,
	PLAYERANIMEVENT_JUMP,
	PLAYERANIMEVENT_RELOAD,
	PLAYERANIMEVENT_RELOAD_START,	///< w_model partial reload for shotguns
	PLAYERANIMEVENT_RELOAD_LOOP,	///< w_model partial reload for shotguns
	PLAYERANIMEVENT_RELOAD_END,		///< w_model partial reload for shotguns
	PLAYERANIMEVENT_CLEAR_FIRING,	///< clear animations on the firing layer
	PLAYERANIMEVENT_DEPLOY,			///< Used to play deploy animations on third person models.
	PLAYERANIMEVENT_SILENCER_ATTACH,
	PLAYERANIMEVENT_SILENCER_DETACH,

	// new events
	PLAYERANIMEVENT_THROW_GRENADE_UNDERHAND,
	PLAYERANIMEVENT_CATCH_WEAPON,
	PLAYERANIMEVENT_COUNT
};