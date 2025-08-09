#include "aimbot.hpp"

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

void TraceLine(vec3_t& vecAbsStart, vec3_t& vecAbsEnd, unsigned int mask, player_t* ignore, trace_t* ptr)
{
	trace_filter filter;
	filter.skip = g::local;
	ray_t ray;
	ray.initialize(vecAbsStart, vecAbsEnd);
	interfaces::trace_ray->trace_ray(ray, mask, &filter, ptr);
}

bool VectortoVectorVisible(vec3_t src, vec3_t point, player_t* p_entity = nullptr)
{
	trace_t TraceInit;
	TraceInit.entity = nullptr;
	trace_filter filter1;
	filter1.skip = g::local;
	ray_t ray;
	ray.initialize(src, point);
	interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter1, &TraceInit);

	if (TraceInit.flFraction == 1.0f)
		return true;

	if (p_entity != nullptr && TraceInit.entity == p_entity)
		return true;

	return false;

}

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, player_t* enemy, float weapon_armor_ratio, float& current_damage)
{
	bool   has_heavy_armor = enemy->has_heavy_armor();
	int    armor = enemy->armor();
	int    hit_group = hitgroup;

	auto armored = [&]() -> bool {
		player_t* target = enemy;

		switch (hitgroup) {
		case HITGROUP_HEAD:
			return target->has_helmet();
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch (hit_group) {
	case HITGROUP_HEAD:
		current_damage *= has_heavy_armor ? 2.f : 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	default:
		break;
	}

	if (armor > 0 && armored()) {
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f, armor_ratio = weapon_armor_ratio / 2.f;

		if (has_heavy_armor) {
			armor_bonus_ratio = 0.33f;
			armor_ratio *= 0.5f;
			bonus_value = 0.33f;
		}

		auto new_damage = current_damage * armor_ratio;

		if (has_heavy_armor) {
			new_damage *= 0.85f;
		}

		if (((current_damage - (current_damage * armor_ratio)) * (bonus_value * armor_bonus_ratio)) > armor) {
			new_damage = current_damage - (armor / armor_bonus_ratio);
		}

		current_damage = new_damage;
	}

}

void UTIL_ClipTraceToPlayers(const vec3_t& vecAbsStart, const vec3_t& vecAbsEnd, unsigned int mask, trace_filter* filter, trace_t* tr)
{
	static auto dwAddress = find_pattern("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool Breakable(player_t* p_entity) {
	static auto	ptr_instruction = find_pattern("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE");

	using Fn = bool(__fastcall*)(player_t*);
	static auto fn = reinterpret_cast<Fn>(ptr_instruction);

	if (!p_entity || !p_entity->index()) {
		return false;
	}

	//  m_takeDamage isn't properly set when using the signature.
	//  back it up, set it to true, then restore.
	auto take_damage{ (char*)((uintptr_t)p_entity + *(size_t*)((uintptr_t)fn + 38)) };
	auto take_damage_backup{ *take_damage };

	auto* cclass = interfaces::client->get_all_classes();

	//				 '       ''     '      '   '
	//			    01234567890123456     012345678
	//  check against CBreakableSurface and CBaseDoor

	//  windows etc. are CBrekableSurface
	//  large garage door in office is CBaseDoor and it get's reported as a breakable when it is not one
	//  this is seperate from "CPropDoorRotating", which is a normal door.
	//  normally you would also check for "CFuncBrush" but it was acting oddly
	//  ((cclass->m_pNetworkName[1]) != 'F' || (cclass->m_pNetworkName[4]) != 'c' || (cclass->m_pNetworkName[5]) != 'B' || (cclass->m_pNetworkName[9]) != 'h')

	if ((cclass->network_name[1] == 'B' && cclass->network_name[9] == 'e' && cclass->network_name[10] == 'S' && cclass->network_name[16] == 'e')
		|| (cclass->network_name[1] != 'B' || cclass->network_name[5] != 'D'))
		*take_damage = 2;

	bool breakable = fn(p_entity);
	*take_damage = take_damage_backup;

	return breakable;
}

bool TraceToExit(trace_t& enter_trace, trace_t& exit_trace, vec3_t start_position, vec3_t dir) {
	vec3_t   start, end;
	float  max_distance = 90.f,
		ray_extension = 4.f,
		current_distance = 0;
	int    first_contents = 0;

	while (current_distance <= max_distance) {
		//  add extra distance to our ray
		current_distance += ray_extension;

		//  multiply the direction vector to the distance so we go outwards, add our position to it.
		start = start_position + dir * current_distance;

		if (!first_contents) {
			first_contents = interfaces::trace_ray->get_point_contents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr); /*0x4600400B*/
		}

		int point_contents = interfaces::trace_ray->get_point_contents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(point_contents & MASK_SHOT_HULL) || point_contents & CONTENTS_HITBOX && point_contents != first_contents) /*0x600400B, *0x40000000*/ {

			//  let's setup our end position by deducting the direction by the extra added distance
			end = start - (dir * ray_extension);

			//  let's cast a ray from our start pos to the end pos
			TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr, &exit_trace);

			//  let's check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exit_trace.startSolid && exit_trace.surface.flags & SURF_HITBOX) {
				TraceLine(start, start_position, MASK_SHOT_HULL, exit_trace.entity, &exit_trace);

				if (exit_trace.did_hit() && !exit_trace.startSolid) {
					start = exit_trace.end;

					return true;
				}

				continue;
			}

			//  can we hit? is the wall solid?
			if (exit_trace.did_hit() && !exit_trace.startSolid) {

				// is the wall a breakable? if so, let's shoot through it.
				if (Breakable(enter_trace.entity) && Breakable(exit_trace.entity)) {
					return true;
				}

				if (enter_trace.surface.flags & SURF_NODRAW || !(exit_trace.surface.flags & SURF_NODRAW) && (exit_trace.plane.normal.dot(dir) <= 1.f)) {
					float mult_amount = exit_trace.flFraction * 4.f;

					start -= dir * mult_amount;

					return true;
				}

				continue;
			}

			if (!exit_trace.did_hit() || exit_trace.startSolid) {
				if ((enter_trace.entity != nullptr && enter_trace.entity->index() != 0) && Breakable(enter_trace.entity)) {
					exit_trace = enter_trace;

					exit_trace.end = start + dir;

					return true;
				}

				continue;
			}
		}
	}

	return false;
}

