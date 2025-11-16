#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"

int offset;
bool features::visuals::get_playerbox(player_t* entity, bbox_t& in) {
	if (!entity)
		return false;

	auto trans = entity->coordinate_frame();

	auto collideable = entity->collideable();

	if (!collideable)
		return false;

	vec3_t min = collideable->mins();
	vec3_t max = collideable->maxs();

	vec3_t points[] = { vec3_t(min.x, min.y, min.z), vec3_t(min.x, max.y, min.z), vec3_t(max.x, max.y, min.z), vec3_t(max.x, min.y, min.z),
						vec3_t(max.x, max.y, max.z), vec3_t(min.x, max.y, max.z), vec3_t(min.x, min.y, max.z), vec3_t(max.x, min.y, max.z) };

	vec3_t points_transformed[8];
	for (int i = 0; i < 8; i++) {
		math::transform_vector(points[i], trans, points_transformed[i]);
	}

	vec3_t pos = entity->abs_origin(), flb, brt, blb, frt, frb, brb, blt, flt;

	if (!interfaces::debug_overlay->world_to_screen(points_transformed[3], flb) || !interfaces::debug_overlay->world_to_screen(points_transformed[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[0], blb) || !interfaces::debug_overlay->world_to_screen(points_transformed[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[2], frb) || !interfaces::debug_overlay->world_to_screen(points_transformed[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points_transformed[6], blt) || !interfaces::debug_overlay->world_to_screen(points_transformed[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	in.x = static_cast<int>(left);
	in.y = static_cast<int>(top) - 5;
	in.w = static_cast<int>(right) - static_cast<int>(left);
	in.h = static_cast<int>(bottom) - static_cast<int>(top) + 5;

	return true;
}

std::array<float, 64> fade;
void features::visuals::player::run() {
	if (!g::local)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!c::visuals::enable_visuals)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (!entity || entity == g::local)
			continue;

		if (!entity || entity == g::local || !entity->is_alive())
			continue;

		bbox_t _box;
		if (!get_playerbox(entity, _box))
			continue;

		if (entity->team() == g::local->team())
			continue;

		offset = 0;

		animate(entity, entity->index());
		draw_name(entity, _box);
		draw_health(entity, _box);
		draw_weapon(entity, _box);
		draw_box(entity, _box);
		draw_armor(entity, _box);
		flags(entity, _box);
		dlights(entity);
		skeleton(entity);
		backtrack_dot(entity);
		backtrack_skeleton(entity);

		if (c::visuals::radar)
			entity->spotted() = true;

		if (c::visuals::oof_arrows)
			visuals::oof_arrows(entity);
	}

	int get_highest_index = interfaces::ent_list->get_highest_index();
	for (int i = 0; i < get_highest_index; i++) {

		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

		if (entity && entity != g::local) {
			dropped_weapons(entity);
		}
	}
}

void features::visuals::player::animate(player_t* entity, const int index) {
	const float step = 255.f * (20.f / 15.f) * interfaces::globals->frame_time;

	c::visuals::fade_animation ? entity->dormant()
		? fade.at(index) -= step : fade.at(index) += step * 5.f : entity->dormant()
		? fade.at(index) = 0.f : fade.at(index) = 230;

	if (fade.at(index) > 230)
		fade.at(index) = 230;
	if (fade.at(index) < 0.f)
		fade.at(index) = 0.f;
}

void features::visuals::player::draw_name(player_t* entity, bbox_t bbox) {
	if (c::visuals::playername) {
		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);
		std::string print(info.fakeplayer ? std::string("[bot] ").append(info.name).c_str() : info.name);

		ImVec2 size = im_render.measure_text(print, fonts::esp_name_font, c::fonts::esp_name_size);

		im_render.text(bbox.x + (bbox.w / 2), bbox.y - static_cast<int>(size.y) - 2,
			c::fonts::esp_name_size, fonts::esp_name_font,
			print,
			true,
			color_t(c::visuals::playername_color[0], c::visuals::playername_color[1], c::visuals::playername_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_name_font_flag[9], c::fonts::esp_name_font_flag[10]);
	}
}

constexpr float anim_time = 255 / 2.0f;
void features::visuals::player::draw_health(player_t* entity, bbox_t bbox) {
	if (c::visuals::healthesp) {
		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		bbox_t temp(bbox.x - 5, bbox.y + (bbox.h - bbox.h * (std::clamp<int>(entity->health(), 0, 100.f) / 100.f)), 2, bbox.h * (std::clamp<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
		bbox_t temp_bg(bbox.x - 5, bbox.y, 2, bbox.h);

		auto health_color = color_t::health_based(entity->health()).get_color_edit_alpha_int(fade.at(entity->index()));

		if (entity->health() > 100)
			health_color = color_t(255, 255, 255).get_color_edit_alpha_int(fade.at(entity->index()));

		std::string healtht = std::to_string(entity->health());

		static float prev_player_hp[65];

		if (prev_player_hp[entity->index()] > entity->health())
			prev_player_hp[entity->index()] -= anim_time * interfaces::globals->frame_time;
		else
			prev_player_hp[entity->index()] = entity->health();


		if (c::visuals::healthesp_s == 0) {
			//bar health
			auto health_color = color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index()));
			auto health_color2 = color_t(c::visuals::health_bar_gradient[0], c::visuals::health_bar_gradient[1], c::visuals::health_bar_gradient[2]).get_color_edit_alpha_int(fade.at(entity->index()));

			if (c::visuals::override_bar) {
				health_color = color_t(c::visuals::health_bar[0], c::visuals::health_bar[1], c::visuals::health_bar[2]).get_color_edit_alpha_int(fade.at(entity->index()));
			}
			else {
				health_color = color_t::health_based(entity->health()).get_color_edit_alpha(fade.at(entity->index()));

				if (entity->health() > 100)
					health_color = clrs::Green;
			}

			im_render.drawrectfilled(bbox.x - 6, bbox.y - 1, 2 + c::visuals::health_bar_width, bbox.h + 2, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));

			if (c::visuals::override_bar && c::visuals::gradient_bar) {
				im_render.drawrectfilledgradient(bbox.x - 5, bbox.y + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f), c::visuals::health_bar_width,
					bbox.h * (prev_player_hp[entity->index()]) / 100.f - (entity->health() >= 100 ? 0 : -1),
					color_t(c::visuals::health_bar[0], c::visuals::health_bar[1], c::visuals::health_bar[2]).get_color_edit_alpha_int(fade.at(entity->index())),
					color_t(c::visuals::health_bar[0], c::visuals::health_bar[1], c::visuals::health_bar[2]).get_color_edit_alpha_int(fade.at(entity->index())),
					color_t(health_color2),
					color_t(health_color2));
			}
			else {
				im_render.drawrectfilled(bbox.x - 5, bbox.y + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f), c::visuals::health_bar_width,
					bbox.h * (prev_player_hp[entity->index()]) / 100.f - (entity->health() >= 100 ? 0 : -1), color_t(health_color));

			}

			if (entity->health() < 100)
				im_render.text(bbox.x - 15, bbox.y - 1 + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f) - 3,
					c::fonts::esp_hp_size,
					fonts::esp_hp_font,
					std::to_string(static_cast<int>(prev_player_hp[entity->index()])),
					true,
					color_t(c::visuals::health_color[0], c::visuals::health_color[1], c::visuals::health_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_font_flag[9], c::fonts::esp_font_flag[10]);
		}
		else if (c::visuals::healthesp_s == 1) {
			//text
			im_render.text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + (c::visuals::playerarmor ? 4 : 1),
				c::fonts::esp_hp_size,
				fonts::esp_hp_font,
				healtht,
				true,
				color_t(c::visuals::health_color[0], c::visuals::health_color[1], c::visuals::health_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_font_flag[9], c::fonts::esp_font_flag[10]);
			offset += 13;
		}
		else if (c::visuals::healthesp_s == 2) {
			//only text
			im_render.text(bbox.x - 13, bbox.y - 1 + (bbox.h - bbox.h * (prev_player_hp[entity->index()]) / 100.f) - 3,
				c::fonts::esp_hp_size,
				fonts::esp_hp_font,
				std::to_string(static_cast<int>(prev_player_hp[entity->index()])),
				true,
				color_t(c::visuals::health_color[0], c::visuals::health_color[1], c::visuals::health_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_font_flag[9], c::fonts::esp_font_flag[10]);
		}
	}
}

std::string u8toStr(const std::u8string& u8str) {
	return std::string{ u8str.cbegin(), u8str.cend() };
}

void features::visuals::player::draw_weapon(player_t* entity, bbox_t bbox) {
	if (c::visuals::playerweapon) {
		auto weapon = entity->active_weapon();

		if (!weapon)
			return;

		auto weapon_data = weapon->get_weapon_data();
		if (!weapon_data)
			return;

		if (c::visuals::player_weapon_type[0]) {
			im_render.text(bbox.x + (bbox.w / 2), bbox.h + bbox.y + (c::visuals::playerarmor ? 4 : 1) + offset, c::fonts::esp_wpn_size, fonts::esp_wpn_font, weapon->weapon_name_definition(), true,
				color_t(c::visuals::player_weapon_color[0], c::visuals::player_weapon_color[1], c::visuals::player_weapon_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_wpn_font_flag[9], c::fonts::esp_wpn_font_flag[10]);
			offset += 13;
		}

		if (c::visuals::player_weapon_type[1]) {
			auto wep_name = u8toStr(weapon->get_wpn_icon());

			im_render.text(bbox.x + (bbox.w / 2), bbox.h + bbox.y + (c::visuals::playerarmor ? 4 : 1) + offset, c::fonts::esp_wpn_size, fonts::esp_wpn_icon, wep_name, true,
				color_t(c::visuals::player_weapon_color[0], c::visuals::player_weapon_color[1], c::visuals::player_weapon_color[2]).get_color_edit_alpha_int(fade.at(entity->index())), c::fonts::esp_wpn_font_flag[9], c::fonts::esp_wpn_font_flag[10]);
		}
		offset += 13;
	}
}

void features::visuals::player::draw_box(player_t* entity, bbox_t bbox) {
	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (c::visuals::playerbox) {
		switch (c::visuals::boxtype) {
		case 0:
			//filled
			if (c::visuals::player_box_outline[0])
				im_render.drawrect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));
			if (c::visuals::player_box_outline[1])
				im_render.drawrect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));
			im_render.drawrect(bbox.x, bbox.y, bbox.w, bbox.h, color_t(c::visuals::playerbox_color[0], c::visuals::playerbox_color[1], c::visuals::playerbox_color[2]).get_color_edit_alpha_int(fade.at(entity->index())));

			break;
		case 1:
			//corner
			if (c::visuals::player_box_outline[0])
				im_render.drawcornerbox(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));
			im_render.drawcornerbox(bbox.x, bbox.y, bbox.w, bbox.h, color_t(c::visuals::playerbox_color[0], c::visuals::playerbox_color[1], c::visuals::playerbox_color[2]).get_color_edit_alpha_int(fade.at(entity->index())));
			if (c::visuals::player_box_outline[1])
				im_render.drawcornerbox(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));
			break;
		}
	}
}

