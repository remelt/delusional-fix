#include "skins.hpp"
#include "../../sdk/sdk.hpp"
#include "../../menu/config/config.hpp"
#include "../visuals/visuals.hpp"

static auto get_wearable_create_fn() -> create_client_class_fn {
	auto client_class = interfaces::client->get_all_classes();
	for (client_class = interfaces::client->get_all_classes();
		client_class; client_class = client_class->next_ptr) {

		if (client_class->class_id == (int)class_ids::ceconwearable)
			return client_class->create_fn;
	}
}

static auto make_glove(int entry, int serial) -> attributable_item_t* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = find_pattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool apply_glove_model(attributable_item_t* glove, const char* model) noexcept {
	player_info_t info;
	interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &info);
	glove->acc_id() = info.xuidlow;
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;

	return true;
}

bool apply_glove_skin(attributable_item_t* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept {
	glove->item_definition_index() = item_definition_index;
	glove->fallback_paint_kit() = paint_kit;
	glove->set_model_index(model_index);
	glove->entity_quality() = entity_quality;
	glove->fallback_wear() = fallback_wear;

	return true;
}


bool apply_knife_model(attributable_item_t* weapon, const char* model) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player) return false;

	auto viewmodel = reinterpret_cast<base_view_model*>(interfaces::ent_list->get_client_entity_handle(local_player->view_model()));
	if (!viewmodel) return false;

	auto h_view_model_weapon = viewmodel->m_hweapon();
	if (!h_view_model_weapon) return false;

	auto view_model_weapon = reinterpret_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity_handle(h_view_model_weapon));
	if (view_model_weapon != weapon) return false;

	viewmodel->model_index() = interfaces::model_info->get_model_index(model);

	auto world_model_handle = view_model_weapon->world_model_handle();
	if (!world_model_handle) return false;

	const auto world_model = reinterpret_cast<base_view_model*>(interfaces::ent_list->get_client_entity_handle(world_model_handle));
	if (!world_model) return false;
	world_model->model_index() = interfaces::model_info->get_model_index(model) + 1;

	if (strstr(model, "knife_gg") != nullptr) {
		auto team = local_player->team();
		viewmodel->m_nBody() = (team == 2) ? 0 : 1;    //if we t == knife_body_t, if ct == knife_body_ct
	}

	return true;
}

bool apply_knife_skin(attributable_item_t* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) {
	weapon->item_definition_index() = item_definition_index;
	weapon->fallback_paint_kit() = paint_kit;
	weapon->model_index() = model_index;
	weapon->entity_quality() = entity_quality;
	weapon->fallback_wear() = fallback_wear;

	return true;
}

void features::skins::full_update() {
	if (!forcing_update)
		return;

	if (!g::local || !g::local->is_alive())
		return;

	// update hud
	using clear_hud_weapon_icon_fn = int(__thiscall*)(void*, int);
	static auto o_clear_hud_weapon_icon = reinterpret_cast<clear_hud_weapon_icon_fn>(find_pattern("client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B")); // @xref: "WeaponIcon--itemcount"
	assert(o_clear_hud_weapon_icon != nullptr);
	assert(o_clear_hud_weapon_icon != nullptr);
	if (const auto hud_weapons = find_hud_element("CCSGO_HudWeaponSelection") - 0x28; hud_weapons != nullptr) {
		// go through all weapons
		for (std::size_t i = 0; i < *(hud_weapons + 0x20); i++)
			i = o_clear_hud_weapon_icon(hud_weapons, i);
	}

	interfaces::client_state->delta_tick = -1;

	forcing_update = false;
}

float weapon_wear(int weapon) {
	float weapon_wear = 0.0f;

	switch (weapon) {
	case 0:
		weapon_wear = 0.0000001f;
		break;
	case 1:
		weapon_wear = 0.07f;
		break;
	case 2:
		weapon_wear = 0.15f;
		break;
	case 3:
		weapon_wear = 0.38f;
		break;
	case 4:
		weapon_wear = 0.45f;
		break;
	}

	return weapon_wear;
}

