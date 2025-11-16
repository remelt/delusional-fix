#pragma once
#include "../../sdk/sdk.hpp"

class MegaPrivateAimbotOtAntohi {
public:
	void RCS(vec3_t& angle);
	void run(c_usercmd* cmd);
#define CHECK_VALID( _v ) 0
	inline void vec3_tSubtract(const vec3_t& a, const vec3_t& b, vec3_t& c)
	{
		CHECK_VALID(a);
		CHECK_VALID(b);
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	inline void sinCos(float radians, PFLOAT sine, PFLOAT cosine)
	{
		__asm
		{
			fld dword ptr[radians]
			fsincos
			mov edx, dword ptr[cosine]
			mov eax, dword ptr[sine]
			fstp dword ptr[edx]
			fstp dword ptr[eax]
		}
	}
	//Чучуть асэма не помешает Приват епта // antosha (rip)
	void anglevec3_ts(vec3_t& angles, vec3_t& forward)
	{
		float	sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, math::deg2rad(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, math::deg2rad(angles[1]));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}
	//legitbot_s settings;
	//legitbot_s option;
	float GetFovToPlayer(vec3_t viewAngle, vec3_t aimAngle)
	{
		vec3_t delta = aimAngle - viewAngle;
		math::fix_angles(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}

	inline float GetFov(vec3_t vLocalOrigin, vec3_t vPosition, vec3_t vForward)
	{
		vec3_t vLocal;

		vec3_tSubtract(vPosition, vLocalOrigin, vLocal);

		vLocal.NormalizeInPlace();

		float fValue = vForward.dot(vLocal);

		//np for kolo's math skills
		if (fValue < -1.0f)
			fValue = -1.0f;

		if (fValue > 1.0f)
			fValue = 1.0f;

		return math::rad2deg(acos(fValue));
	}


	vec3_t current_punch = { 0, 0, 0 };
	vec3_t last_punch = { 0, 0, 0 };
};
extern MegaPrivateAimbotOtAntohi g_Aimbot;

namespace triggerbot {
	void run(c_usercmd* cmd);
}

struct backtrack_data {
	player_t* player;
	float sim_time;
	studio_hitbox_set_t* hitboxset;
	vec3_t m_headpos;
	matrix_t m_matrix[128];
	vec3_t head;
	vec3_t chest;
	vec3_t pelvis;
	vec3_t neck;
	vec3_t lchest;
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