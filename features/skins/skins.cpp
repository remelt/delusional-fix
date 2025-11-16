#include "skins.hpp"
#include "../../sdk/sdk.hpp"
#include "../../menu/config/config.hpp"

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


bool apply_knife_model( attributable_item_t* weapon, const char* model ) {
	auto local_player = reinterpret_cast< player_t* >( interfaces::ent_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
	if (!local_player)
		return false;

	auto viewmodel = reinterpret_cast< base_view_model* >( interfaces::ent_list->get_client_entity_handle( local_player->view_model()));
	if (!viewmodel)
		return false;

	auto h_view_model_weapon = viewmodel->m_hweapon( );
	if (!h_view_model_weapon)
		return false;

	auto view_model_weapon = reinterpret_cast< attributable_item_t* >( interfaces::ent_list->get_client_entity_handle( h_view_model_weapon ) );
	if (view_model_weapon != weapon)
		return false;

	viewmodel->model_index( ) = interfaces::model_info->get_model_index( model );

	if (strstr(model, "knife_gg") != nullptr) {
		auto team = local_player->team();
		viewmodel->m_nBody() = (team == 2) ? 0 : 1;    //if we t == knife_body_t, if ct == knife_body_ct
	}

	return true;
}

bool apply_knife_skin( attributable_item_t* weapon, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear ) {
	weapon->item_definition_index( ) = item_definition_index;
	weapon->fallback_paint_kit( ) = paint_kit;
	weapon->model_index( ) = model_index;
	weapon->entity_quality( ) = entity_quality;
	weapon->fallback_wear( ) = fallback_wear;

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
	auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
	auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
	auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
	auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
	auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
	auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
	auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";
	//index gloves
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

		//apply glove model
		switch (c::skins::gloves_model) {
		case 0:
			break;
		case 1:
			apply_glove_model(glove, model_blood);
			break;
		case 2:
			apply_glove_model(glove, model_sport);
			break;
		case 3:
			apply_glove_model(glove, model_slick);
			break;
		case 4:
			apply_glove_model(glove, model_leath);
			break;
		case 5:
			apply_glove_model(glove, model_moto);
			break;
		case 6:
			apply_glove_model(glove, model_speci);
			break;
		case 7:
			apply_glove_model(glove, model_hydra);
			break;
		}

		//apply glove skins
		switch (c::skins::gloves_model) {
		case 0:
			break;
		case 1:
			apply_glove_skin(glove, GLOVE_STUDDED_BLOODHOUND, c::skins::gloves_skin_id, index_blood, 3, g_wear);
			break;
		case 2:
			apply_glove_skin(glove, GLOVE_SPORTY, c::skins::gloves_skin_id, index_sport, 3, g_wear);
			break;
		case 3:
			apply_glove_skin(glove, GLOVE_SLICK, c::skins::gloves_skin_id, index_slick, 3, g_wear);
			break;
		case 4:
			apply_glove_skin(glove, GLOVE_LEATHER_WRAP, c::skins::gloves_skin_id, index_leath, 3, g_wear);
			break;
		case 5:
			apply_glove_skin(glove, GLOVE_MOTORCYCLE, c::skins::gloves_skin_id, index_moto, 3, g_wear);
			break;
		case 6:
			apply_glove_skin(glove, GLOVE_SPECIALIST, c::skins::gloves_skin_id, index_speci, 3, g_wear);
			break;
		case 7:
			apply_glove_skin(glove, GLOVE_HYDRA, c::skins::gloves_skin_id, index_hydra, 3, g_wear);
			break;
		}

		glove->item_id_high() = -1;
		glove->fallback_seed() = 0;
		glove->fallback_stattrak() = -1;

		glove->net_pre_data_update(data_update_created);
	}
}