void features::skins::gloves_changer() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (!c::skins::gloves_endable)
		return;

	//model gloves
	auto model_fang = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_brokenfang.mdl";
	auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
	auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
	auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
	auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
	auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
	auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
	auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";

	//index gloves
	auto index_fang = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_brokenfang.mdl"));
	auto index_blood = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
	auto index_sport = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
	auto index_slick = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
	auto index_leath = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
	auto index_moto = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
	auto index_speci = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
	auto index_hydra = interfaces::model_info->get_model_index(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

	//credit to namazso for nskinz
	uintptr_t* const wearables = local_player->get_wearables();
	if (!wearables)
		return;

	static uintptr_t glove_handle = uintptr_t(0);

	auto glove = reinterpret_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity_handle(wearables[0]));

	if (!glove) // There is no glove
	{
		const auto our_glove = reinterpret_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity_handle(glove_handle));

		if (our_glove) // Try to get our last created glove
		{
			wearables[0] = glove_handle;
			glove = our_glove;
		}
	}
	if (!local_player || // We are dead but we have a glove, destroy it
		!local_player->is_alive() ||
		!interfaces::engine->is_connected() ||
		!interfaces::engine->is_in_game()
		) {
		if (glove) {
			glove->net_set_destroyed_on_recreate_entities();
			glove->net_release();
		}
		return;
	}
	if (!glove) // We don't have a glove, but we should
	{
		const auto entry = interfaces::ent_list->get_highest_index() + 1;
		const auto serial = rand() % 0x1000;
		glove = make_glove(entry, serial);   // He he
		wearables[0] = entry | serial << 16;
		glove_handle = wearables[0]; // Let's store it in case we somehow lose it.
	}
	if (glove)
	{
		//glove conditions
		float g_wear = 0.f;
		switch (c::skins::gloves_wear) {
		case 0:
			g_wear = 0.0000001f;
			break;
		case 1:
			g_wear = 0.07f;
			break;
		case 2:
			g_wear = 0.15f;
			break;
		case 3:
			g_wear = 0.38f;
			break;
		case 4:
			g_wear = 0.45f;
			break;
		}

		static const char* glove_models[] = { nullptr, model_fang, model_blood, model_sport, model_slick, model_leath, model_moto, model_speci, model_hydra };
		const int glove_model_indexes[] = { 0, index_fang, index_blood, index_sport, index_slick, index_leath, index_moto, index_speci, index_hydra };
		static const int glove_ids[] = { 0, GLOVE_STUDDED_BROKENFANG, GLOVE_STUDDED_BLOODHOUND, GLOVE_SPORTY, GLOVE_SLICK, GLOVE_LEATHER_WRAP, GLOVE_MOTORCYCLE, GLOVE_SPECIALIST, GLOVE_HYDRA };

		//apply glove model
		for (int i = 0; i < IM_ARRAYSIZE(glove_models); i++) {
			if (i == 0) { continue; }
			if (c::skins::gloves_model == i) {
				apply_glove_model(glove, glove_models[c::skins::gloves_model]);
				break;
			}
		}

		//apply glove skins
		for (int i = 0; i < IM_ARRAYSIZE(glove_models); i++) {
			if (i == 0) { continue; }
			if (c::skins::gloves_model == i) {
				apply_glove_skin(glove, glove_ids[i], c::skins::gloves_skin_id, glove_model_indexes[i], 3, g_wear);
				break;
			}
		}

		glove->item_id_high() = -1;
		glove->fallback_seed() = 0;
		glove->fallback_stattrak() = -1;

		glove->net_pre_data_update(data_update_created);
	}
}

