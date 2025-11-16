#include "aimbot.hpp"
#include "../../sdk/math/math.hpp"
#include "../../menu/config/config.hpp"
#include "../misc/misc.hpp"
#include "autowall.hpp"

std::vector<int> hitboxes;
int hitbox;
float smooth;
int rcs_p;
int aimbot_fov;
int autowall_dmg;
bool autowall_lethal;
bool autowall_b;
bool silent;
bool rcs;
bool katai = true;

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

void get_aimbot_settings(void* weapon) {
	static int hitbox_num;

	if (is_pistol(weapon))
	{
		aimbot_fov = c::aimbot::pistol_aimbot_fov;
		silent = c::aimbot::pistol_aimbot_silent;
		smooth = c::aimbot::pistol_aimbot_smooth;
		rcs = c::aimbot::pistol_aimbot_rcs;
		rcs_p = c::aimbot::pistol_aimbot_rcs_p;
		autowall_b = c::aimbot::pistol_autowall;
		autowall_dmg = c::aimbot::pistol_autowall_dmg;
		autowall_lethal = c::aimbot::pistol_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_pistol);
	}
	else if (is_heavy_pistol(weapon)) {
		aimbot_fov = c::aimbot::heavy_pistol_aimbot_fov;
		silent = c::aimbot::heavy_pistol_aimbot_silent;
		smooth = c::aimbot::heavy_pistol_aimbot_smooth;
		rcs = c::aimbot::heavy_pistol_aimbot_rcs;
		rcs_p = c::aimbot::heavy_pistol_aimbot_rcs_p;
		autowall_b = c::aimbot::heavy_pistol_autowall;
		autowall_dmg = c::aimbot::heavy_pistol_autowall_dmg;
		autowall_lethal = c::aimbot::heavy_pistol_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_heavy_pistol);
	}
	else if (is_shotgun(weapon)) {
		aimbot_fov = c::aimbot::shotgun_aimbot_fov;
		silent = c::aimbot::shotgun_aimbot_silent;
		smooth = c::aimbot::shotgun_aimbot_smooth;
		rcs = c::aimbot::shotgun_aimbot_rcs;
		rcs_p = c::aimbot::shotgun_aimbot_rcs_p;
		autowall_b = c::aimbot::shotgun_autowall;
		autowall_dmg = c::aimbot::shotgun_autowall_dmg;
		autowall_lethal = c::aimbot::shotgun_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_shotgun);
	}
	else if (is_heavy(weapon)) {
		aimbot_fov = c::aimbot::heavy_aimbot_fov;
		silent = c::aimbot::heavy_aimbot_silent;
		smooth = c::aimbot::heavy_aimbot_smooth;
		rcs = c::aimbot::heavy_aimbot_rcs;
		rcs_p = c::aimbot::heavy_aimbot_rcs_p;
		autowall_b = c::aimbot::heavy_autowall;
		autowall_dmg = c::aimbot::heavy_autowall_dmg;
		autowall_lethal = c::aimbot::heavy_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_heavy);
	}
	else if (is_smg(weapon)) {
		aimbot_fov = c::aimbot::smg_aimbot_fov;
		silent = c::aimbot::smg_aimbot_silent;
		smooth = c::aimbot::smg_aimbot_smooth;
		rcs = c::aimbot::smg_aimbot_rcs;
		rcs_p = c::aimbot::smg_aimbot_rcs_p;
		autowall_b = c::aimbot::smg_autowall;
		autowall_dmg = c::aimbot::smg_autowall_dmg;
		autowall_lethal = c::aimbot::smg_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_smg);
	}
	else if (is_rifle(weapon)) {
		aimbot_fov = c::aimbot::rifle_aimbot_fov;
		silent = c::aimbot::rifle_aimbot_silent;
		smooth = c::aimbot::rifle_aimbot_smooth;
		rcs = c::aimbot::rifle_aimbot_rcs;
		rcs_p = c::aimbot::rifle_aimbot_rcs_p;
		autowall_b = c::aimbot::rifle_autowall;
		autowall_dmg = c::aimbot::rifle_autowall_dmg;
		autowall_lethal = c::aimbot::rifle_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_rifle);
	}
	else if (is_sniper(weapon)) {
		aimbot_fov = c::aimbot::sniper_aimbot_fov;
		silent = c::aimbot::sniper_aimbot_silent;
		smooth = c::aimbot::sniper_aimbot_smooth;
		rcs = c::aimbot::sniper_aimbot_rcs;
		rcs_p = c::aimbot::sniper_aimbot_rcs_p;
		autowall_b = c::aimbot::sniper_autowall;
		autowall_dmg = c::aimbot::sniper_autowall_dmg;
		autowall_lethal = c::aimbot::sniper_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_sniper);
	}
	else if (is_auto_sniper(weapon)) {
		aimbot_fov = c::aimbot::autosniper_aimbot_fov;
		silent = c::aimbot::autosniper_aimbot_silent;
		smooth = c::aimbot::autosniper_aimbot_smooth;
		rcs = c::aimbot::autosniper_aimbot_rcs;
		rcs_p = c::aimbot::autosniper_aimbot_rcs_p;
		autowall_b = c::aimbot::autosniper_autowall;
		autowall_dmg = c::aimbot::autosniper_autowall_dmg;
		autowall_lethal = c::aimbot::autosniper_autowall_lethal;
		hitboxes = get_selected_hitboxes(c::aimbot::hitboxes_autosniper);
	}
}

