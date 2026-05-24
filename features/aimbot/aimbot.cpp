#include "aimbot.hpp"
#include "autowall.hpp"
#include "../../sdk/math/math.hpp"
#include "../../menu/config/config.hpp"
#include "../misc/misc.hpp"

bool is_knife(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_KNIFE, WEAPON_KNIFEGG, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GHOST, WEAPON_KNIFE_GUT, WEAPON_KNIFE_GYPSY_JACKKNIFE, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_STILETTO, WEAPON_KNIFE_T, WEAPON_KNIFE_TACTICAL, WEAPON_KNIFE_URSUS, WEAPON_KNIFE_WIDOWMAKER, WEAPON_KNIFEGG };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_grenade(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_FRAG_GRENADE, WEAPON_HEGRENADE, WEAPON_INCGRENADE, WEAPON_SMOKEGRENADE, WEAPON_TAGRENADE, WEAPON_MOLOTOV , WEAPON_DECOY };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_bomb(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_C4 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_pistol(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_GLOCK, WEAPON_ELITE, WEAPON_P250, WEAPON_TEC9, WEAPON_CZ75A, WEAPON_USP_SILENCER, WEAPON_HKP2000, WEAPON_FIVESEVEN };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_heavy_pistol(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_DEAGLE, WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_shotgun(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_NOVA, WEAPON_XM1014, WEAPON_SAWEDOFF, WEAPON_MAG7 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_heavy(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_M249, WEAPON_NEGEV };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_smg(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_MAC10, WEAPON_MP7, WEAPON_UMP45, WEAPON_P90, WEAPON_BIZON, WEAPON_MP9, WEAPON_MP5SD };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_rifle(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_AK47,WEAPON_AUG,WEAPON_FAMAS,WEAPON_GALILAR,WEAPON_M4A1,WEAPON_M4A1_SILENCER, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_sniper(void* weapon) {
	if (!weapon)
		return false;


	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_AWP, WEAPON_SSG08 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool is_auto_sniper(void* weapon) {
	if (!weapon)
		return false;

	auto current_weapon = reinterpret_cast<weapon_t*>(weapon);
	int id = current_weapon->item_definition_index();
	static const std::vector<int> v = { WEAPON_G3SG1, WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

template<size_t N>
std::vector<int> get_selected_hitboxes(const bool(&hitboxes)[N]) {
	std::vector<int> selected_hitboxes;

	if (N > 0 && hitboxes[0]) selected_hitboxes.emplace_back(hitbox_head);
	if (N > 1 && hitboxes[1]) selected_hitboxes.emplace_back(hitbox_neck);
	if (N > 2 && hitboxes[2]) selected_hitboxes.emplace_back(hitbox_chest);
	if (N > 3 && hitboxes[3]) selected_hitboxes.emplace_back(hitbox_pelvis);

	return selected_hitboxes;
}

bool get_aimbot_settings(aimbot_settings& settings, std::vector<int>& hitboxes, void* weapon) {
	const bool groups[] = { is_pistol(weapon), is_heavy_pistol(weapon), is_shotgun(weapon), is_heavy(weapon), is_smg(weapon), is_rifle(weapon), is_sniper(weapon), is_auto_sniper(weapon) };

	for (int i = 0; i < IM_ARRAYSIZE(groups); i++) {
		if (groups[i]) {
			settings = aimbot.settings[i];
			menu::weapon_selection = i;

			hitboxes = get_selected_hitboxes(settings.hitboxes);
			if (hitboxes.empty()) {
				hitboxes.emplace_back(hitbox_head);
			}

			return true;
		}
	}
	return false;
}

bool can_fire(weapon_t* weap, c_usercmd* cmd) {
	float flServerTime = (float)(g::local->get_tick_base() * interfaces::globals->interval_per_tick);

	//!!!
	if (g::local->next_attack() <= flServerTime) {
		return (weap->next_primary_attack() <= flServerTime);
	}
	return false;
}

void aimbot_c::run(c_usercmd* cmd)
{
	if (!c::aimbot::aimbot || !g::local->is_alive() || !interfaces::engine->is_connected())
		return;

	auto weapon = g::local->active_weapon();
	if (!weapon)
		return;

	// ugly2
	static bool once = true;
	if (c::aimbot::aimbot_panic && menu::checkkey(c::aimbot::aimbot_panic_key, c::aimbot::aimbot_panic_key_s)) {
		if (once) {
			features::misc::notify(("aimbot is off"), color(255, 255, 255, 255));
			once = false;
		}
		return;
	}
	if (!once) {
		features::misc::notify(("aimbot is on"), color(255, 255, 255, 255));
		once = true;
	}

	if (menu::open)
		return;

	aimbot_settings settings;
	std::vector<int> hitboxes;
	if (!get_aimbot_settings(settings, hitboxes, weapon))
		return;

	// ugly
	float best_fov = settings.fov;
	if (settings.fov == 180) {
		best_fov = FLT_MAX;
	}

	if (!menu::checkkey(c::aimbot::aimbot_key, c::aimbot::aimbot_key_s))
		return;

	if (!(cmd->buttons & in_attack) && c::aimbot::non_sticky_aimbot)
		return;

	if (!can_fire(weapon, cmd))
		return;

	if (weapon->clip1_count() <= 0)
		return;

	vec3_t final_position = vec3_t(0, 0, 0);
	vec3_t client_eye_pos = g::local->get_eye_pos();

	const auto aim_punch = g::local->aim_punch_angle();
	auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();

	vec3_t vAngle;
	vec3_t client_view_angles;

	client_view_angles = cmd->view_angles;
	AngleVectors(client_view_angles, vAngle);

	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		player_t* target = player_t::get_player_by_index(i);
		if (!target || target == g::local || target->dormant() || !target->is_alive() || target->has_gun_game_immunity()|| target->team() == g::local->team() || !target->is_alive())
			continue;

		target->pvs_fix();

		for (const auto hitbox : hitboxes)
		{
			vec3_t target_hitbox_position = target->get_hitbox_position(hitbox);
			const auto angle = GetFov(client_eye_pos, target_hitbox_position, vAngle);

			if (c::backtrack::backtrack && c::aimbot::aim_at_bt) {
				int bone_id = 8;
				auto& record = backtrack.data[i];

				if (auto modelStudio = interfaces::model_info->get_studio_model(target->model()); modelStudio != nullptr) {
					auto hitboxSet = modelStudio->hitbox_set(0);

					if (hitboxSet != nullptr) {
						auto hitbox_ = hitboxSet->hitbox(hitbox);

						if (hitbox_ != nullptr) {
							bone_id = hitbox_->bone;
						}
					}
				}

				for (int j = 0; j < static_cast<int>(record.size()) - 2; j++) {
					auto& a = record[j];
					vec3_t target_hitbox_position_bt = a.m_matrix[bone_id].get_origin();
					const auto bt_angle = GetFov(client_eye_pos, target_hitbox_position_bt, vAngle);

					if (!g::local->can_see_player_pos(target_hitbox_position_bt))
					{
						// doesnt work properly
						// should be fixed
						float damage = Autowall::GetDamage(target_hitbox_position_bt);
						if (damage < settings.autowall_dmg && !settings.autowall_lethal || !settings.autowall_b || settings.autowall_lethal && damage < target->health())
							continue;
					}

					if (bt_angle < best_fov) {
						best_fov = bt_angle;
						final_position = target_hitbox_position_bt;
					}
				}
			}

			if (!g::local->can_see_player_pos(target, target_hitbox_position))
			{
				const auto damage = Autowall::GetDamage(target_hitbox_position);
				if (damage < settings.autowall_dmg && !settings.autowall_lethal || !settings.autowall_b || settings.autowall_lethal && damage < target->health())
					continue;
			}

			if (angle < best_fov)
			{
				best_fov = angle;
				final_position = target_hitbox_position;
			}
		}
	}

	if (final_position.null())
		return;

	const vec3_t aim_angles = CalcAngle(client_eye_pos, final_position);
	const auto rcs_value = float(settings.rcs_p) / 100.f;

	if (aim_angles.null())
		return;

	cmd->view_angles = aim_angles;

	if (settings.rcs) // move it lower if u dont want smooth to be applied to rcs
	{
		cmd->view_angles = cmd->view_angles - (aim_punch * weapon_recoil_scale * rcs_value);
	}
	if (settings.smooth > 0) // too basic, too bad, but who uses it anyways ?
	{
		client_view_angles.x = AngleNormalize(client_view_angles.x);
		client_view_angles.y = AngleNormalize(client_view_angles.y);

		vec3_t qDelta = cmd->view_angles - client_view_angles;

		qDelta.x = AngleNormalize(qDelta.x);
		qDelta.y = AngleNormalize(qDelta.y);

		cmd->view_angles.x = client_view_angles.x + qDelta.x / (float)settings.smooth;
		cmd->view_angles.y = client_view_angles.y + qDelta.y / (float)settings.smooth;

		cmd->view_angles.x = AngleNormalize(cmd->view_angles.x);
		cmd->view_angles.y = AngleNormalize(cmd->view_angles.y);
	}
	if (!settings.silent)
	{
		interfaces::engine->set_view_angles(cmd->view_angles);
	}

#ifdef _DEBUG
	interfaces::console->console_printf("Hitbox: x = %.20f, y = %.20f, z = %.20f\n", final_position.x, final_position.y, final_position.z);
	interfaces::console->console_printf("Angles: x = %.20f, y = %.20f, z = %.20f\n", aim_angles.x, aim_angles.y, aim_angles.z);
#endif
}

aimbot_c aimbot;