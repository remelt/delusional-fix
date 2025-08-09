#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../../sdk/math/math.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../hooks/hooks.hpp"

i_material* mat_regular;
i_material* mat_flat;
i_material* mat_crystal;
i_material* mat_pearlescent;
i_material* mat_reverse_pearlescent;
i_material* mat_fog;
i_material* mat_damascus;
i_material* mat_model;
i_material* mat_overlay_glow;
i_material* mat_overlay_outline;
i_material* mat_overlay_metallic;
i_material* mat_overlay_snow;

i_material* create_material(std::string name, std::string type, std::string material) {
	auto kv = static_cast<c_key_values*>(malloc(36));
	kv->init(type.c_str());
	kv->load_from_buffer(name.c_str(), material.c_str());
	return interfaces::material_system->create_material(name.c_str(), kv);
}

void override_material(bool ignorez, bool wireframe, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = mat_regular; break;
	case 1: material = mat_flat; break;
	case 2: material = mat_crystal; break;
	case 3: material = mat_pearlescent; break;
	case 4: material = mat_reverse_pearlescent; break;
	case 5: material = mat_fog; break;
	case 6: material = mat_damascus; break;
	case 7: material = mat_model; break;
	}

	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);

	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void override_overlay_material(bool ignorez, bool wireframe, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = mat_overlay_glow; break;
	case 1: material = mat_overlay_outline; break;
	case 2: material = mat_overlay_metallic; break;
	case 3: material = mat_overlay_snow; break;
	}

	// material flags
	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);

	// overlay color
	auto overlay = material->find_var("$envmaptint", nullptr);
	overlay->set_vec_value(clr.r(), clr.g(), clr.b());

	// color modulate
	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void features::chams::initialize() {
	mat_regular = interfaces::material_system->find_material("debug/debugambientcube", TEXTURE_GROUP_MODEL);
	mat_regular->increment_reference_count();

	mat_flat = interfaces::material_system->find_material("debug/debugdrawflat", TEXTURE_GROUP_MODEL);
	mat_flat->increment_reference_count();

	mat_overlay_outline = interfaces::material_system->find_material("dev/glow_armsrace", TEXTURE_GROUP_MODEL);
	mat_overlay_outline->increment_reference_count();

	mat_pearlescent = create_material("pearlescent.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$model" "1"
	"$phong" "1"
	"$pearlescent" "1"
	"$basemapalphaphongmask" "1" })#");

	mat_reverse_pearlescent = create_material("pearlescent.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$model" "1"
	"$phong" "1"
	"$pearlescent" "-1"
	"$basemapalphaphongmask" "1" })#");

	mat_crystal = create_material("crystal.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$bumpmap" "models/weapons/customization/materials/origamil_camo"
	"$model" "1" })#");

	mat_overlay_glow = create_material("glow.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1" })#");

	mat_overlay_metallic = create_material("metallic.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$additive" "1"
	"$envmap" "env_cubemap"
	"$envmaptint" "[1 1 1]"
	"$envmapfresnel" "1" })#");

	mat_fog = create_material("fog.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$basetexture" "dev/zone_warning"
 	"$model" "1"
    Proxies {
		TextureScroll {
			"texturescrollvar" "$baseTextureTransform"
			"texturescrollrate" "0.10"
			"texturescrollangle" "90"
        }
    } })#");

	mat_overlay_snow = create_material("snow.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
    "$basetexture" "dev/snowfield"
	"$additive" "1"
    Proxies {
		TextureScroll {
			"texturescrollvar" "$baseTextureTransform"
            "texturescrollrate" "0.10"
            "texturescrollangle" "90"
        }
    } })#");

	mat_damascus = create_material("damascus.vmt", "VertexLitGeneric", R"#("VertexLitGeneric" {
	"$basetexture" "models\weapons\customization\paints\antiqued\damascus"
 	"$model" "1"
    Proxies {
		TextureScroll {
			"texturescrollvar" "$baseTextureTransform"
			"texturescrollrate" "0.10"
			"texturescrollangle" "90"
        }
    } })#");

	mat_model = create_material("model.vmt", "Modulate", R"#("Modulate" {
	"$basetexture" "vgui/white_additive"
	"$model" "1" })#");
}

void features::chams::run_other(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	
}