void features::visuals::player::draw_armor(player_t* entity, bbox_t bbox) {
	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (c::visuals::playerarmor) {
		auto armor = entity->armor();

		if (!armor)
			return;

		im_render.drawrectfilled(bbox.x - 1, bbox.y + bbox.h + 2, bbox.w + 2, 3, color_t(0, 0, 0).get_color_edit_alpha_int(fade.at(entity->index())));
		im_render.drawrectfilled(bbox.x, bbox.y + bbox.h + 3, (bbox.w * entity->armor()) / 100, 1, color_t(c::visuals::playerarmor_clr[0], c::visuals::playerarmor_clr[1], c::visuals::playerarmor_clr[2]).get_color_edit_alpha_int(fade.at(entity->index())));
	}
}

void features::visuals::player::flags(player_t* entity, bbox_t bbox) {
	std::vector<std::pair<std::string, color_t>> flags;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);


	//if (c::visuals::visuals_flags[0] && info.fakeplayer)
		//flags.push_back(std::pair<std::string, color_t>(("bot"), color_t(255, 255, 255).get_color_edit_alpha_int(fade.at(entity->index()))));

	if (c::visuals::visuals_flags[1] && entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color_t>(("hk"), color_t(255, 255, 255).get_color_edit_alpha_int(fade.at(entity->index()))));
	else if (c::visuals::visuals_flags[1] && !entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color_t>(("k"), color_t(255, 255, 255).get_color_edit_alpha_int(fade.at(entity->index()))));

	if (c::visuals::visuals_flags[2] && entity->money())
		flags.push_back(std::pair<std::string, color_t>(std::string("$").append(std::to_string(entity->money())), color_t(35, 255, 0).get_color_edit_alpha_int(fade.at(entity->index()))));

	if (c::visuals::visuals_flags[3] && entity->is_scoped())
		flags.push_back(std::pair<std::string, color_t>(std::string(("scoped")), color_t(255, 255, 255).get_color_edit_alpha_int(fade.at(entity->index()))));

	if (c::visuals::visuals_flags[4] && entity->is_flashed())
		flags.push_back(std::pair<std::string, color_t>(std::string(("flashed")), color_t(255, 70, 70).get_color_edit_alpha_int(fade.at(entity->index()))));

	if (c::visuals::visuals_flags[5] && entity->is_defusing())
		flags.push_back(std::pair<std::string, color_t>(std::string(("defusing")), color_t(255, 70, 70).get_color_edit_alpha_int(fade.at(entity->index()))));

	auto position = 0;
	for (auto text : flags) {
		im_render.text(bbox.x + bbox.w + 3, bbox.y + position - 2, 12.f, fonts::esp_misc, text.first, false, text.second, true);
		position += 10;
	}
}

