#include "aimbot.hpp"
#include "autowall.hpp"

#define HITGROUP_GENERIC 0
#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3
#define HITGROUP_LEFTARM 4
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG 6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR 10

float Autowall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	switch (iHitGroup) {
	case HITGROUP_GENERIC:
		return 0.5f;
	case HITGROUP_HEAD:
		return 2.0f;
	case HITGROUP_CHEST:
		return 0.5f;
	case HITGROUP_STOMACH:
		return 0.75f;
	case HITGROUP_LEFTARM:
		return 0.5f;
	case HITGROUP_RIGHTARM:
		return 0.5f;
	case HITGROUP_LEFTLEG:
		return 0.375f;
	case HITGROUP_RIGHTLEG:
		return 0.375f;
	case HITGROUP_GEAR:
		return 0.5f;
	default:
		return 1.0f;
	}
}

void Autowall::ClipTraceToPlayers(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, uint32_t mask, trace_filter* filter, trace_t* tr)
{
	float smallestFraction = tr->flFraction;
	constexpr float maxRange = 60.0f;

	vec3_t delta(vecAbsEnd - vecAbsStart);
	const float delta_length = delta.length();
	delta.normalize_in_place();

	ray_t ray;
	ray.initialize(vecAbsStart, vecAbsEnd);

	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {
		auto ent = player_t::get_player_by_index(i);
		if (!ent || ent->dormant() || ent->life_state() != LIFE_ALIVE)
			continue;

		if (filter && !filter->should_hit_entity(ent, mask))
			continue;

		auto collideble = ent->collideable();
		auto mins = collideble->mins();
		auto maxs = collideble->maxs();

		auto obb_center = (maxs + mins) * 0.5f;
		auto extend = (obb_center - vecAbsStart);
		auto rangeAlong = delta.dot(extend);

		trace_filter filter;
		filter.skip = g::local;

		float range;
		if (rangeAlong >= 0.0f) {
			if (rangeAlong <= delta_length)
				range = vec3_t(obb_center - ((delta * rangeAlong) + vecAbsStart)).length();
			else
				range = -(obb_center - vecAbsEnd).length();
		}
		else {
			range = -extend.length();
		}

		if (range >= 0.0f && range <= maxRange) {
			trace_t playerTrace;
			interfaces::trace_ray->ClipRayToEntity(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, ent, &playerTrace);
			if (playerTrace.flFraction < smallestFraction) {
				*tr = playerTrace;
				smallestFraction = playerTrace.flFraction;
			}
		}
	}
}

void Autowall::ScaleDamage(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	static auto mp_damage_scale_ct_head = interfaces::console->get_convar("mp_damage_scale_ct_head");
	static auto mp_damage_scale_t_head = interfaces::console->get_convar("mp_damage_scale_t_head");
	static auto mp_damage_scale_ct_body = interfaces::console->get_convar("mp_damage_scale_ct_body");
	static auto mp_damage_scale_t_body = interfaces::console->get_convar("mp_damage_scale_t_body");

	auto team = enemy->team();
	auto head_scale = team == 2 ? mp_damage_scale_ct_head->get_float() : mp_damage_scale_t_head->get_float();
	auto body_scale = team == 2 ? mp_damage_scale_ct_body->get_float() : mp_damage_scale_t_body->get_float();

	auto armor_heavy = enemy->has_heavy_armor();
	auto armor_value = static_cast<float>(enemy->armor());

	if (armor_heavy)
		head_scale *= 0.5f;

	// ref: CCSPlayer::TraceAttack
	switch (hitgroup) {
	case HITGROUP_HEAD:
		current_damage = (current_damage * 4.f) * head_scale;
		break;
	case HITGROUP_CHEST:
	case 8:
		current_damage *= body_scale;
		break;
	case HITGROUP_STOMACH:
		current_damage = (current_damage * 1.25f) * body_scale;
		break;
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		current_damage *= body_scale;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage = (current_damage * 0.75f) * body_scale;
		break;
	default:
		break;
	}

	static auto IsArmored = [](player_t* player, int hitgroup) {
		auto has_helmet = player->has_helmet();
		auto armor_value = static_cast<float>(player->armor());

		if (armor_value > 0.f) {
			switch (hitgroup) {
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
			case 8:
				return true;
			case HITGROUP_HEAD:
				return has_helmet || player->has_heavy_armor();
			default:
				return player->has_heavy_armor();
			}
		}

		return false;
		};

	if (IsArmored(enemy, hitgroup)) {
		auto armor_scale = 1.f;
		auto armor_ratio = (weapon_armor_ratio * 0.5f);
		auto armor_bonus_ratio = 0.5f;

		if (armor_heavy) {
			armor_ratio *= 0.2f;
			armor_bonus_ratio = 0.33f;
			armor_scale = 0.25f;
		}

		float new_damage = current_damage * armor_ratio;
		float estiminated_damage = (current_damage - (current_damage * armor_ratio)) * (armor_scale * armor_bonus_ratio);
		if (estiminated_damage > armor_value)
			new_damage = (current_damage - (armor_value / armor_bonus_ratio));

		current_damage = new_damage;
	}
}