void features::skins::knife_changer() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	//model knifes
	const char* model_ct = "models/weapons/v_knife_default_ct.mdl";
	const char* model_t = "models/weapons/v_knife_default_t.mdl";
	const char* model_gold = "models/weapons/v_knife_gg.mdl";
	const char* model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	const char* model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	const char* model_karambit = "models/weapons/v_knife_karam.mdl";
	const char* model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	const char* model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	const char* model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	const char* model_flip = "models/weapons/v_knife_flip.mdl";
	const char* model_gut = "models/weapons/v_knife_gut.mdl";
	const char* model_huntsman = "models/weapons/v_knife_tactical.mdl";
	const char* model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	const char* model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	const char* model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	const char* model_talon = "models/weapons/v_knife_widowmaker.mdl";
	const char* model_ursus = "models/weapons/v_knife_ursus.mdl";
	const char* model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	const char* model_css = "models/weapons/v_knife_css.mdl";
	const char* model_outdoor = "models/weapons/v_knife_outdoor.mdl";
	const char* model_canis = "models/weapons/v_knife_canis.mdl";
	const char* model_cord = "models/weapons/v_knife_cord.mdl";

	//indes knifes
	int index_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	int index_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	int index_gold = interfaces::model_info->get_model_index("models/weapons/v_knife_gg.mdl");
	int index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	int index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	int index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	int index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	int index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	int index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	int index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	int index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	int index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	int index_shadow_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	int index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	int index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	int index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	int index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	int index_skeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	int index_css = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");
	int index_outdoor = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	int index_canis = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	int index_cord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");

	auto active_weapon = local_player->active_weapon();
	if (!active_weapon)
		return;

	auto my_weapons = local_player->weapons();
	for (size_t i = 0; my_weapons[i] != 0xFFFFFFFF; i++) {
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity_handle(my_weapons[i]));

		if (!weapon)
			return;

		float wear = 0.f;

		switch (c::skins::knife_changer_wear) {
		case 0: wear = 0.0000001f; break;
		case 1: wear = 0.07f; break;
		case 2: wear = 0.15f; break;
		case 3: wear = 0.38f; break;
		case 4: wear = 0.45f; break;
		}

		if (c::skins::knife_changer_enable) {
			static const char* knife_models[] = { "", model_bayonet, model_m9, model_karambit, model_bowie, model_butterfly, model_falchion, model_flip, model_gut, model_huntsman, model_shadow_daggers, model_navaja, model_stiletto, model_talon, model_ursus, model_ct, model_t, model_gold, model_css, model_outdoor, model_canis, model_cord, model_skeleton };
			const int knife_ids[] = { 0, WEAPON_BAYONET, WEAPON_KNIFE_M9_BAYONET, WEAPON_KNIFE_KARAMBIT, WEAPON_KNIFE_SURVIVAL_BOWIE, WEAPON_KNIFE_BUTTERFLY, WEAPON_KNIFE_FALCHION, WEAPON_KNIFE_FLIP, WEAPON_KNIFE_GUT, WEAPON_KNIFE_TACTICAL, WEAPON_KNIFE_PUSH, WEAPON_KNIFE_GYPSY_JACKKNIFE, WEAPON_KNIFE_STILETTO, WEAPON_KNIFE_WIDOWMAKER, WEAPON_KNIFE_URSUS, WEAPON_KNIFE, WEAPON_KNIFE_T, WEAPON_KNIFEGG, WEAPON_KNIFE_CSS, WEAPON_KNIFE_OUTDOOR, WEAPON_KNIFE_CANIS, WEAPON_KNIFE_CORD, WEAPON_KNIFE_SKELETON };
			static const int knife_indexes[] = { 0, index_bayonet, index_m9, index_karambit, index_bowie, index_butterfly, index_falchion, index_flip, index_gut, index_huntsman, index_shadow_daggers, index_navaja, index_stiletto, index_talon, index_ursus, index_ct, index_t, index_gold, index_css, index_outdoor, index_canis, index_cord, index_skeleton };

			if (active_weapon->client_class()->class_id == class_ids::cknife) {
				for (int i = 0; i < IM_ARRAYSIZE(knife_models); i++) {
					if (i == 0) { continue; }
					if (c::skins::knife_changer_model == i) {
						apply_knife_model(weapon, knife_models[c::skins::knife_changer_model]);
						break;
					}
				}
			}
			if (weapon->client_class()->class_id == class_ids::cknife) {
				for (int i = 0; i < IM_ARRAYSIZE(knife_models); i++) {
					if (i == 0) { continue; }
					if (c::skins::knife_changer_model == i) {
						apply_knife_skin(weapon, knife_ids[i], c::skins::knife_changer_paint_kit, knife_indexes[i], 3, wear);
						break;
					}
				}
			}
		}
		if (c::skins::weapon_endable) {
			static const int weapons[] = { WEAPON_USP_SILENCER, WEAPON_HKP2000, WEAPON_GLOCK, WEAPON_P250, WEAPON_FIVESEVEN, WEAPON_TEC9, WEAPON_CZ75A, WEAPON_ELITE, WEAPON_DEAGLE, WEAPON_REVOLVER, WEAPON_FAMAS, WEAPON_GALILAR, WEAPON_M4A1, WEAPON_M4A1_SILENCER, WEAPON_AK47, WEAPON_SG556, WEAPON_AUG, WEAPON_SSG08, WEAPON_AWP, WEAPON_SCAR20, WEAPON_G3SG1, WEAPON_SAWEDOFF, WEAPON_M249, WEAPON_NEGEV, WEAPON_MAG7, WEAPON_XM1014, WEAPON_NOVA, WEAPON_BIZON, WEAPON_MP5SD, WEAPON_MP7, WEAPON_MP9, WEAPON_MAC10, WEAPON_P90, WEAPON_UMP45 };

			for (int i = 0; i < IM_ARRAYSIZE(weapons); i++) {
				if (weapon->item_definition_index() == weapons[i]) {
					auto settings = &features::skins::weapon_skin[i];
					weapon->fallback_paint_kit() = settings->paint_kit_index, weapon->fallback_wear() = weapon_wear(settings->wear);
					break;
				}
			}
		}

		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->fallback_seed() = 661;
		weapon->item_id_high() = -1;
	}
}


