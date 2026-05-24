#pragma once
#include "../../utils/padding.hpp"
#include "../math/vec3.hpp"
#include "crc32/crc32.h"

enum cmd_buttons {
	in_attack = ( 1 << 0 ),
	in_jump = ( 1 << 1 ),
	in_duck = ( 1 << 2 ),
	in_forward = ( 1 << 3 ),
	in_back = ( 1 << 4 ),
	in_use = ( 1 << 5 ),
	in_cancel = ( 1 << 6 ),
	in_left = ( 1 << 7 ),
	in_right = ( 1 << 8 ),
	in_moveleft = ( 1 << 9 ),
	in_moveright = ( 1 << 10 ),
	in_attack2 = ( 1 << 11 ),
	in_run = ( 1 << 12 ),
	in_reload = ( 1 << 13 ),
	in_alt1 = ( 1 << 14 ),
	in_alt2 = ( 1 << 15 ),
	in_score = ( 1 << 16 ),
	in_speed = ( 1 << 17 ),
	in_walk = ( 1 << 18 ),
	in_zoom = ( 1 << 19 ),
	in_weapon1 = ( 1 << 20 ),
	in_weapon2 = ( 1 << 21 ),
	in_bullrush = ( 1 << 22 ),
	in_grenade1 = ( 1 << 23 ),
	in_grenade2 = ( 1 << 24 ),
	in_attack3 = ( 1 << 25 )
};

class c_usercmd {
public:
	virtual ~c_usercmd( ){};

	int command_number;
	int tick_count;
	vec3_t view_angles;
	vec3_t aim_direction;
	float forward_move;
	float side_move;
	float up_move;
	int buttons;
	unsigned char impulse;
	int weapon_select;
	int weapon_subtype;
	int random_seed;
	short mouse_dx;
	short mouse_dy;
	bool has_been_predicted;
	vec3_t m_head_angles;
	vec3_t m_head_offset;

	unsigned int get_check_sum() const
	{
		unsigned int hash_crc = 0UL;

		crc32::init(&hash_crc);
		crc32::process_buffer(&hash_crc, &this->command_number, sizeof(this->command_number));
		crc32::process_buffer(&hash_crc, &this->tick_count, sizeof(this->tick_count));
		crc32::process_buffer(&hash_crc, &this->view_angles, sizeof(this->view_angles));
		crc32::process_buffer(&hash_crc, &this->aim_direction, sizeof(this->aim_direction));
		crc32::process_buffer(&hash_crc, &this->forward_move, sizeof(this->forward_move));
		crc32::process_buffer(&hash_crc, &this->side_move, sizeof(this->side_move));
		crc32::process_buffer(&hash_crc, &this->up_move, sizeof(this->up_move));
		crc32::process_buffer(&hash_crc, &this->buttons, sizeof(this->buttons));
		crc32::process_buffer(&hash_crc, &this->impulse, sizeof(this->impulse));
		crc32::process_buffer(&hash_crc, &this->weapon_select, sizeof(this->weapon_select));
		crc32::process_buffer(&hash_crc, &this->weapon_subtype, sizeof(this->weapon_subtype));
		crc32::process_buffer(&hash_crc, &this->random_seed, sizeof(this->random_seed));
		crc32::process_buffer(&hash_crc, &this->mouse_dx, sizeof(this->mouse_dx));
		crc32::process_buffer(&hash_crc, &this->mouse_dy, sizeof(this->mouse_dy));
		crc32::end(&hash_crc);

		return hash_crc;
	}
};

class c_verified_user_cmd
{
public:
	c_usercmd m_user_cmd;
	unsigned int m_hash_crc;
};