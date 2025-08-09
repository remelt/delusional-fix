#pragma once
#include "../math/vec3.hpp"
#include "../structs/usercmd.hpp"
#include "../../utils/vfunc.hpp"
#include "../../utils/utlvector.hpp"

class player_t;
using c_base_handle = std::uintptr_t;

class player_move_helper {
public:
	bool	first_run_of_iunctions : 1;
	bool	game_code_moved_player : 1;
	int	player_handle;
	int	impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int	buttons;
	int	old_buttons;
	float	forward_move;
	float	side_move;
	float	up_move;
	float	max_speed;
	float	client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float	out_step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float	constraint_radius;
	float	constraint_width;
	float	constraint_speed_factor;
	float	u0[ 5 ];
	vec3_t	abs_origin;
	virtual	void u1( ) = 0;
	virtual void set_host( player_t* host ) = 0;
	virtual void reset_touch_list(void) = 0;
	virtual bool pad() = 0;
	virtual void process_impacts(void) = 0;
};

class player_move_data {
public:
	bool    first_run_of_instructions : 1;
	bool    game_code_moved_player : 1;
	int     player_handle;
	int     impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int     buttons;
	int     old_buttons;
	float   fw_move;
	float   sd_move;
	float   up_move;
	float   max_speed;
	float   client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float   step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float   constraint_radius;
	float   constraint_width;
	float   constraint_speed_factor;
	float   u0[ 5 ];
	vec3_t	abs_origin;
};

class virtual_game_movement {

public:
	virtual				~virtual_game_movement( void ) {}
	virtual void			process_movement( player_t* player, player_move_data* move ) = 0;
	virtual void			reset( void ) = 0;
	virtual void			start_track_prediction_errors( player_t* player ) = 0;
	virtual void			finish_track_prediction_errors( player_t* player ) = 0;
	virtual void			diff_print( char const* fmt, ... ) = 0;
	virtual vec3_t const& get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const& get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const& get_player_view_offset( bool ducked ) const = 0;
	virtual bool			is_moving_player_stuck( void ) const = 0;
	virtual player_t* get_moving_player( void ) const = 0;
	virtual void			unblock_posher( player_t* player, player_t* pusher ) = 0;
	virtual void			setup_movement_bounds( player_move_data* move ) = 0;
};

class player_game_movement : public virtual_game_movement {
public:
	virtual ~player_game_movement( void ) { }
};

class player_prediction {
public:
	std::byte		pad0[0x4];						// 0x0000
	std::uintptr_t	last_ground;					// 0x0004
	bool			in_prediction;					// 0x0008
	bool			old_cl_predict_value;				// 0x000B
	bool			is_first_time_predicted;			// 0x0009
	bool			engine_paused;					// 0x000A
	int				i_previous_start_frame;			// 0x000C
	int				incoming_packet_number;			// 0x0010
	float			last_server_world_time_stamp;		// 0x0014

	struct split_t {
		bool		is_first_time_predicted;			// 0x0018
		std::byte	pad0[0x3];						// 0x0019
		int			commands_predicted;				// 0x001C
		int			server_commands_acknowledged;	// 0x0020
		int			previous_ack_had_errors;			// 0x0024
		float		ideal_pitch;					// 0x0028
		int			last_command_acknowledged;		// 0x002C
		bool		previous_ack_error_triggers_full_latch_reset; // 0x0030
		CUtlVector<c_base_handle> entities_with_prediction_errors_in_last_ack; // 0x0031
		bool		performed_tick_shift;			// 0x0045
	};

	split_t			split[1];						// 0x0018
	// SavedGlobals 0x4C

public:
	void update(int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_command) {
		call_vfunc<void>(this, 3, start_frame, valid_frame, incoming_acknowledged, outgoing_command);
	}
	void get_local_view_angles(vec3_t& view) {
		call_vfunc<void>(this, 12, std::ref(view));
	}
	void set_local_view_angles(vec3_t& view) {
		call_vfunc<void>(this, 13, std::ref(view));
	}
	void check_moving_ground(player_t* ent, double frametime) {
		call_vfunc<void>(this, 18, ent, frametime);
	}
	void setup_move(player_t* ent, c_usercmd* cmd, player_move_helper* helper, player_move_data* move_data) {
		call_vfunc<void>(this, 20, ent, cmd, helper, move_data);
	}
	void finish_move(player_t* ent, c_usercmd* cmd, player_move_data* move_data) {
		call_vfunc<void>(this, 21, ent, cmd, move_data);
	}
	int32_t& predicted_commands() {
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + 0x1C);
	}

	int32_t& previous_start_frame() {
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + 0xC);
	}
};