typedef void(*recv_var_proxy_fn)(const c_recv_proxy_data* p_data, void* p_struct, void* p_out);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12
#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15
#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13
#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12
#define SEQUENCE_BOWIE_IDLE1 1
#define	LIFE_ALIVE 0
#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

recv_var_proxy_fn sequence_proxy_fn = nullptr;
recv_var_proxy_fn recv_model_index;

void features::skins::hooked_recvproxy_viewmodel(c_recv_proxy_data* p_data, void* p_struct, void* p_out) {

	//model knifes
	const char* model_ct = "models/weapons/v_knife_default_ct.mdl";
	const char* model_t = "models/weapons/v_knife_default_t.mdl";
	const char* model_gold = "models/weapons/v_knife_gg.mdl";
	const char* model_bayonet = "models/weapons/v_knife_bayonet.mdl";
	const char* model_m9 = "models/weapons/v_knife_m9_bay.mdl";
	const char* model_karambit = "models/weapons/v_knife_karam.mdl";
	const char* model_bowie = "models/weapons/v_knife_survival_bowie.mdl";
	const char* model_butterfly = "models/weapons/v_knife_butterfly.mdl";
	const char* model_falchion = "models/weapons/v_knife_falchion_advanced.mdl";
	const char* model_flip = "models/weapons/v_knife_flip.mdl";
	const char* model_gut = "models/weapons/v_knife_gut.mdl";
	const char* model_huntsman = "models/weapons/v_knife_tactical.mdl";
	const char* model_shadow_daggers = "models/weapons/v_knife_push.mdl";
	const char* model_navaja = "models/weapons/v_knife_gypsy_jackknife.mdl";
	const char* model_stiletto = "models/weapons/v_knife_stiletto.mdl";
	const char* model_talon = "models/weapons/v_knife_widowmaker.mdl";
	const char* model_ursus = "models/weapons/v_knife_ursus.mdl";
	const char* model_skeleton = "models/weapons/v_knife_skeleton.mdl";
	const char* model_css = "models/weapons/v_knife_css.mdl";
	const char* model_outdoor = "models/weapons/v_knife_outdoor.mdl";
	const char* model_canis = "models/weapons/v_knife_canis.mdl";
	const char* model_cord = "models/weapons/v_knife_cord.mdl";

	//indes knifes
	int index_ct = interfaces::model_info->get_model_index("models/weapons/v_knife_default_ct.mdl");
	int index_t = interfaces::model_info->get_model_index("models/weapons/v_knife_default_t.mdl");
	int index_gold = interfaces::model_info->get_model_index("models/weapons/v_knife_gg.mdl");
	int index_bayonet = interfaces::model_info->get_model_index("models/weapons/v_knife_bayonet.mdl");
	int index_m9 = interfaces::model_info->get_model_index("models/weapons/v_knife_m9_bay.mdl");
	int index_karambit = interfaces::model_info->get_model_index("models/weapons/v_knife_karam.mdl");
	int index_bowie = interfaces::model_info->get_model_index("models/weapons/v_knife_survival_bowie.mdl");
	int index_butterfly = interfaces::model_info->get_model_index("models/weapons/v_knife_butterfly.mdl");
	int index_falchion = interfaces::model_info->get_model_index("models/weapons/v_knife_falchion_advanced.mdl");
	int index_flip = interfaces::model_info->get_model_index("models/weapons/v_knife_flip.mdl");
	int index_gut = interfaces::model_info->get_model_index("models/weapons/v_knife_gut.mdl");
	int index_huntsman = interfaces::model_info->get_model_index("models/weapons/v_knife_tactical.mdl");
	int index_shadow_daggers = interfaces::model_info->get_model_index("models/weapons/v_knife_push.mdl");
	int index_navaja = interfaces::model_info->get_model_index("models/weapons/v_knife_gypsy_jackknife.mdl");
	int index_stiletto = interfaces::model_info->get_model_index("models/weapons/v_knife_stiletto.mdl");
	int index_talon = interfaces::model_info->get_model_index("models/weapons/v_knife_widowmaker.mdl");
	int index_ursus = interfaces::model_info->get_model_index("models/weapons/v_knife_ursus.mdl");
	int index_skeleton = interfaces::model_info->get_model_index("models/weapons/v_knife_skeleton.mdl");
	int index_css = interfaces::model_info->get_model_index("models/weapons/v_knife_css.mdl");
	int index_outdoor = interfaces::model_info->get_model_index("models/weapons/v_knife_outdoor.mdl");
	int index_canis = interfaces::model_info->get_model_index("models/weapons/v_knife_canis.mdl");
	int index_cord = interfaces::model_info->get_model_index("models/weapons/v_knife_cord.mdl");

	const int knife_indexes[] = { 0, index_bayonet, index_m9, index_karambit, index_bowie, index_butterfly, index_falchion, index_flip, index_gut, index_huntsman, index_shadow_daggers, index_navaja, index_stiletto, index_talon, index_ursus, index_ct, index_t, index_gold, index_css, index_outdoor, index_canis, index_cord, index_skeleton };
	static const char* knife_models[] = { "", model_bayonet, model_m9, model_karambit, model_bowie, model_butterfly, model_falchion, model_flip, model_gut, model_huntsman, model_shadow_daggers, model_navaja, model_stiletto, model_talon, model_ursus, model_ct, model_t, model_gold, model_css, model_outdoor, model_canis, model_cord, model_skeleton };

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (c::skins::knife_changer_model > 0 && c::skins::knife_changer_enable && local_player && local_player->is_alive() && interfaces::engine->is_connected()) {

		for (int i = 0; i < IM_ARRAYSIZE(knife_models); i++) {
			if (i == 0) { continue; }
			if (p_data->value.m_int == knife_indexes[i]) {
				p_data->value.m_int = knife_indexes[c::skins::knife_changer_model];
				break;
			}
		}
	}
	recv_model_index(p_data, p_struct, p_out);
}

