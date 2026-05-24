#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"
#include "../../features/aimbot/aimbot.hpp"

void __fastcall sdk::hooks::fire_bullet::fire_bullet(player_t* _this, void* edx, vec3_t vecSource, const vec3_t* pangShootView, float flDistance, float flPenetration, int nPenetrationCount, int nBulletType, int iDamage, float flRangeModifier, player_t* pAttacker, bool bDoEffects, float flSpreadX, float flSpreadY) {
	ofunc(_this, edx, vecSource, pangShootView, flDistance, flPenetration, nPenetrationCount, nBulletType, iDamage, flRangeModifier, pAttacker, bDoEffects, flSpreadX, flSpreadY);
}