void features::skins::knife_changer( ) {
	if (!interfaces::engine->is_connected( ) && !interfaces::engine->is_in_game( ))
		return;

	auto local_player = reinterpret_cast< player_t* >( interfaces::ent_list->get_client_entity( interfaces::engine->get_local_player( ) ) );
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
			if (active_weapon->client_class()->class_id == class_ids::cknife) {
				switch (c::skins::knife_changer_model) {
				case 0:
					break;
				case 1:
					apply_knife_model(weapon, model_bayonet);
					break;
				case 2:
					apply_knife_model(weapon, model_m9);
					break;
				case 3:
					apply_knife_model(weapon, model_karambit);
					break;
				case 4:
					apply_knife_model(weapon, model_bowie);
					break;
				case 5:
					apply_knife_model(weapon, model_butterfly);
					break;
				case 6:
					apply_knife_model(weapon, model_falchion);
					break;
				case 7:
					apply_knife_model(weapon, model_flip);
					break;
				case 8:
					apply_knife_model(weapon, model_gut);
					break;
				case 9:
					apply_knife_model(weapon, model_huntsman);
					break;
				case 10:
					apply_knife_model(weapon, model_shadow_daggers);
					break;
				case 11:
					apply_knife_model(weapon, model_navaja);
					break;
				case 12:
					apply_knife_model(weapon, model_stiletto);
					break;
				case 13:
					apply_knife_model(weapon, model_talon);
					break;
				case 14:
					apply_knife_model(weapon, model_ursus);
					break;
				case 15:
					apply_knife_model(weapon, model_ct);
					break;
				case 16:
					apply_knife_model(weapon, model_t);
					break;
				case 17:
					apply_knife_model(weapon, model_gold);
					break;
				case 18:
					apply_knife_model(weapon, model_css);
					break;
				case 19:
					apply_knife_model(weapon, model_outdoor);
					break;
				case 20:
					apply_knife_model(weapon, model_canis);
					break;
				case 21:
					apply_knife_model(weapon, model_cord);
					break;
				case 22:
					apply_knife_model(weapon, model_skeleton);
					break;
				}
			}

			if (weapon->client_class()->class_id == class_ids::cknife) {
				switch (c::skins::knife_changer_model) {
				case 0:
					break;
				case 1:
					apply_knife_skin(weapon, WEAPON_BAYONET, c::skins::knife_changer_paint_kit, index_bayonet, 3, wear);
					break;
				case 2:
					apply_knife_skin(weapon, WEAPON_KNIFE_M9_BAYONET, c::skins::knife_changer_paint_kit, index_m9, 3, wear);
					break;
				case 3:
					apply_knife_skin(weapon, WEAPON_KNIFE_KARAMBIT, c::skins::knife_changer_paint_kit, index_karambit, 3, wear);
					break;
				case 4:
					apply_knife_skin(weapon, WEAPON_KNIFE_SURVIVAL_BOWIE, c::skins::knife_changer_paint_kit, index_bowie, 3, wear);
					break;
				case 5:
					apply_knife_skin(weapon, WEAPON_KNIFE_BUTTERFLY, c::skins::knife_changer_paint_kit, index_butterfly, 3, wear);
					break;
				case 6:
					apply_knife_skin(weapon, WEAPON_KNIFE_FALCHION, c::skins::knife_changer_paint_kit, index_falchion, 3, wear);
					break;
				case 7:
					apply_knife_skin(weapon, WEAPON_KNIFE_FLIP, c::skins::knife_changer_paint_kit, index_flip, 3, wear);
					break;
				case 8:
					apply_knife_skin(weapon, WEAPON_KNIFE_GUT, c::skins::knife_changer_paint_kit, index_gut, 3, wear);
					break;
				case 9:
					apply_knife_skin(weapon, WEAPON_KNIFE_TACTICAL, c::skins::knife_changer_paint_kit, index_huntsman, 3, wear);
					break;
				case 10:
					apply_knife_skin(weapon, WEAPON_KNIFE_PUSH, c::skins::knife_changer_paint_kit, index_shadow_daggers, 3, wear);
					break;
				case 11:
					apply_knife_skin(weapon, WEAPON_KNIFE_GYPSY_JACKKNIFE, c::skins::knife_changer_paint_kit, index_navaja, 3, wear);
					break;
				case 12:
					apply_knife_skin(weapon, WEAPON_KNIFE_STILETTO, c::skins::knife_changer_paint_kit, index_stiletto, 3, wear);
					break;
				case 13:
					apply_knife_skin(weapon, WEAPON_KNIFE_WIDOWMAKER, c::skins::knife_changer_paint_kit, index_talon, 3, wear);
					break;
				case 14:
					apply_knife_skin(weapon, WEAPON_KNIFE_URSUS, c::skins::knife_changer_paint_kit, index_ursus, 3, wear);
					break;
				case 15:
					apply_knife_skin(weapon, WEAPON_KNIFE, c::skins::knife_changer_paint_kit, index_ct, 3, wear);
					break;
				case 16:
					apply_knife_skin(weapon, WEAPON_KNIFE_T, c::skins::knife_changer_paint_kit, index_t, 3, wear);
					break;
				case 17:
					apply_knife_skin(weapon, WEAPON_KNIFEGG, c::skins::knife_changer_paint_kit, index_gold, 3, wear);
					break;
				case 18:
					apply_knife_skin(weapon, WEAPON_KNIFE_CSS, c::skins::knife_changer_paint_kit, index_css, 3, wear);
					break;
				case 19:
					apply_knife_skin(weapon, WEAPON_KNIFE_OUTDOOR, c::skins::knife_changer_paint_kit, index_outdoor, 3, wear);
					break;
				case 20:
					apply_knife_skin(weapon, WEAPON_KNIFE_CANIS, c::skins::knife_changer_paint_kit, index_canis, 3, wear);
					break;
				case 21:
					apply_knife_skin(weapon, WEAPON_KNIFE_CORD, c::skins::knife_changer_paint_kit, index_cord, 3, wear);
					break;
				case 22:
					apply_knife_skin(weapon, WEAPON_KNIFE_SKELETON, c::skins::knife_changer_paint_kit, index_skeleton, 3, wear);
					break;
				}
			}
		}
		if (c::skins::weapon_endable) {
			//apply weapon skins
			switch (weapon->item_definition_index()) {
				case WEAPON_USP_SILENCER:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_usp, weapon->fallback_wear() = weapon_wear(c::skins::wear_usp);
					break;
				case WEAPON_HKP2000:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_p2000, weapon->fallback_wear() = weapon_wear(c::skins::wear_p2000);
					break;
				case WEAPON_GLOCK:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_glock, weapon->fallback_wear() = weapon_wear(c::skins::wear_glock);
					break;
				case WEAPON_P250:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_p250, weapon->fallback_wear() = weapon_wear(c::skins::wear_p250);
					break;
				case WEAPON_FIVESEVEN:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_fiveseven, weapon->fallback_wear() = weapon_wear(c::skins::wear_fiveseven);
					break;
				case WEAPON_TEC9:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_tec, weapon->fallback_wear() = weapon_wear(c::skins::wear_tec);
					break;
				case WEAPON_CZ75A:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_cz, weapon->fallback_wear() = weapon_wear(c::skins::wear_cz);
					break;
				case WEAPON_ELITE:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_duals, weapon->fallback_wear() = weapon_wear(c::skins::wear_duals);
					break;
				case WEAPON_DEAGLE:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_deagle, weapon->fallback_wear() = weapon_wear(c::skins::wear_deagle);
					break;
				case WEAPON_REVOLVER:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_revolver, weapon->fallback_wear() = weapon_wear(c::skins::wear_revolver);
					break;
				case WEAPON_FAMAS:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_famas, weapon->fallback_wear() = weapon_wear(c::skins::wear_famas);
					break;
				case WEAPON_GALILAR:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_galil, weapon->fallback_wear() = weapon_wear(c::skins::wear_galil);
					break;
				case WEAPON_M4A1:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_m4a4, weapon->fallback_wear() = weapon_wear(c::skins::wear_m4a4);
					break;
				case WEAPON_M4A1_SILENCER:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_m4a1, weapon->fallback_wear() = weapon_wear(c::skins::wear_m4a1);
					break;
				case WEAPON_AK47:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_ak47, weapon->fallback_wear() = weapon_wear(c::skins::wear_ak47);
					break;
				case WEAPON_SG556:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_sg553, weapon->fallback_wear() = weapon_wear(c::skins::wear_sg553);
					break;
				case WEAPON_AUG:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_aug, weapon->fallback_wear() = weapon_wear(c::skins::wear_aug);
					break;
				case WEAPON_SSG08:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_ssg08, weapon->fallback_wear() = weapon_wear(c::skins::wear_ssg08);
					break;
				case WEAPON_AWP:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_awp, weapon->fallback_wear() = weapon_wear(c::skins::wear_awp);
					break;
				case WEAPON_SCAR20:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_scar, weapon->fallback_wear() = weapon_wear(c::skins::wear_scar);
					break;
				case WEAPON_G3SG1:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_g3sg1, weapon->fallback_wear() = weapon_wear(c::skins::wear_g3sg1);
					break;
				case WEAPON_SAWEDOFF:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_sawoff, weapon->fallback_wear() = weapon_wear(c::skins::wear_sawoff);
					break;
				case WEAPON_M249:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_m249, weapon->fallback_wear() = weapon_wear(c::skins::wear_m249);
					break;
				case WEAPON_NEGEV:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_negev, weapon->fallback_wear() = weapon_wear(c::skins::wear_negev);
					break;
				case WEAPON_MAG7:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_mag7, weapon->fallback_wear() = weapon_wear(c::skins::wear_mag7);
					break;
				case WEAPON_XM1014:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_xm1014, weapon->fallback_wear() = weapon_wear(c::skins::wear_xm1014);
					break;
				case WEAPON_NOVA:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_nova, weapon->fallback_wear() = weapon_wear(c::skins::wear_nova);
					break;
				case WEAPON_BIZON:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_bizon, weapon->fallback_wear() = weapon_wear(c::skins::wear_bizon);
					break;
				case WEAPON_MP5SD:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_mp5sd, weapon->fallback_wear() = weapon_wear(c::skins::wear_mp5sd);
					break;
				case WEAPON_MP7:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_mp7, weapon->fallback_wear() = weapon_wear(c::skins::wear_mp7);
					break;
				case WEAPON_MP9:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_mp9, weapon->fallback_wear() = weapon_wear(c::skins::wear_mp9);
					break;
				case WEAPON_MAC10:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_mac10, weapon->fallback_wear() = weapon_wear(c::skins::wear_mac10);
					break;
				case WEAPON_P90:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_p90, weapon->fallback_wear() = weapon_wear(c::skins::wear_p90);
					break;
				case WEAPON_UMP45:
					weapon->fallback_paint_kit() = c::skins::paint_kit_index_ump45, weapon->fallback_wear() = weapon_wear(c::skins::wear_ump45);
					break;
			}
		}
		//if (g::local->body() == 0) {
		//	g::local->body() = 1;
		//}
		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->fallback_seed() = 661;
		weapon->item_id_high() = -1;
	}
}