void draw_bomb_text(entity_t* bomb_ent, float time) {

}

void features::visuals::bomb_esp(entity_t* entity) {
	if (!c::visuals::bomb_esp)
		return;
}

int padding = 0;
void features::visuals::dropped_weapons(player_t* entity) {
	auto class_id = entity->client_class()->class_id;
	auto model_name = interfaces::model_info->get_model_name(entity->model());
	auto weapon = entity;
	if (!entity || !weapon || !class_id)
		return;

	bbox_t bbox;
	if (get_playerbox(weapon, bbox) && strstr(model_name, "models/weapons/w_") && strstr(model_name, "_dropped.mdl")) {
		auto wpn = (weapon_t*)entity;
		if (!wpn)
			return;

		float origin = g::local->origin().distance_to(entity->origin());
		float distance = origin <= 250.f ? 1.f : 1.f - ((origin - 250.f) / (c::visuals::dropped_weapon_distance));

		if (distance < 0.02f)
			return;

		if (c::visuals::dropped_weapon_name) {
			im_render.text(bbox.x + (bbox.w / 2), bbox.y - 13, c::fonts::esp_dropped_wpn_size, fonts::esp_dropped, wpn->weapon_name_definition().c_str(), true, color_t(c::visuals::dropped_weapon_name_clr[0], c::visuals::dropped_weapon_name_clr[1], c::visuals::dropped_weapon_name_clr[2], c::visuals::dropped_weapon_name_clr[3]), c::fonts::esp_dropped_wpn_font_flag[9], c::fonts::esp_dropped_wpn_font_flag[10]);
			padding += 13;
		}

		if (c::visuals::dropped_weapon_icon) {
			auto wep_name = u8toStr(wpn->get_wpn_icon());

			im_render.text(bbox.x + (bbox.w / 2), bbox.y + 7, 12.f, fonts::esp_wpn_icon, wep_name, true,
				color_t(c::visuals::dropped_weapon_icon_clr[0], c::visuals::dropped_weapon_icon_clr[1], c::visuals::dropped_weapon_icon_clr[2], c::visuals::dropped_weapon_icon_clr[3]));
		}
	}
}

