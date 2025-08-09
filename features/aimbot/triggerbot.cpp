#include "aimbot.hpp"
#include "../../sdk/math/math.hpp"
#include "../../menu/config/config.hpp"

void triggerbot::run(c_usercmd* cmd) {
	if (!interfaces::engine->is_in_game() || !g::local->is_alive()) 
		return;

	auto weapon = g::local->active_weapon();
	if (!weapon || weapon->clip1_count() <= 0) return;

	auto weapon_data = weapon->get_weapon_data();
	if (!weapon_data) return;

	if (weapon->next_primary_attack() > g::local->get_tick_base() * interfaces::globals->interval_per_tick) return;

	if (menu::open) 
		return;

	if (!c::triggerbot::enabled || !menu::checkkey(c::triggerbot::triggerbot_key, c::triggerbot::triggerbot_key_s))
		return;

	vec3_t src, dst, forward;
	trace_t trace;
	ray_t ray;
	trace_filter filter;

	math::angle_vectors(cmd->view_angles + g::local->aim_punch_angle(), &forward);
	forward *= weapon->get_weapon_data()->m_flWeaponRange;
	filter.skip = g::local;
	src = g::local->get_eye_pos();
	dst = src + forward;

	ray.initialize(src, dst);
	interfaces::trace_ray->trace_ray(ray, 0x46004003, &filter, &trace);

	if (!trace.entity || !trace.entity->is_alive() || trace.entity->team() == g::local->team() || trace.entity->has_gun_game_immunity() || trace.entity->dormant() || !trace.entity->is_player())
		return;

	if (trace.hit_group == hitgroup_head || trace.hit_group == hitgroup_chest || trace.hit_group == hitgroup_stomach) {
		cmd->buttons |= in_attack;
	}
}