bool HandleBulletPenetration(weapon_info_t* wpn_data, fire_bullet_data& data, bool extracheck, vec3_t point)
{
	trace_t       trace_exit;
	surfacedata_t* enter_surface_data = interfaces::surface_props->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int           enter_material = enter_surface_data->game.material;

	player_t* entity = data.enter_trace.entity;

	float         enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float         final_damage_modifier = 0.f;
	float         combined_penetration_modifier = 0.f;
	bool          solid_surf = ((data.enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool          light_surf = ((data.enter_trace.surface.flags >> 7) & SURF_LIGHT);

	if (data.penetrate_count <= 0
		|| (!data.penetrate_count && !light_surf && !solid_surf && enter_material != CHAR_TEX_GLASS && enter_material != CHAR_TEX_GRATE)
		|| wpn_data->m_flPenetration <= 0.f
		|| !TraceToExit(data.enter_trace, trace_exit, data.enter_trace.end, data.direction)
		&& !(interfaces::trace_ray->get_point_contents(data.enter_trace.end, MASK_SHOT_HULL, nullptr) & (MASK_SHOT_HULL)))
		return false;

	surfacedata_t* exit_surface_data = interfaces::surface_props->GetSurfaceData(trace_exit.surface.surfaceProps);
	int           exit_material = exit_surface_data->game.material;
	float         exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;

	static auto damage_reduction_bullets = interfaces::console->get_convar("ff_damage_reduction_bullets");
	static auto damage_bullet_penetration = interfaces::console->get_convar("ff_damage_bullet_penetration");

	if (enter_material == CHAR_TEX_GLASS || enter_material == CHAR_TEX_GRATE) {
		combined_penetration_modifier = 3.f;
		final_damage_modifier = 0.05f;
	}
	else if (light_surf || solid_surf) {
		combined_penetration_modifier = 1.f;
		final_damage_modifier = 0.16f;
	}
	else if (data.enter_trace.entity && enter_material == CHAR_TEX_FLESH && (g::local->team() == entity->team() && damage_reduction_bullets->get_float() == 0.f)) {
		//  look's like you aren't shooting through your teammate today
		if (damage_bullet_penetration->get_float() == 0.f) {
			return false;
		}

		combined_penetration_modifier = damage_bullet_penetration->get_float();
		final_damage_modifier = 0.16f;
	}

	else {
		combined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) * 0.5f;
		final_damage_modifier = 0.16f;
	}

	//  do our materials line up?
	if (enter_material == exit_material) {
		if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD) {
			combined_penetration_modifier = 3.f;
		}

		else if (exit_material == CHAR_TEX_PLASTIC) {
			combined_penetration_modifier = 2.f;
		}
	}

	//  calculate thickness of the wall by getting the length of the range of the trace and squaring
	float thickness = (trace_exit.end - data.enter_trace.end).length_sqr();

	if (extracheck)
		if (!VectortoVectorVisible(trace_exit.end, point, entity))
			return false;

	float modifier = fmaxf(1.f / combined_penetration_modifier, 0.f);

	//  this calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
	float lost_damage = fmaxf(
		((modifier * thickness) / 24.f) //* 0.041666668
		+ ((data.current_damage * final_damage_modifier)
			+ (fmaxf(3.75 / wpn_data->m_flPenetration, 0.f) * 3.f * modifier)), 0.f);

	//  did we loose too much damage?
	if (lost_damage > data.current_damage) {
		return false;
	}

	//  we can't use any of the damage that we've lost
	if (lost_damage > 0.f) {
		data.current_damage -= lost_damage;
	}

	//  do we still have enough damage to deal?
	if (data.current_damage < 1.f) {
		return false;
	}

	data.src = trace_exit.end;
	--data.penetrate_count;

	return true;

}