void features::visuals::player::backtrack_dot(player_t* entity) {
	auto records = &backtrack.data[entity->index()];

	if (!c::visuals::skeletonesp)
		return;

	if (records->size() < 2)
		return;

	vec3_t previous_screenpos;

	for (int i = 1; i < backtrack.data[entity->index()].size(); i++) {//loop bt records
		vec3_t screen_pos;
		auto head = backtrack.data[entity->index()][i].m_headpos;

		if (!interfaces::debug_overlay->world_to_screen(head, screen_pos))
			continue;

		if (previous_screenpos.is_valid() && !previous_screenpos.is_zero()) {

			im_render.drawrect(screen_pos.x, screen_pos.y, 2, 2, color_t(c::visuals::skeletonbt_clr[0], c::visuals::skeletonbt_clr[1], c::visuals::skeletonbt_clr[2]).get_color_edit_alpha_int(fade.at(entity->index())), 0.1f);

			if (c::backtrack::selected_tick)
				im_render.drawcircle(screen_pos.x, screen_pos.y, 4, 32,
					(i == backtrack.selected_tick && entity->index() == backtrack.sel_ent_index) ? color_t(c::visuals::selected_tick_clr[0], c::visuals::selected_tick_clr[1], c::visuals::selected_tick_clr[2], 1.f) : color_t(0, 0, 0, 0));

		} previous_screenpos = screen_pos;
	}
}

