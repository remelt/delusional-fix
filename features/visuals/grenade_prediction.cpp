#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../../hooks/hooks.hpp"

enum nade_throw_act {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

void features::grenade_prediction::predict(c_usercmd* ucmd) {
	constexpr float restitution = 0.45f;
	constexpr float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{ }, grenade_act{ 1 };
	vec3_t pos, thrown_direction, start, eye_origin;
	vec3_t angles, thrown;

	static auto sv_gravity = interfaces::console->get_convar("sv_gravity");

	gravity = sv_gravity->get_float() / 8.0f;
	step = interfaces::globals->interval_per_tick;

	eye_origin = g::local->get_eye_pos();
	angles = ucmd->view_angles;

	thrown = angles;

	if (thrown.x < 0) {
		thrown.x = -10 + thrown.x * ((90 - 10) / 90.0f);
	}
	else {
		thrown.x = -10 + thrown.x * ((90 + 10) / 90.0f);
	}

	//	find out how we're throwing the grenade
	auto primary_attack = ucmd->buttons & in_attack;
	auto secondary_attack = ucmd->buttons & in_attack2;

	if (primary_attack && secondary_attack) {
		grenade_act = ACT_LOB;
	}
	else if (secondary_attack) {
		grenade_act = ACT_DROP;
	}

	unk01 = power[grenade_act];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	math::angle_vectors(thrown, &thrown_direction); //

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = (thrown_direction * new_velocity) + g::local->velocity();

	for (auto time = 0.0f; index < 500; time += step) {
		pos = start + thrown_direction * step;

		trace_t trace;
		trace_filter filter;
		filter.skip = g::local;
		ray_t ray;
		ray.initialize(start, pos);

		interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);

		//	modify path if we have hit something
		if (trace.flFraction != 1.0f) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot_product(trace.plane.normal) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += (step * (1.0f - trace.flFraction));
		}

		auto detonate = detonated(reinterpret_cast<weapon_t*>(g::local->active_weapon()), time, trace);

		//	emplace nade point
		_points.at(index++) = c_nadepoint(start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate);
		start = pos;

		//	apply gravity modifier
		thrown_direction.z -= gravity * trace.flFraction * step;

		if (detonate) {
			break;
		}
	}

	//	invalidate all empty points and finish prediction
	for (auto n = index; n < 500; ++n) {
		_points.at(n).m_valid = false;
	}

	_predicted = true;
}

bool features::grenade_prediction::detonated(weapon_t* weapon, float time, trace_t& trace) {
	if (!weapon) {
		return true;
	}

	const auto index = weapon->item_definition_index();

	switch (index) {
		//	flash and HE grenades only live up to 2.5s after thrown
	case 43:
	case 44:
		if (time > 2.5f) {
			return true;
		}
		break;

		//	fire grenades detonate on ground hit, or 3.5s after thrown
	case WEAPON_MOLOTOV:
	case 48:
		if (trace.fractionLeftSolid != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f) {
			return true;
		}
		break;

		//	decoy and smoke grenades were buggy in prediction, so i used this ghetto work-around
	case WEAPON_DECOY:
	case 45:
		if (time > 2.5f) {
			return true;
		}
		break;
	}

	return false;
}

void features::grenade_prediction::run(c_usercmd* ucmd) {
	if (!c::misc::nadepred)
		return;

	if (!(ucmd->buttons & in_attack) && !(ucmd->buttons & in_attack2)) {
		_predicted = false;
		return;
	}

	const static std::vector< int > nades{ WEAPON_FLASHBANG, WEAPON_SMOKEGRENADE, WEAPON_HEGRENADE, WEAPON_MOLOTOV, WEAPON_DECOY, WEAPON_INCGRENADE };

	auto weapon = g::local->active_weapon();

	if (!weapon) {
		return;
	}

	if (std::find(nades.begin(), nades.end(), weapon->item_definition_index()) != nades.end()) {
		return predict(ucmd);
	}

	_predicted = false;
}

void features::grenade_prediction::draw() {
	if (!c::misc::nadepred)
		return;

	if (!interfaces::engine->is_in_game() || !g::local || !g::local->is_alive())
		return;

	vec3_t start;
	vec3_t end;

	if (_predicted) {
		for (auto& p : _points) {
			if (!p.m_valid) {
				break;
			}

			if (interfaces::debug_overlay->world_to_screen(p.m_start, start) && interfaces::debug_overlay->world_to_screen(p.m_end, end)) {
				im_render.drawline(start.x, start.y, end.x, end.y, color_t(c::misc::nadepred_clr[0], c::misc::nadepred_clr[1], c::misc::nadepred_clr[2]), 2.f);
			}
		}
	}
}