void features::skins::set_view_model_sequence(const c_recv_proxy_data* pDataConst, void* p_struct, void* p_out) {
	c_recv_proxy_data* p_data = const_cast<c_recv_proxy_data*>(pDataConst);
	base_view_model* player_view_model = static_cast<base_view_model*>(p_struct);

	if (player_view_model) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
		player_t* p_owner = static_cast<player_t*>(interfaces::ent_list->get_client_entity(player_view_model->m_howner() & 0xFFF));
		if (p_owner == local_player) {
			std::string sz_model = interfaces::model_info->get_model_name(interfaces::model_info->get_model(player_view_model->model_index()));
			int m_nSequence = p_data->value.m_int;
			if (sz_model == "models/weapons/v_knife_butterfly.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (sz_model == "models/weapons/v_knife_falchion_advanced.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_push.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (sz_model == "models/weapons/v_knife_survival_bowie.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_ursus.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_stiletto.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_widowmaker.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_css.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_css.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_cord.mdl" || sz_model == "models/weapons/v_knife_canis.mdl" || sz_model == "models/weapons/v_knife_outdoor.mdl" || sz_model == "models/weapons/v_knife_skeleton.mdl") {
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
				}
			}
			p_data->value.m_int = m_nSequence;
		}
	}
	sequence_proxy_fn(p_data, p_struct, p_out);
}