void features::chams::run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	const model_t* model = info.model;
	bool is_nuke = strstr(model->name, ("nuke_cargo_elevator_arms")) != nullptr;
	bool is_arm = strstr(model->name, ("arms")) != nullptr && !is_nuke;
	bool is_weapon = strstr(model->name, ("weapons/v_")) != nullptr && !is_arm;
	bool is_player = strstr(model->name, ("models/player")) != nullptr;
	bool is_sleeve = strstr(model->name, ("sleeve")) != nullptr;
	auto ent = interfaces::ent_list->get_client_entity(info.entity_index);

	if (is_player) {
		player_t* player = reinterpret_cast<player_t*>(ent);

		if (!player || !player->is_alive() || player->dormant() || player == g::local || player->team() == g::local->team()) 
			return;

		if (c::chams::invisible_chams) {
			override_material(true, c::chams::invisible_wireframe, c::chams::cham_type, color_t(c::chams::invisible_chams_clr[0], c::chams::invisible_chams_clr[1], c::chams::invisible_chams_clr[2], c::chams::invisible_chams_clr[3]));
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
		}
		interfaces::model_render->override_material(nullptr);

		if (c::chams::visible_chams) {
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_material(false, false, c::chams::cham_type, color_t(c::chams::visible_chams_clr[0], c::chams::visible_chams_clr[1], c::chams::visible_chams_clr[2], c::chams::visible_chams_clr[3]));
		}
		if (c::chams::visible_chams_ov) {
			sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, matrix);
			override_overlay_material(false, c::chams::visible_wireframe_ov, c::chams::cham_type_overlay, color_t(c::chams::visible_chams_clr_ov[0], c::chams::visible_chams_clr_ov[1],c::chams::visible_chams_clr_ov[2], c::chams::visible_chams_clr_ov[3]));
		}
	}
}

void override_material_bt(bool ignorez, bool wireframe, int type, color_t clr) {
	i_material* material = nullptr;

	switch (type) {
	case 0: material = mat_regular; break;
	case 1: material = mat_flat; break;
	case 2: material = mat_crystal; break;
	case 3: material = mat_pearlescent; break;
	case 4: material = mat_reverse_pearlescent; break;
	case 5: material = mat_fog; break;
	case 6: material = mat_damascus; break;
	case 7: material = mat_model; break;
	}

	material->set_material_var_flag(material_var_ignorez, ignorez);
	material->set_material_var_flag(material_var_wireframe, wireframe);

	material->color_modulate(clr.r(), clr.g(), clr.b());
	material->alpha_modulate(clr.a());
	interfaces::model_render->override_material(material);
}

void features::chams::run_bt(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	const auto mdl = info.model;

	if (!mdl) 
		return;

	bool is_player = strstr(mdl->name, "models/player") != nullptr;

	if (is_player) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(info.entity_index));
		if (!player || !player->is_alive() || player->dormant())
			return;

		if (player->team() != g::local->team()) {
			if (c::chams::backtrack_chams && c::backtrack::backtrack && player->is_moving() && g::local->is_alive()) {
				auto record = &backtrack.data[player->index()];
				if (c::chams::backtrack_chams_draw_all_ticks) {
					for (int i = 0; i < record->size(); i++) {
						if (record && !record->empty() && record->size() && (record->at(i).sim_time)) {
							float h, s, v, a;
							h = (math::rgbtoh(c::chams::backtrack_chams_clr2[0], c::chams::backtrack_chams_clr2[1], c::chams::backtrack_chams_clr2[2]) - math::rgbtoh(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2])) / backtrack.data[player->index()].size();
							s = (math::rgbtos(c::chams::backtrack_chams_clr2[0], c::chams::backtrack_chams_clr2[1], c::chams::backtrack_chams_clr2[2]) - math::rgbtos(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2])) / backtrack.data[player->index()].size();
							v = (math::rgbtov(c::chams::backtrack_chams_clr2[0], c::chams::backtrack_chams_clr2[1], c::chams::backtrack_chams_clr2[2]) - math::rgbtov(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2])) / backtrack.data[player->index()].size();
							a = (c::chams::backtrack_chams_clr2[3] - c::chams::backtrack_chams_clr1[3]) / backtrack.data[player->index()].size();

							float hh, ss, vv;
							hh = math::rgbtoh(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2]) + (h * i);
							ss = math::rgbtos(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2]) + (s * i);
							vv = math::rgbtov(c::chams::backtrack_chams_clr1[0], c::chams::backtrack_chams_clr1[1], c::chams::backtrack_chams_clr1[2]) + (v * i);
							float doneclr[3] = { math::fromhsvtor(hh, ss, vv), math::fromhsvtog(hh, ss, vv), math::fromhsvtob(hh, ss, vv) };

							color_t gradient = color_t(doneclr[0], doneclr[1], doneclr[2], c::chams::backtrack_chams_clr1[3] + (a * i));
							color_t non_gradient = color_t(c::chams::backtrack_chams_clr2[0], c::chams::backtrack_chams_clr2[1], c::chams::backtrack_chams_clr2[2], c::chams::backtrack_chams_clr2[3]);

							override_material_bt(c::chams::backtrack_chams_invisible, false, c::chams::cham_type_bt, c::chams::backtrack_chams_gradient ? gradient : non_gradient);

							sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, backtrack.data[player->index()][i].m_matrix);
							interfaces::model_render->override_material(nullptr);
						}
					}
				}
				else  {
					if (record && !record->empty() && record->size() && (record->front().sim_time)) {
						override_material_bt(c::chams::backtrack_chams_invisible, false, c::chams::cham_type_bt, color_t(c::chams::backtrack_chams_clr2[0], c::chams::backtrack_chams_clr2[1], c::chams::backtrack_chams_clr2[2], c::chams::backtrack_chams_clr2[3]));
						sdk::hooks::draw_model_execute::draw_model_execute_original(interfaces::model_render, 0, ctx, state, info, record->back().m_matrix);
						interfaces::model_render->override_material(nullptr);
					}
				}
			}
		}
	}
}