bool SimulateFireBullet(player_t* local, weapon_t* weapon, fire_bullet_data& data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto* wpn_data = weapon->get_weapon_data();
	data.current_damage = (float)wpn_data->m_iWeaponDamage;
	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_flWeaponRange - data.trace_length;
		vec3_t End_Point = data.src + data.direction * data.trace_length_remaining;
		TraceLine(data.src, End_Point, MASK_SHOT_HULL | CONTENTS_HITBOX, local, &data.enter_trace);
		/*UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		if (data.enter_trace.flFraction == 1.0f)
			break;
		if ((data.enter_trace.hitGroup <= 7) && (data.enter_trace.hitGroup > 0) && (local->team() != data.enter_trace.entity->team()))
		{
			data.trace_length += data.enter_trace.flFraction * data.trace_length_remaining;
			data.current_damage *= pow(wpn_data->m_flRangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitGroup, data.enter_trace.entity, wpn_data->m_flArmorRatio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(wpn_data, data, false, vec3_t(0, 0, 0)))
			break;*/
		data.trace_length += data.enter_trace.flFraction * data.trace_length;

		//  let's make our damage drops off the further away the bullet is.
		data.current_damage *= pow(wpn_data->m_flRangeModifier, (data.trace_length / 500.f));

		//  get surface data
		auto enter_surface_data = interfaces::surface_props->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		auto      enter_pen_mod = enter_surface_data->game.flPenetrationModifier;
		int      enter_material = enter_surface_data->game.material;

		if (data.trace_length > 3000.f || enter_pen_mod < 0.1f) {
			break;
		}

		//  this looks gay as fuck if we put it in to 1 long line of code
		if (!g::local || !data.enter_trace.entity)
			break;
		bool can_do_damage = (data.enter_trace.hit_group != HITGROUP_GEAR && data.enter_trace.hit_group != HITGROUP_GENERIC);
		bool is_enemy = (local->team() != data.enter_trace.entity->team());

		//  check to see if we hit an entity, if so scale the damage
		if ((can_do_damage && is_enemy)) {

			//  scale our damage
			ScaleDamage(data.enter_trace.hit_group, data.enter_trace.entity, wpn_data->m_flArmorRatio, data.current_damage);

			return true;
		}

		//  if the bullet can't penetrate wall we don't want to continue looping
		if (!HandleBulletPenetration(wpn_data, data, false, vec3_t(0, 0, 0))) {
			break;
		}

	}
	return false;
}

float autowall::damage(const vec3_t& point)
{
	auto data = fire_bullet_data(g::local->get_eye_pos(), g::local);

	vec3_t angles;
	angles = math::calc_angle(data.src, point);
	math::angle_vectors(angles, data.direction);
	data.direction.normalize_in_place();

	if (SimulateFireBullet(g::local, g::local->active_weapon(), data))
		return data.current_damage;

	return -1.f;
}

bool autowall::can_hit_floating_point(const vec3_t& point, const vec3_t& source) // ez
{
	if (!g::local)
		return false;

	fire_bullet_data data = fire_bullet_data(source, g::local);

	vec3_t angles = math::calc_angle(data.src, point);
	math::angle_vectors(angles, data.direction);
	data.direction.normalize2();

	weapon_t* pWeapon = (weapon_t*)g::local->active_weapon();

	if (!pWeapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	weapon_info_t* weaponData = pWeapon->get_weapon_data();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->m_iWeaponDamage;
	data.trace_length_remaining = weaponData->m_flWeaponRange - data.trace_length;
	vec3_t end = data.src + (data.direction * data.trace_length_remaining);
	TraceLine(data.src, end, MASK_SHOT | CONTENTS_HITBOX, g::local, &data.enter_trace);

	if (VectortoVectorVisible(data.src, point) || HandleBulletPenetration(weaponData, data, true, point))
		return true;

	return false;
}
