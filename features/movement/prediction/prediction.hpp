#pragma once
#include "../../../sdk/sdk.hpp"

using md5_pseudo_random_fn = uint32_t(__thiscall*)(uint32_t);

namespace prediction_backup{
	inline player_move_data data{};
	inline vec3_t		origin{};
	inline vec3_t		velocity{};
	inline vec3_t      angles{};
	inline c_usercmd* cmd{};
	inline float		forward_move{};
	inline float		side_move{};
	inline float		curtime{};
	inline float		frametime{};
	inline bool		in_pred = false;
	inline bool		in_first_time = true;
	inline int		tickcount{};
	inline int       player_tickbase{};
	inline int		movetype{};
	inline int		flags{};
	inline int* prediction_random_seed{};
	inline player_t* prediction_player{ nullptr };
}

namespace prediction {
	inline void backup_originals(c_usercmd* cmd) {
		if (!g::local || !g::local->is_alive())
			return;

		//player						   
		prediction_backup::flags = g::local->flags();
		prediction_backup::velocity = g::local->velocity();
		prediction_backup::origin = g::local->origin();
		prediction_backup::player_tickbase = g::local->get_tick_base();
		prediction_backup::movetype = g::local->move_type();
		//cmd
		prediction_backup::cmd = cmd;
		prediction_backup::angles = cmd->view_angles;
		prediction_backup::side_move = cmd->side_move;
		prediction_backup::forward_move = cmd->forward_move;
	};

	void begin( c_usercmd* cmd );
	void end();
	void restore_ent_to_predicted_frame(int i);
	void repredict();
	void post_think();
	void updatepacket();

	inline player_move_data data{};
	inline c_usercmd* last_command;
	inline int* prediction_random_seed;
	inline md5_pseudo_random_fn md5_pseudo_random;
	inline int* prediction_player;
	inline player_t** prediction_player_real;
	inline int correct_tickbase;
	inline bool in_first;
	inline bool in_pred;
	inline bool in_prediction;
	
	inline vec3_t origin;
	inline vec3_t velocity;
	inline vec3_t original_angle;

	inline float old_cur_time;
	inline float old_frame_time;
	inline int old_tick_count;
}