void features::visuals::player::backtrack_skeleton(player_t* entity) {
	if (!c::visuals::skeleton_last)
		return;

	auto hdr = interfaces::model_info->get_studio_model(entity->model());

	if (!hdr)
		return;

	if (backtrack.data[entity->index()].size() < 1)
		return;

	auto record = &backtrack.data[entity->index()].back();

	vec3_t v_parent, v_child, s_parent, s_child;

	for (int j = 0; j < hdr->bones_count; j++) {
		studio_bone_t* bone = hdr->bone(j);

		if (!bone)
			continue;

		if ((bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			v_child = entity->get_bone_position(j);
			v_parent = entity->get_bone_position(bone->parent);

			v_child = vec3_t(record->m_matrix[j][0][3], record->m_matrix[j][1][3], record->m_matrix[j][2][3]);
			v_parent = vec3_t(record->m_matrix[bone->parent][0][3], record->m_matrix[bone->parent][1][3], record->m_matrix[bone->parent][2][3]);

			if (interfaces::debug_overlay->world_to_screen(v_parent, s_parent) && interfaces::debug_overlay->world_to_screen(v_child, s_child)) {
				im_render.drawline(s_parent[0], s_parent[1], s_child[0], s_child[1], color_t(c::visuals::skeleton_last_clr[0], c::visuals::skeleton_last_clr[1], c::visuals::skeleton_last_clr[2]).get_color_edit_alpha_int(fade.at(entity->index())));
			}
		}
	}
}

void features::visuals::player::skeleton(player_t* entity) {
	if (c::visuals::skeleton) {

		auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());
		if (!p_studio_hdr)
			return;

		vec3_t v_parent, v_child, s_parent, s_child;

		for (int i = 0; i < p_studio_hdr->bones_count; i++) {
			studio_bone_t* bone = p_studio_hdr->bone(i);
			if (!bone)
				return;

			if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
				v_child = entity->get_bone_position(i);
				v_parent = entity->get_bone_position(bone->parent);

				if (interfaces::debug_overlay->world_to_screen(v_parent, s_parent) && interfaces::debug_overlay->world_to_screen(v_child, s_child))
					im_render.drawline(s_parent[0], s_parent[1], s_child[0], s_child[1], color_t(c::visuals::skeletonesp_clr[0], c::visuals::skeletonesp_clr[1], c::visuals::skeletonesp_clr[2]).get_color_edit_alpha_int(fade.at(entity->index())));
			}
		}
	}
}

void rotate(std::array< vec2_t, 3 >& points, float rotation) {
	const auto points_center = (points.at(0) + points.at(1) + points.at(2)) / 3;
	for (auto& point : points) {
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = math::deg2rad(rotation);
		const auto c = cos(theta);
		const auto s = sin(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}

void features::visuals::oof_arrows(player_t* player) {
	int width, height;
	interfaces::engine->get_screen_size(width, height);
	vec3_t screen_point;
	interfaces::debug_overlay->world_to_screen(player->origin(), screen_point);
	if (screen_point.x < 0 || screen_point.y < 0 || screen_point.x > width || screen_point.y > height) {
		vec3_t viewangles;

		interfaces::engine->get_view_angles(viewangles);

		const auto screen_center = vec2_t(width * .5f, height * .5f);
		const auto angle_yaw_rad = math::deg2rad(viewangles.y - math::calc_angle(g::local->get_eye_pos(), player->get_hitbox_position(2)).y - 90);

		int radius = c::visuals::oof_arrows_dist;
		int size = c::visuals::oof_arrows_size;

		const auto new_point_x = screen_center.x + ((((width - (size * 3)) * .5f) * (radius / 100.0f)) * cos(angle_yaw_rad)) + (int)(6.0f * (((float)size - 4.f) / 16.0f));
		const auto new_point_y = screen_center.y + ((((height - (size * 3)) * .5f) * (radius / 100.0f)) * sin(angle_yaw_rad));

		std::array< vec2_t, 3 >points{ vec2_t(new_point_x - size, new_point_y - size),
			vec2_t(new_point_x + size, new_point_y),
			vec2_t(new_point_x - size, new_point_y + size) };

		rotate(points, viewangles.y - math::calc_angle(g::local->get_eye_pos(), player->get_hitbox_position(2)).y - 90);
		surface::draw_filled_triangle(points, player->visible() ? color(c::visuals::oof_arrows_clr[0] * 255, c::visuals::oof_arrows_clr[1] * 255, c::visuals::oof_arrows_clr[2] * 255, static_cast<float>(c::visuals::oof_arrows_clr[3] * fade.at(player->index()))) : color(c::visuals::oof_arrows_clr2[0] * 255, c::visuals::oof_arrows_clr2[1] * 255, c::visuals::oof_arrows_clr2[2] * 255, static_cast<float>(c::visuals::oof_arrows_clr2[3] * fade.at(player->index()))));
	}
}