vec3_t calculateRelativeAngle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) noexcept
{
	vec3_t delta = destination - source;
	vec3_t angles{ math::rad2deg(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x,
				   math::rad2deg(atan2f(delta.y, delta.x)) - viewAngles.y ,
					0.f };
	angles.normalize();
	return angles;
}

bool can_fire(weapon_t* weap, c_usercmd* cmd) {
	float flServerTime = (float)(g::local->get_tick_base() * interfaces::globals->interval_per_tick);

	//!!!
	if (g::local->next_attack() <= flServerTime) {
		return (weap->next_primary_attack() <= flServerTime);
	}
	return false;
}

void MegaPrivateAimbotOtAntohi::run(c_usercmd* cmd)
{

	if (!c::aimbot::aimbot || !g::local->is_alive() || !interfaces::engine->is_connected())
		return;

	if (c::aimbot::aimbot_panic && menu::checkkey(c::aimbot::aimbot_panic_key, c::aimbot::aimbot_panic_key_s)) {
		if (katai) {
			features::misc::notify(("aimbot is off"), color(255, 255, 255, 255));
			katai = false;
		}
		return;
	}
	if (!katai) {
		features::misc::notify(("aimbot is on"), color(255, 255, 255, 255));
		katai = true;
	}

	auto pukolki = g::local->active_weapon();
	if (!pukolki)
		return;
	if (!menu::open) {
		if (is_pistol(pukolki)) {
			menu::weapon_selection = 0;
		}
		else if (is_heavy_pistol(pukolki)) {
			menu::weapon_selection = 1;
		}
		else if (is_shotgun(pukolki)) {
			menu::weapon_selection = 2;
		}
		else if (is_heavy(pukolki)) {
			menu::weapon_selection = 3;
		}
		else if (is_smg(pukolki)) {
			menu::weapon_selection = 4;
		}
		else if (is_rifle(pukolki)) {
			menu::weapon_selection = 5;
		}
		else if (is_sniper(pukolki)) {
			menu::weapon_selection = 6;
		}
		else if (is_auto_sniper(pukolki)) {
			menu::weapon_selection = 7;
		}
	}
	if (!GetAsyncKeyState(c::aimbot::aimbot_key))
		return;
	if (menu::open)
		return;
	if (!is_rifle(pukolki) && !is_shotgun(pukolki) && !is_heavy_pistol(pukolki) && !is_auto_sniper(pukolki) && !is_pistol(pukolki) && !is_sniper(pukolki) && !is_smg(pukolki) && !is_heavy(pukolki))
		return;

	get_aimbot_settings(pukolki);
	if (smooth == 0) {
		smooth = 1;
	}
	if (silent) {
		smooth = 1;
	}
	if (hitboxes.empty()) {
		hitboxes.emplace_back(hitbox_head);
	}
	if (!(cmd->buttons & in_attack) && c::aimbot::non_sticky_aimbot)
		return;
	if (!can_fire(pukolki, cmd))
		return;
	if (pukolki->clip1_count() <= 0) {
		return;
	}

	float typofovchik = aimbot_fov;
	vec3_t NuKudastrealyat = vec3_t(0, 0, 0);
	vec3_t NashiOkiEpta = g::local->get_eye_pos();

	const auto aimPunch = g::local->aim_punch_angle();
	float ebuchiirngexploit = interfaces::console->get_convar("weapon_recoil_scale")->get_float();
	static vec3_t vAngle;
	static vec3_t vClientViewAngles;
	interfaces::engine->get_view_angles(vClientViewAngles);
	anglevec3_ts(vClientViewAngles, vAngle);
	for (auto i = 1; i <= interfaces::globals->max_clients; i++)
	{
		player_t* eblanchik = player_t::get_player_by_index(i);
		if (!eblanchik || eblanchik == g::local || eblanchik->dormant() || !eblanchik->is_alive() || eblanchik->has_gun_game_immunity())
			continue;
		if (eblanchik->team() == g::local->team())
			continue;
		if (!eblanchik->is_alive())
			continue;
		auto localPlayerEyePosition = g::local->get_eye_pos();
		for (const auto AhuetYapernul : hitboxes)
		{

			vec3_t GdeKostyanchik = eblanchik->get_hitbox_position(AhuetYapernul);
			const auto angle = calculateRelativeAngle(NashiOkiEpta, GdeKostyanchik, cmd->view_angles + aimPunch);
			const auto fovss = std::hypot(angle.y, angle.x);

			if (fovss > typofovchik)
				continue;

			if (!g::local->can_see_player_pos(eblanchik, GdeKostyanchik))
			{

				//const auto damage = c_autowall::get()->autowall(g::local->GetEyePos(), hitboxPos, g::local, player).damage;
				const auto damage = Autowall::GetDamage(GdeKostyanchik);

				if (damage < autowall_dmg && !autowall_lethal || !autowall_b || autowall_lethal && damage < eblanchik->health())
					continue;
			}
			if (pukolki->is_knife())
				continue;
			vec3_t NenuzhnayaZalupaDlyaFov = calculateRelativeAngle(NashiOkiEpta, GdeKostyanchik, cmd->view_angles + aimPunch);
			float avotetouzhenuzhno = GetFov(NashiOkiEpta, GdeKostyanchik, vAngle);
			if (fovss < typofovchik)
			{
				// KOSTYAN DETECTED
				typofovchik = fovss;
				NuKudastrealyat = GdeKostyanchik;
			}

			if (c::backtrack::backtrack && c::aimbot::aim_at_bt) {
				int bone_id = 8;
				auto& record = backtrack.data[i];

				if (auto modelStudio = interfaces::model_info->get_studio_model(eblanchik->model()); modelStudio != nullptr) {
					auto hitboxSet = modelStudio->hitbox_set(0);

					if (hitboxSet != nullptr) {
						auto hitbox_ = hitboxSet->hitbox(AhuetYapernul);

						if (hitbox_ != nullptr) {
							bone_id = hitbox_->bone;
						}
					}
				}

				float best_record = FLT_MAX;
				int best_index = -1;
				vec3_t position;

				for (int j = 0; j < static_cast<int>(record.size()) - 2; j++) {
					auto& a = record[j];
					auto angle_a = math::calculate_angle(g::local->get_eye_pos(), a.m_matrix[bone_id].get_origin(), cmd->view_angles);
					auto hypot_a = angle_a.length_2d();

					if (hypot_a < best_record) {
						best_record = hypot_a;
						best_index = j;
					}
				}

				if (best_index != -1) {
					position = record[best_index].m_matrix[bone_id].get_origin();
				}
				else {
					position = eblanchik->get_hitbox_position(AhuetYapernul);
				}

				if (!g::local->can_see_player_pos(eblanchik, position))
				{
					float ebatshasvpenu = Autowall::GetDamage(position);
					if (ebatshasvpenu < autowall_dmg && !autowall_lethal || !autowall_b || autowall_lethal && ebatshasvpenu < eblanchik->health())
						continue;
				}
				NuKudastrealyat = position;
			}
		}
	}

	if (NuKudastrealyat.wtf())
		return;

	vec3_t NeNushasBuduebat = math::Vpenivatel(NashiOkiEpta, NuKudastrealyat, cmd->view_angles).normalized();

	auto ZV = float(rcs_p) / 50.f;
	if (rcs)
	{
		cmd->view_angles += (NeNushasBuduebat.clamped() - g::local->aim_punch_angle() * ZV) / smooth;
	}
	else
	{
		cmd->view_angles += (NeNushasBuduebat.clamped()) / smooth;
	}

	if (!silent)
	{
		interfaces::engine->set_view_angles(cmd->view_angles);
	}

	auto weapon = g::local->active_weapon();
	auto weapon_data = weapon->get_weapon_data();

}

MegaPrivateAimbotOtAntohi g_Aimbot;