bool Autowall::TraceToExit(vec3_t& end, trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace)
{
	float distance = 0.0f;
	while (distance <= 90.0f) {
		distance += 4.0f;

		end = start + dir * distance;
		auto point_contents = interfaces::trace_ray->get_point_contents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);
		if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
			continue;

		auto new_end = end - (dir * 4.0f);
		ray_t ray;
		ray.initialize(end, new_end);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, 0, exit_trace);
		if (exit_trace->startSolid && exit_trace->surface.flags & SURF_HITBOX) {
			ray.initialize(end, start);
			trace_filter filter;
			filter.skip = exit_trace->entity;
			interfaces::trace_ray->trace_ray(ray, 0x600400B, &filter, exit_trace);
			if ((exit_trace->flFraction < 1.0f || exit_trace->allsolid) && !exit_trace->startSolid) {
				end = exit_trace->end;
				return true;
			}
			continue;
		}

		if (!(exit_trace->flFraction < 1.0 || exit_trace->allsolid || exit_trace->startSolid) || exit_trace->startSolid) {
			if (exit_trace->entity) {
				if (enter_trace->entity && enter_trace->entity == interfaces::ent_list->get_client_entity(0))
					return true;
			}
			continue;
		}

		if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
			continue;

		if (exit_trace->plane.normal.dot(dir) <= 1.0f) {
			auto fraction = exit_trace->flFraction * 4.0f;
			end = end - (dir * fraction);
			return true;
		}
	}
	return false;
}

bool Autowall::HandleBulletPenetration(weapon_info_t* weaponInfo, FireBulletData& data)
{
	surfacedata_t* enter_surface_data = interfaces::surface_props->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
	data.current_damage *= powf(weaponInfo->m_flRangeModifier, data.trace_length * 0.002f);

	if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	vec3_t dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.end, data.direction, &trace_exit))
		return false;

	surfacedata_t* exit_surface_data = interfaces::surface_props->GetSurfaceData(trace_exit.surface.surfaceProps);
	const int exit_material = exit_surface_data->game.material;
	const float exit_surf_penetration_mod = *(float*)((uint8_t*)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier;
	if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71) {
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

	if (enter_material == exit_material) {
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	const float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	const float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weaponInfo->m_flPenetration) * 1.25f);
	float thickness = (trace_exit.end - data.enter_trace.end).length();

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.end;
	data.penetrate_count--;
	return true;
}

void TraceLine(vec3_t vecAbsStart, vec3_t vecAbsEnd, unsigned int mask, player_t* ignore, trace_t* ptr)
{
	ray_t ray;
	ray.initialize(vecAbsStart, vecAbsEnd);
	trace_filter filter;
	filter.skip = ignore;
	interfaces::trace_ray->trace_ray(ray, mask, &filter, ptr);
}

bool Autowall::SimulateFireBullet(weapon_t* pWeapon, FireBulletData& data)
{
	auto* weaponInfo = pWeapon->get_weapon_data();
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	data.current_damage = (float)weaponInfo->m_iWeaponDamage;
	while (data.penetrate_count > 0 && data.current_damage >= 1.0f) {
		data.trace_length_remaining = weaponInfo->m_flWeaponRange - data.trace_length;
		vec3_t end = data.src + data.direction * data.trace_length_remaining;
		TraceLine(data.src, end, MASK_SHOT, g::local, &data.enter_trace);

		trace_filter local_filter;
		local_filter.skip = g::local;
		ClipTraceToPlayers(data.src, end + data.direction * 40.0f, MASK_SHOT_HULL | CONTENTS_HITBOX, &local_filter, &data.enter_trace);
		if (data.enter_trace.flFraction == 1.0f)
			break;

		if (data.enter_trace.hit_group <= HITGROUP_RIGHTLEG && data.enter_trace.hit_group > HITGROUP_GENERIC) {
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= powf(weaponInfo->m_flRangeModifier, data.trace_length * 0.002f);

			player_t* player = (player_t*)data.enter_trace.entity;
			Autowall::ScaleDamage(data.enter_trace.hit_group, player, weaponInfo->m_flArmorRatio, data.current_damage);
			return true;
		}

		if (!HandleBulletPenetration(weaponInfo, data))
			break;
	}
	return false;
}

float Autowall::GetDamage(const vec3_t& point)
{
	float damage = 0.f;
	vec3_t dst = point;
	FireBulletData data;
	data.src = g::local->get_eye_pos();
	data.filter.skip = g::local;
	data.direction = dst - data.src;
	data.direction.normalize_in_place();
	weapon_t* activeWeapon = (weapon_t*)g::local->active_weapon();
	if (!activeWeapon)
		return -1.0f;
	if (SimulateFireBullet(activeWeapon, data))
		damage = data.current_damage;
	return damage;
}

#define HITGROUP_GENERIC   0
#define HITGROUP_HEAD      1
#define HITGROUP_CHEST     2
#define HITGROUP_STOMACH   3
#define HITGROUP_LEFTARM   4
#define HITGROUP_RIGHTARM  5
#define HITGROUP_LEFTLEG   6
#define HITGROUP_RIGHTLEG  7
#define HITGROUP_GEAR      10

#define HITGROUP_GENERIC    0
#define HITGROUP_HEAD        1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10
#define DAMAGE_NO		0
#define DAMAGE_EVENTS_ONLY	1
#define DAMAGE_YES		2
#define DAMAGE_AIM		3
#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

void ScaleDamage_1(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	int armor = enemy->armor();
	float ratio;

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	}

	if (armor > 0)
	{
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			if (enemy->has_helmet())
			{
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
				{
					ratio = current_damage - (armor * 2.0);
				}
				current_damage = ratio;
			}
			break;
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			ratio = (weapon_armor_ratio * 0.5) * current_damage;
			if (((current_damage - ratio) * 0.5) > armor)
			{
				ratio = current_damage - (armor * 2.0);
			}
			current_damage = ratio;
			break;
		}
	}
}