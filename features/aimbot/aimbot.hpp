#pragma once
#include "../../sdk/sdk.hpp"

namespace aimbot {
	void run(c_usercmd* cmd);
	void StopMovement(c_usercmd* cmd);
}

namespace triggerbot {
	void run(c_usercmd* cmd);
}

struct fire_bullet_data {
	fire_bullet_data(const vec3_t& eyePos, player_t* entity) {
		src = eyePos;
		filter.skip = entity;
	}

	vec3_t          src;
	trace_t         enter_trace;
	vec3_t          direction;
	trace_filter   filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

namespace autowall {
	bool can_hit_floating_point(const vec3_t& point, const vec3_t& source);
	float damage(const vec3_t& point);
}

struct backtrack_data {
	player_t* player;
	float sim_time;
	studio_hitbox_set_t* hitboxset;
	vec3_t m_headpos;
	matrix_t m_matrix[128];
};

class backtrack_c {
public:
	void setup_records();
	void on_move(c_usercmd* pCmd);
	float correct_time = 0.0f;
	float latency = 0.0f;
	float lerp_time = 0.0f;
	int selected_tick = 0;
	int sel_ent_index = 0;
	std::map<int, std::deque<backtrack_data>> data;
};
extern backtrack_c backtrack;

class fake_latency {
private:
	struct incoming_sequence_t {
		incoming_sequence_t(int instate, int outstate, int seqnr, float time) {
			m_in_reliable_state = instate;
			m_out_reliable_state = outstate;
			m_sequence_nr = seqnr;
			m_current_time = time;
		}
		int m_in_reliable_state;
		int m_out_reliable_state;
		int m_sequence_nr;
		float m_current_time;
	};
	std::deque< incoming_sequence_t > sequences;
	int m_last_incoming_sequence_number;
public:
	bool flipped_state = false;
	void update_sequence(i_net_channel* net_channel);
	void clear_sequence();
	void add_latency(i_net_channel* net_channel, float latency);
};
extern fake_latency fakeping;

namespace cvar {
	inline float sv_maxunlag;
	inline float sv_minupdaterate;
	inline float sv_maxupdaterate;
	inline float sv_client_min_interp_ratio;
	inline float sv_client_max_interp_ratio;
	inline float cl_interp_ratio;
	inline float cl_interp;
	inline float cl_updaterate;
	inline float flLerpRatio;
	inline float updateRate;

	inline void init() {
		sv_maxunlag = interfaces::console->get_convar("sv_maxunlag")->get_float();
		sv_minupdaterate = interfaces::console->get_convar("sv_minupdaterate")->get_float();
		sv_maxupdaterate = interfaces::console->get_convar("sv_maxupdaterate")->get_float();
		sv_client_min_interp_ratio = interfaces::console->get_convar("sv_client_min_interp_ratio")->get_float();
		sv_client_max_interp_ratio = interfaces::console->get_convar("sv_client_max_interp_ratio")->get_float();
		cl_interp_ratio = interfaces::console->get_convar("cl_interp_ratio")->get_float();
		cl_interp = interfaces::console->get_convar("cl_interp")->get_float();
		cl_updaterate = interfaces::console->get_convar("cl_updaterate")->get_float();

		flLerpRatio = std::clamp(flLerpRatio, sv_client_min_interp_ratio, sv_client_max_interp_ratio);
		if (flLerpRatio == 0.0f)
			flLerpRatio = 1.0f;
		updateRate = std::clamp(cl_updaterate, sv_minupdaterate, sv_maxupdaterate);
	}
}