// still working tho (rly bad actually, dont do like that)
static bool updated = true;
static bool updated_t = true;
static bool updated_ct = true;

void features::skins::agent_changer( ) {

	if (!c::skins::agent_changer && !updated) {
		features::skins::forcing_update = true;
		updated = true;
		return;
	}
	else if (!c::skins::agent_changer) {
		return;
	}

	if (!interfaces::engine->is_connected( ) || !interfaces::engine->is_in_game( ))
		return;

	const auto local = interfaces::ent_list->get<player_t>( interfaces::engine->get_local_player( ) );

	if (!local || !local->is_alive( ) || local->client_class( )->class_id != class_ids::ccsplayer)
		return;

	static auto game_type = interfaces::console->get_convar( ( "game_type" ) );

	if (game_type->get_int( ) == 6)
		return;

	const char* models_to_change_ct[ ] = {
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

	auto model_index_ct = interfaces::model_info->get_model_index( models_to_change_ct[ c::skins::agent_ct ] );

	auto model_index_t = interfaces::model_info->get_model_index(models_to_change_ct[ c::skins::agent_t ] );

	if (!model_index_ct || !model_index_t)
		return;

	// kinda fixed ?
	if (local->team() == 2) {
		if (!(c::skins::agent_t == 0)) {
			local->set_model_index(model_index_t);
			updated_t = false;
			updated = false;
		}
		else if (!updated_t) {
			features::skins::forcing_update = true;
			updated_t = true;
		}
	}
	else if (local->team() == 3) {
		if (!(c::skins::agent_ct == 0)) {
			updated = false;
			updated_ct = false;
			local->set_model_index(model_index_ct);
		}
		else if (!updated_ct) {
			updated_ct = true;
			features::skins::forcing_update = true;
		}
	}
}