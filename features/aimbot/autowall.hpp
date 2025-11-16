#include "../../sdk/sdk.hpp"

namespace Autowall
{
	struct FireBulletData
	{
		vec3_t src;
		trace_t enter_trace;
		vec3_t direction;
		trace_filter filter;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	};

	float GetDamage(const vec3_t& vecPoint);
	bool SimulateFireBullet(weapon_t* pWeapon, FireBulletData& data);
	bool HandleBulletPenetration(weapon_info_t* weaponInfo, FireBulletData& data);
	bool TraceToExit(vec3_t& end, trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace);
	bool DidHitNonWorldEntity(player_t* player);
	void ScaleDamage(int hitbox, player_t* enemy, float weapon_armor_ratio, float& current_damage);
	float GetHitgroupDamageMultiplier(int hitbox);
	void ClipTraceToPlayers(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, uint32_t mask, trace_filter* filter, trace_t* tr);
}