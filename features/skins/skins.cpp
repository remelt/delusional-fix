#include "skins.hpp"
#include "models.hpp"

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

bool apply_glove_skin(attributable_item_t* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear, int fallback_seed) noexcept {
	glove->item_definition_index() = item_definition_index;
	glove->fallback_paint_kit() = paint_kit;
	glove->set_model_index(model_index);
	glove->entity_quality() = entity_quality;
	glove->fallback_wear() = fallback_wear;
	glove->fallback_seed() = fallback_seed;

	return true;
}

bool apply_knife_skin(attributable_item_t* weapon, int item_definition_index, int paint_kit, int model_index, float fallback_wear, int fallback_seed) noexcept {

	const bool is_default = item_definition_index == WEAPON_KNIFE || item_definition_index == WEAPON_KNIFE_T;

	if (!is_default) {
		weapon->fallback_paint_kit() = paint_kit;
		weapon->fallback_wear() = fallback_wear;
		weapon->fallback_seed() = fallback_seed;
		weapon->entity_quality() = 3;
	}
	else {
		weapon->fallback_paint_kit() = 0;
		weapon->entity_quality() = 0;
	}
	weapon->item_definition_index() = item_definition_index;
	weapon->model_index() = model_index;

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

float get_wear(int weapon) {

	switch (weapon) {
	case 0:
		return 0.0000001f;
	case 1:
		return 0.07f;
	case 2:
		return 0.15f;
	case 3:
		return 0.38f;
	case 4:
		return 0.45f;
	default:
		return 0.f;
	}
}

void features::skins::gloves_changer() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	if (!c::skins::gloves_endable || !c::skins::gloves_model)
		return;

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
		const auto glove_model = models::gloves[c::skins::gloves_model];

		apply_glove_model(glove, glove_model);
		apply_glove_skin(glove, definition_index::gloves[c::skins::gloves_model], c::skins::gloves_skin_id, interfaces::model_info->get_model_index(glove_model), 3, get_wear(c::skins::gloves_wear), c::skins::gloves_seed);

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

	auto my_weapons = local_player->weapons();
	for (size_t i = 0; my_weapons[i] != 0xFFFFFFFF; i++) {
		auto weapon = reinterpret_cast<attributable_item_t*>(interfaces::ent_list->get_client_entity_handle(my_weapons[i]));

		if (!weapon)
			return;

		if (weapon->client_class()->class_id == class_ids::cknife) {
			if (!c::skins::knife_changer_enable || !c::skins::knife_changer_model) { continue; }

			const auto knife_model = models::knives[c::skins::knife_changer_model];
			const auto knife_model_index = interfaces::model_info->get_model_index(knife_model);

			apply_knife_model(weapon, knife_model);
			apply_knife_skin(weapon, definition_index::knives[c::skins::knife_changer_model], c::skins::knife_changer_paint_kit, knife_model_index, get_wear(c::skins::knife_changer_wear), c::skins::knife_changer_seed);
		}
		else if (c::skins::weapon_endable) {
			for (int a = 0; a < IM_ARRAYSIZE(definition_index::weapons); a++) {
				if (weapon->item_definition_index() == definition_index::weapons[a]) {
					auto settings = &c::skins::weapon_skin[a];
					if (!settings->paint_kit_index) { continue; }

					weapon->fallback_paint_kit() = settings->paint_kit_index, weapon->fallback_wear() = get_wear(settings->wear), weapon->fallback_seed() = settings->seed;
					break;
				}
			}
		}

		weapon->original_owner_xuid_low() = 0;
		weapon->original_owner_xuid_high() = 0;
		weapon->item_id_high() = -1;
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

	auto model_index_ct = interfaces::model_info->get_model_index(models::agents[c::skins::agent_ct]);
	auto model_index_t = interfaces::model_info->get_model_index(models::agents[c::skins::agent_t]);

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