#pragma once
#include "../../sdk/sdk.hpp"

namespace rng_factor
{
	inline bool near_enemy = false;
	inline bool players_iskilled = false;
	inline bool player_ishurt = false;
	inline float percent_rng_factor = 0.f;

}

namespace features::misc {
	inline int spec_p = 0;
	struct notify_t {
		std::string text;
		float time;
		color _color;
		notify_t( std::string _text, color __color ) {
			text = _text;
			_color = __color;
			time = interfaces::globals->cur_time;
		}
	};
	inline std::vector<notify_t> notify_list;

	struct hit_info_t {
		std::string player_name;
		int damage;
		int health;
		std::string hitgroup;

	};
	inline hit_info_t hitinfo;

	struct iwebz_t {
		int add_x = 300;
		int add_y = 100;
		int width = 195;
		int height = 20;
		int drag_pos_x = 0;
		int drag_pos_y = 0;
		bool allow_to_move = false;
		int count = 0;
		float t_size = 0.0f;
	};
	inline iwebz_t iwebzspec;

	void draw();
	void handle_spotify();
	void notify( std::string text, color _color );
	void spectators_list();
	void spectators_list_iwebz();
	void kz_practice();
	void kz_practice_logic(c_usercmd* cmd);
	void reveal_server_ranks(c_usercmd* cmd);
	void door_spam(c_usercmd* cmd);
	void fix_mouse_delta(c_usercmd* cmd);
	void reset_thirdperson();
	void thirdperson();
	void view_model();
	void clantag_spammer();
	void force_crosshair();
	void sniper_crosshair();
	void radaringame(player_t* p);
	void vote_revealer(i_game_event* event);
	void hit_info(i_game_event* event);
	void kill_say(i_game_event* event);

	namespace hitmarker {
		void draw();
		void event(i_game_event* event);
	}

	namespace jumpstats {
		void jumpstats(c_usercmd* cmd);
	}

}