void features::skins::animation_hook() {
	c_client_class* pClass = interfaces::client->get_all_classes();
	while (pClass)
	{
		const char* pszName = pClass->recvtable_ptr->table_name;
		if (!strcmp(pszName, ("DT_BaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClass->recvtable_ptr->props_count; nIndex++) {
				recv_prop* pProp = &(pClass->recvtable_ptr->props[nIndex]);

				if (!pProp || strcmp(pProp->prop_name, ("m_nSequence"))) continue;

				sequence_proxy_fn = (recv_var_proxy_fn)pProp->proxy_fn;

				pProp->proxy_fn = (recv_var_proxy_fn)set_view_model_sequence;
			}
		}

		if (!strcmp(pszName, ("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->recvtable_ptr->props_count; i++)
			{
				recv_prop* pProp = &(pClass->recvtable_ptr->props[i]);
				const char* name = pProp->prop_name;

				if (!strcmp(name, ("m_nModelIndex")))
				{
					recv_model_index = (recv_var_proxy_fn)pProp->proxy_fn;
					pProp->proxy_fn = (recv_var_proxy_fn)hooked_recvproxy_viewmodel;
				}
			}
		}
		pClass = pClass->next_ptr;
	}
	printf(("knife animations initialized\n"));
}

void features::skins::animation_unhook() {
	for (c_client_class* pClass = interfaces::client->get_all_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, ("CBaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, ("m_nSequence"))) continue;

				pProp->proxy_fn = sequence_proxy_fn;

				break;
			}
			break;
		}
	}

	for (c_client_class* pClass = interfaces::client->get_all_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, ("CBaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, ("m_nModelIndex"))) continue;

				pProp->proxy_fn = recv_model_index;

				break;
			}
			break;
		}
	}
}

void features::skins::agent_changer() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	const auto local = interfaces::ent_list->get<player_t>(interfaces::engine->get_local_player());

	if (!local || !local->is_alive() || local->client_class()->class_id != class_ids::ccsplayer)
		return;

	static bool prev_t = false;
	static bool prev_ct = false;

	if (!c::skins::agent_changer) {
		if (prev_t && local->team() == 2) {
			prev_t = false;
			prev_ct = false;
			features::skins::forcing_update = true;
		}
		if (prev_ct && local->team() == 3) {
			prev_t = false;
			prev_ct = false;
			features::skins::forcing_update = true;
		}
		return;
	}

	static auto game_type = interfaces::console->get_convar(("game_type"));

	if (game_type->get_int() == 6)
		return;

	const char* models_to_change_ct[] = {
	"models/player/custom_player/legacy/ctm_diver_varianta.mdl", // actually nothing here
	"models/player/custom_player/legacy/ctm_diver_varianta.mdl", // Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman
	"models/player/custom_player/legacy/ctm_diver_variantb.mdl", // Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman
	"models/player/custom_player/legacy/ctm_diver_variantc.mdl", // Lieutenant Rex Krikey | SEAL Frogman
	"models/player/custom_player/legacy/ctm_fbi_varianth.mdl", // Michael Syfers | FBI Sniper
	"models/player/custom_player/legacy/ctm_fbi_variantf.mdl", // Operator | FBI SWAT
	"models/player/custom_player/legacy/ctm_fbi_variantb.mdl", // Special Agent Ava | FBI
	"models/player/custom_player/legacy/ctm_fbi_variantg.mdl", // Markus Delrow | FBI HRT
	"models/player/custom_player/legacy/ctm_gendarmerie_varianta.mdl", // Sous-Lieutenant Medic | Gendarmerie Nationale
	"models/player/custom_player/legacy/ctm_gendarmerie_variantb.mdl", // Chem-Haz Capitaine | Gendarmerie Nationale
	"models/player/custom_player/legacy/ctm_gendarmerie_variantc.mdl", // Chef d'Escadron Rouchard | Gendarmerie Nationale
	"models/player/custom_player/legacy/ctm_gendarmerie_variantd.mdl", // Aspirant | Gendarmerie Nationale
	"models/player/custom_player/legacy/ctm_gendarmerie_variante.mdl", // Officer Jacques Beltram | Gendarmerie Nationale
	"models/player/custom_player/legacy/ctm_sas_variantg.mdl", // D Squadron Officer | NZSAS
	"models/player/custom_player/legacy/ctm_sas_variantf.mdl", // B Squadron Officer | SAS
	"models/player/custom_player/legacy/ctm_st6_variante.mdl", // Seal Team 6 Soldier | NSWC SEAL
	"models/player/custom_player/legacy/ctm_st6_variantg.mdl", // Buckshot | NSWC SEAL
	"models/player/custom_player/legacy/ctm_st6_varianti.mdl", // Lt. Commander Ricksaw | NSWC SEAL
	"models/player/custom_player/legacy/ctm_st6_variantj.mdl", // 'Blueberries' Buckshot | NSWC SEAL
	"models/player/custom_player/legacy/ctm_st6_variantk.mdl", // 3rd Commando Company | KSK
	"models/player/custom_player/legacy/ctm_st6_variantl.mdl", // 'Two Times' McCoy | TACP Cavalry
	"models/player/custom_player/legacy/ctm_st6_variantm.mdl", // 'Two Times' McCoy | USAF TACP
	"models/player/custom_player/legacy/ctm_st6_variantn.mdl", // Primeiro Tenente | Brazilian 1st Battalion
	"models/player/custom_player/legacy/ctm_swat_variante.mdl", // Cmdr. Mae 'Dead Cold' Jamison | SWAT
	"models/player/custom_player/legacy/ctm_swat_variantf.mdl", // 1st Lieutenant Farlow | SWAT
	"models/player/custom_player/legacy/ctm_swat_variantg.mdl", // John 'Van Healen' Kask | SWAT
	"models/player/custom_player/legacy/ctm_swat_varianth.mdl", // Bio-Haz Specialist | SWAT
	"models/player/custom_player/legacy/ctm_swat_varianti.mdl", // Sergeant Bombson | SWAT
	"models/player/custom_player/legacy/ctm_swat_variantj.mdl", // Chem-Haz Specialist | SWAT
	"models/player/custom_player/legacy/ctm_swat_variantk.mdl", // Lieutenant 'Tree Hugger' Farlow | SWAT
	"models/player/custom_player/legacy/tm_professional_varj.mdl", // Getaway Sally | The Professionals
	"models/player/custom_player/legacy/tm_professional_vari.mdl", // Number K | The Professionals
	"models/player/custom_player/legacy/tm_professional_varh.mdl", // Little Kev | The Professionals
	"models/player/custom_player/legacy/tm_professional_varg.mdl", // Safecracker Voltzmann | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf5.mdl", // Bloody Darryl The Strapped | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf4.mdl", // Sir Bloody Loudmouth Darryl | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf3.mdl", // Sir Bloody Darryl Royale | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf2.mdl", // Sir Bloody Skullhead Darryl | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf1.mdl", // Sir Bloody Silent Darryl | The Professionals
	"models/player/custom_player/legacy/tm_professional_varf.mdl", // Sir Bloody Miami Darryl | The Professionals
	"models/player/custom_player/legacy/tm_phoenix_varianti.mdl", // Street Soldier | Phoenix
	"models/player/custom_player/legacy/tm_phoenix_varianth.mdl", // Soldier | Phoenix
	"models/player/custom_player/legacy/tm_phoenix_variantg.mdl", // Slingshot | Phoenix
	"models/player/custom_player/legacy/tm_phoenix_variantf.mdl", // Enforcer | Phoenix
	"models/player/custom_player/legacy/tm_leet_variantj.mdl", // Mr. Muhlik | Elite Crew
	"models/player/custom_player/legacy/tm_leet_varianti.mdl", // Prof. Shahmat | Elite Crew
	"models/player/custom_player/legacy/tm_leet_varianth.mdl", // Osiris | Elite Crew
	"models/player/custom_player/legacy/tm_leet_variantg.mdl", // Ground Rebel | Elite Crew
	"models/player/custom_player/legacy/tm_leet_variantf.mdl", // The Elite Mr. Muhlik | Elite Crew
	"models/player/custom_player/legacy/tm_jungle_raider_variantf2.mdl", // Trapper | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variantf.mdl", // Trapper Aggressor | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variante.mdl", // Vypa Sista of the Revolution | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variantd.mdl", // Col. Mangos Dabisi | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variant?.mdl", // Arno The Overgrown | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variantb2.mdl", // 'Medium Rare' Crasswater | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_variantb.mdl", // Crasswater The Forgotten | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_jungle_raider_varianta.mdl", // Elite Trapper Solman | Guerrilla Warfare
	"models/player/custom_player/legacy/tm_balkan_varianth.mdl", // 'The Doctor' Romanov | Sabre
	"models/player/custom_player/legacy/tm_balkan_variantj.mdl", // Blackwolf | Sabre
	"models/player/custom_player/legacy/tm_balkan_varianti.mdl", // Maximus | Sabre
	"models/player/custom_player/legacy/tm_balkan_variantf.mdl", // Dragomir | Sabre
	"models/player/custom_player/legacy/tm_balkan_variantg.mdl", // Rezan The Ready | Sabre
	"models/player/custom_player/legacy/tm_balkan_variantk.mdl", // Rezan the Redshirt | Sabre
	"models/player/custom_player/legacy/tm_balkan_variantl.mdl", // Dragomir | Sabre Footsoldier
	};

	auto model_index_ct = interfaces::model_info->get_model_index(models_to_change_ct[c::skins::agent_ct]);
	auto model_index_t = interfaces::model_info->get_model_index(models_to_change_ct[c::skins::agent_t]);

	if (!model_index_ct || !model_index_t)
		return;

	if (local->team() == 2) {
		if (c::skins::agent_t > 0) {
			local->set_model_index(model_index_t);
		}
		if (auto comp = c::skins::agent_t > 0 ? true : false; prev_t != comp) {
			prev_t = c::skins::agent_t > 0 ? true : false;
			features::skins::forcing_update = true;
		}
	}
	else if (local->team() == 3) {
		if (c::skins::agent_ct > 0) {
			local->set_model_index(model_index_ct);
		}
		if (auto comp = c::skins::agent_ct > 0 ? true : false; prev_ct != comp) {
			prev_ct = c::skins::agent_ct > 0 ? true : false;
			features::skins::forcing_update = true;
		}
	}
}