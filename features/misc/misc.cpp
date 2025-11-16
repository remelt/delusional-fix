#include "misc.hpp"
#include "../../menu/menu.hpp"
#include "../../includes/imgui/imgui.h"
#include "../../includes/imgui/imgui_internal.h"
#include "../../features/visuals/visuals.hpp"
#include "../../sdk/sdk.hpp"

void features::misc::draw( ) {
	const auto stay_time = 4.f;
	const auto fade_time = 0.6f;

	for (auto i = 0; i < features::misc::notify_list.size(); i++) {
		if (features::misc::notify_list.empty())
			continue;

		notify_t notify = features::misc::notify_list[i];

		float elapsed_time = interfaces::globals->cur_time - (notify.time + stay_time);
		int fade_alpha = static_cast<int>(255.f - (elapsed_time / fade_time) * 255.f);
		fade_alpha = std::clamp(fade_alpha, 0, 255);

		if (fade_alpha <= 0) {
			features::misc::notify_list.erase(features::misc::notify_list.begin() + i);
			i--;
			continue;
		}



		ImGui::PushFont(fonts::logs_font_flag);
		ImGui::GetForegroundDrawList()->AddText(ImVec2(5 + 1, 5 + (i * 15) + 1), ImColor(0, 0, 0, fade_alpha), notify.text.c_str());
		ImGui::GetForegroundDrawList()->AddText(ImVec2(5, 5 + (i * 15)), ImColor(notify._color.r, notify._color.g, notify._color.b, fade_alpha), notify.text.c_str());
		ImGui::PopFont();
	}
}

void features::misc::notify( std::string text, color _color ) {
	if (text.empty()) {
		return;
	}

	features::misc::notify_list.push_back( notify_t( text, color( _color.r, _color.g, _color.b, _color.a ) ) );
}

void renderbox(int x, int y, int w, int h) {
	interfaces::surface->draw_filled_rectangle(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y, w, h);
}

void renderlineh(int x, int y, int size) {
	interfaces::surface->draw_line(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y, features::misc::iwebzspec.add_x + x + size, features::misc::iwebzspec.add_y + y);
}

void renderlinev(int x, int y, int size) {
	interfaces::surface->draw_line(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y, features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y + size);
}

void renderdiagonalr(int x, int y, int size) {
	interfaces::surface->draw_line(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y + size, features::misc::iwebzspec.add_x + x + size, features::misc::iwebzspec.add_y + y);
}

void render_background(int x, int y, int width) {
	interfaces::surface->set_drawing_color(59, 59, 59, 255);
	for (int i = 1; i < 20; i++) {
		renderlineh(x + i + 1, y + 20 - i, width - 1);
	}
	for (int i = 1; i <= features::misc::iwebzspec.count; i++) {
		if (i % 2 == 0) {
			interfaces::surface->set_drawing_color(75, 75, 75, 255);
			interfaces::surface->draw_filled_rectangle(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y + (i * 20), width, 20);
		}
		else {
			interfaces::surface->set_drawing_color(45, 45, 45, 255);
			interfaces::surface->draw_filled_rectangle(features::misc::iwebzspec.add_x + x, features::misc::iwebzspec.add_y + y + (i * 20), width, 20);
		}
	}
	interfaces::surface->set_drawing_color(85, 85, 85, 255);
	renderlineh(x + 20, y, width + 1);
	renderdiagonalr(x, y, 20);
	renderdiagonalr(x + width, y, 20);
	renderlineh(x, y + 20, width);
	renderlinev(x, y + 20, features::misc::iwebzspec.count * 20);
	renderlinev(width, y + 20, features::misc::iwebzspec.count * 20);
	renderlineh(x, y + 20 + (features::misc::iwebzspec.count * 20), width);
}

void features::misc::spectators_list_iwebz() {
	if (!c::misc::spectators_list)
		return;

	if (c::misc::spectatorlist_type == 1) {

		static bool is_dragging = false;
		static ImVec2 drag_offset;

		auto mouse_pos = get_mouse_position();

		bool is_pos = (mouse_pos.x >= features::misc::iwebzspec.add_x && mouse_pos.x <= features::misc::iwebzspec.add_x + features::misc::iwebzspec.width + 20 &&
			mouse_pos.y >= features::misc::iwebzspec.add_y && mouse_pos.y <= features::misc::iwebzspec.add_y + features::misc::iwebzspec.height);
		bool is_press = GetAsyncKeyState(VK_LBUTTON) & 0x8000;

		if (is_pos && is_press) {
			if (!is_dragging) {
				drag_offset = ImVec2(mouse_pos.x - features::misc::iwebzspec.add_x, mouse_pos.y - features::misc::iwebzspec.add_y);
			}
			is_dragging = true;
		}
		else if (!is_press) {
			is_dragging = false;
		}

		if (is_dragging) {
			features::misc::iwebzspec.add_x = static_cast<int>(mouse_pos.x - drag_offset.x);
			features::misc::iwebzspec.add_y = static_cast<int>(mouse_pos.y - drag_offset.y);
		}

		render_background(0, 0, features::misc::iwebzspec.width);

		surface::text(features::misc::iwebzspec.add_x + (features::misc::iwebzspec.width / 2) - 42, features::misc::iwebzspec.add_y + 4, surface::fonts::esp, "SPECTATOR LIST", false, color(c::misc::spectators_list_color_1[0] * 255, c::misc::spectators_list_color_1[1] * 255, c::misc::spectators_list_color_1[2] * 255));

		features::misc::iwebzspec.count = 0;
		std::vector<std::string> spectator_names;
		std::vector<std::string> spectator_targets;
		for (int i = 1; i <= interfaces::engine->get_max_clients(); i++) {
			auto player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

			if (!player || player->is_alive() || player->dormant())
				continue;

			if (!c::misc::spectatorlist_show_target)
				if (reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(player->observer_target())) != g::local)
					continue;

			auto spectated_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(player->observer_target()));

			player_info_t info;
			interfaces::engine->get_player_info(player->index(), &info);

			if (std::strcmp(info.name, "GOTV") != 0) {
				if (spectated_player && spectated_player->is_alive()) {
					features::misc::iwebzspec.count++;
					player_info_t spectated_info;
					interfaces::engine->get_player_info(spectated_player->index(), &spectated_info);

					char spectate_buffer[1024];
					if (c::misc::spectatorlist_show_target) {
						spectator_names.push_back(info.name);
						spectator_targets.push_back(spectated_info.name);
						sprintf_s(spectate_buffer, ("%s -> %s"), info.name, spectated_info.name);
					}
					else {
						spectator_names.push_back(info.name);
						sprintf_s(spectate_buffer, ("%s"), info.name);
					}

					color col_text(255, 255, 255, 255);
					if (spectated_player == g::local) {
						col_text = color(c::misc::spectators_list_color_1[0] * 255, c::misc::spectators_list_color_1[1] * 255, c::misc::spectators_list_color_1[2] * 255);
					}

					surface::text(features::misc::iwebzspec.add_x + 4, features::misc::iwebzspec.add_y + 4 + (features::misc::iwebzspec.count * 20), surface::fonts::watermark, spectate_buffer, false, col_text);
					features::misc::iwebzspec.t_size = surface::get_text_size(surface::fonts::esp, spectate_buffer).x;
				}
				else if (!spectated_player && c::misc::spectatorlist_show_target) {
					features::misc::iwebzspec.count++;
					surface::text(features::misc::iwebzspec.add_x + 4, features::misc::iwebzspec.add_y + 4 + (features::misc::iwebzspec.count * 20), surface::fonts::watermark, info.name, false, color(255, 255, 255, 255));
					features::misc::iwebzspec.t_size = surface::get_text_size(surface::fonts::esp, info.name).x;
				}
			}
		}
	}
}

void features::misc::spectators_list() {
	if (!c::misc::spectators_list || !interfaces::engine->is_in_game())
		return;

	if (c::misc::spectatorlist_type == 0) {
		int x, y;
		int height = 0;
		interfaces::engine->get_screen_size(x, y);
		std::vector<std::string> spectator_names;
		std::vector<std::string> spectator_targets;
		for (int i = 0; i < interfaces::globals->max_clients; i++) {
			auto observator = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(i));

			if (!observator || observator->is_alive() || observator->dormant())
				continue;

			if (!c::misc::spectatorlist_show_target)
				if (reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(observator->observer_target())) != g::local)
					continue;

			auto spectated_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(observator->observer_target()));

			if (!spectated_player || !spectated_player->is_alive())
				continue;

			if (c::misc::spectator_local && spectated_player != g::local)
				continue;

			player_info_t spectating_info, spectated_info;
			interfaces::engine->get_player_info(observator->index(), &spectating_info);
			interfaces::engine->get_player_info(spectated_player->index(), &spectated_info);

			char spectate_buffer[1024];
			if (c::misc::spectatorlist_show_target) {
				spectator_names.push_back(spectating_info.name);
				spectator_targets.push_back(spectated_info.name);
				sprintf_s(spectate_buffer, ("%s -> %s"), spectating_info.name, spectated_info.name);
			}
			else {
				spectator_names.push_back(spectating_info.name);
				sprintf_s(spectate_buffer, ("%s"), spectating_info.name);
			}

			int y = 5;

			if (c::misc::watermark) {
				if (c::misc::show_spotify_currently_playing && c::misc::player_type == 0)
					y = 45;
				else if (c::misc::show_spotify_currently_playing && c::misc::player_type == 1)
					if (c::misc::progressbar_enable)
						y = 77;
					else
						y = 68;
				else
					y = 30;
			}
			else if (c::misc::show_spotify_currently_playing && c::misc::player_type == 0) {
				y = 20;
			}
			else if (c::misc::show_spotify_currently_playing && c::misc::player_type == 1) {
				if (c::misc::progressbar_enable)
					y = 52;
				else
					y = 43;
			}

			ImColor spec_clr = ImColor(c::misc::spectators_list_color_2[0], c::misc::spectators_list_color_2[1], c::misc::spectators_list_color_2[2]);
			ImColor local_spec_clr = ImColor(c::misc::spectators_list_color_1[0], c::misc::spectators_list_color_1[1], c::misc::spectators_list_color_1[2]);

			auto text_size = im_render.get_text_size(spectate_buffer, fonts::main_spec_font, 0.f, 12.f);

			ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(x - 6 - text_size + 1, (y + 1) + (16 * height)), ImColor(0, 0, 0, 255), spectate_buffer);
			ImGui::GetBackgroundDrawList()->AddText(fonts::main_spec_font, 12.f, ImVec2(x - 6 - text_size, y + (16 * height)), spectated_player == g::local ? local_spec_clr : spec_clr, spectate_buffer);
			height++;
		}
	}	
}

vec3_t origin;
float pressdelay = 0.f;
int currentcheckpoint = 0;
std::deque<std::pair<vec3_t, vec3_t>> checkpoints;
void features::misc::kz_practice_logic(c_usercmd* cmd) {
	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !c::misc::practice) {
		checkpoints.clear();
		currentcheckpoint = 0;
		pressdelay = 0;
		return;
	}

	float j = interfaces::console->get_convar("sv_cheats")->get_float();
	if (j == 1) {
		if (g::local->is_alive() && pressdelay < interfaces::globals->realtime) {
			std::stringstream ss;
			if (GetAsyncKeyState(c::misc::savepos) && (g::local->flags() & fl_onground)) {
				checkpoints.push_front({ vec3_t(cmd->view_angles.x, cmd->view_angles.y, 0.f), local_player->origin() });
				currentcheckpoint = 0;
				if (checkpoints.size() > 99)
					checkpoints.pop_back();
				pressdelay = interfaces::globals->realtime + 0.2;
				for (int i = 1; i <= 1; ++i) {
					interfaces::chat_element->chatprintf("#delusional#_print_saved_c");

				}
			}
			else if (GetAsyncKeyState(c::misc::savepos) && !(g::local->flags() & fl_onground)){
				pressdelay = interfaces::globals->realtime + 0.2;
				for (int i = 1; i == 1; ++i) {
					interfaces::chat_element->chatprintf("#delusional#_print_save_c");
				}
			}

			if (!checkpoints.empty()) {
				if (menu::checkkey(c::misc::loadpos, c::misc::loadpos_s)) {
					auto& cur = checkpoints.at(currentcheckpoint);
					std::string cmd = ("setpos ") + std::to_string(cur.second.x) + (" ") + std::to_string(cur.second.y) + (" ") + std::to_string(cur.second.z) + (";");
					interfaces::engine->execute_cmd(cmd.c_str());
					cmd = ("setang ") + std::to_string(cur.first.x) + (" ") + std::to_string(cur.first.y) + (" ") + std::to_string(cur.first.z) + (";");
					interfaces::engine->execute_cmd(cmd.c_str());
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						interfaces::chat_element->chatprintf("#delusional#_print_tp_c");
					}
				}
				if (menu::checkkey(c::misc::nextkey, c::misc::nextkey_s)) {
					if (currentcheckpoint > 0)
						currentcheckpoint--;
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						interfaces::chat_element->chatprintf("#delusional#_print_next_c");
					}
				}
				if (menu::checkkey(c::misc::prevkey, c::misc::prevkey_s)) {
					if (currentcheckpoint + 1 < checkpoints.size())
						currentcheckpoint++;
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						interfaces::chat_element->chatprintf("#delusional#_print_prev_c");
					}
				}
				if (menu::checkkey(c::misc::undokey, c::misc::undokey_s)) {
					checkpoints.erase(checkpoints.begin() + currentcheckpoint);
					if (currentcheckpoint >= checkpoints.size())
						currentcheckpoint = max(0, checkpoints.size() - 1);
					pressdelay = interfaces::globals->realtime + 0.2;
					for (int i = 1; i <= 1; ++i) {
						interfaces::chat_element->chatprintf("#delusional#_print_undid_c");
					}
				}
			}
		}
	}
}

void features::misc::kz_practice() {
	if (!c::misc::practice_window )
		return;

}

void features::misc::reveal_server_ranks(c_usercmd * cmd) {
	if (!c::misc::misc_reveal_ranks || !interfaces::engine->is_in_game())
		return;

	if (cmd->buttons & in_score)
		interfaces::client->dispatch_user_message(50, 0, 0, nullptr);
}

void features::misc::thirdperson() {

}

void features::misc::reset_thirdperson() {

}

void features::misc::view_model() {
	if (c::misc::view_model) {
		if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
			return;

		if (!g::local || !g::local->is_alive())
			return;

		const auto view_model = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(g::local->view_model()));

		if (!view_model)
			return;

		vec3_t forward{}, right{}, up{};
		math::angle_vectors_alternative(view_model->abs_angles(), &forward, &right, &up);

		view_model->set_abs_origin(view_model->abs_origin() + forward * c::misc::view_model_z + up * c::misc::view_model_y + right * c::misc::view_model_x);
	}

	//

	static auto default_scale = interfaces::console->get_convar("cl_wpn_sway_scale")->get_float();
	static auto* swayscale = interfaces::console->get_convar("cl_wpn_sway_scale");

	swayscale->set_value(c::misc::swayscale ? c::misc::swayscale_value : default_scale);
}

void features::misc::door_spam(c_usercmd* cmd) {
	if (!c::misc::door_spam || !menu::checkkey(c::misc::door_spam_key, c::misc::door_spam_key_s))
		return;

	static bool spam = false;
	static float time = 0;
	if (interfaces::globals->cur_time > time) {

		cmd->buttons &= in_use;

		time = interfaces::globals->cur_time + 0.2f;
	}
	if (fabs(time - interfaces::globals->cur_time) > 0.3f)
		time = interfaces::globals->cur_time;
}

void features::misc::fix_mouse_delta(c_usercmd* cmd) {
	if (!c::misc::mouse_fix)
		return;

	static vec3_t delta_viewangles{ };
	vec3_t delta = cmd->view_angles - delta_viewangles;
	delta.clamp();

	static convar* sensitivity = interfaces::console->get_convar("sensitivity");

	if (!sensitivity)
		return;

	if (delta.x != 0.f) {
		static convar* m_pitch = interfaces::console->get_convar("m_pitch");

		if (!m_pitch)
			return;

		int final_dy = static_cast<int>((delta.x / m_pitch->get_float()) / sensitivity->get_float());
		if (final_dy <= 32767) {
			if (final_dy >= -32768) {
				if (final_dy >= 1 || final_dy < 0) {
					if (final_dy <= -1 || final_dy > 0)
						final_dy = final_dy;
					else
						final_dy = -1;
				}
				else {
					final_dy = 1;
				}
			}
			else {
				final_dy = 32768;
			}
		}
		else {
			final_dy = 32767;
		}

		cmd->mouse_dy = static_cast<short>(final_dy);
	}

	if (delta.y != 0.f) {
		static convar* m_yaw = interfaces::console->get_convar("m_yaw");

		if (!m_yaw)
			return;

		int final_dx = static_cast<int>((delta.y / m_yaw->get_float()) / sensitivity->get_float());
		if (final_dx <= 32767) {
			if (final_dx >= -32768) {
				if (final_dx >= 1 || final_dx < 0) {
					if (final_dx <= -1 || final_dx > 0)
						final_dx = final_dx;
					else
						final_dx = -1;
				}
				else {
					final_dx = 1;
				}
			}
			else {
				final_dx = 32768;
			}
		}
		else {
			final_dx = 32767;
		}

		cmd->mouse_dx = static_cast<short>(final_dx);
	}

	delta_viewangles = cmd->view_angles;
}

void features::misc::radaringame(player_t* p) {
	

}

void apply_clan_tag(const char* tag, const char* name) {
	using fn = int(__fastcall*)(const char*, const char*);
	static auto apply_clan_tag_fn = reinterpret_cast<fn>(find_pattern("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(tag, name);
};

bool r3set = false;
void features::misc::clantag_spammer() {
	if (!interfaces::engine->is_in_game()) {
		return;
	}
	if (!c::misc::misc_clantag_spammer) {
		if (r3set) {
			apply_clan_tag(xs(""), xs(""));
			r3set = false;
		}
		return;
	}

	r3set = true;
	static bool reset = true;
	static auto lasttime = 0.0f;
	static std::string torotate = c::misc::misc_clantag_text;
	static std::string rotating = c::misc::misc_clantag_text;

	if (c::misc::misc_clantag_type == 0) {
		if (interfaces::globals->realtime - lasttime < 1.0f)
			return;

		apply_clan_tag(xs("delusional"), xs("delusional"));
		lasttime = interfaces::globals->realtime;
	}
	else if (c::misc::misc_clantag_type == 1) {
		if (c::misc::misc_animated_clantag) {
			if (interfaces::globals->realtime - lasttime < c::misc::misc_clantag_speed)
				return;

			if (torotate != std::string(c::misc::misc_clantag_text)) {
				torotate = c::misc::misc_clantag_text;
				rotating = c::misc::misc_clantag_text;
			}

			if (c::misc::misc_clantag_rotation) {
				if (!rotating.empty()) {
					char last = rotating.back();
					rotating.pop_back();
					rotating.insert(rotating.begin(), last);
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}
			else {
				if (!rotating.empty()) {
					std::rotate(rotating.begin(), rotating.begin() + (rotating.size() - 1), rotating.end());
					apply_clan_tag(rotating.c_str(), torotate.c_str());
				}
			}

			lasttime = interfaces::globals->realtime;
		}
		else {
			if (interfaces::globals->realtime - lasttime < 1.0f)
				return;

			apply_clan_tag(c::misc::misc_clantag_text, c::misc::misc_clantag_text);
			lasttime = interfaces::globals->realtime;
		}
	}
}

inline int hitmarker_time = 0;
void features::misc::hitmarker::event(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));

	if (!attacker || !event_name)
		return;

	switch (event_name) {
	case fnv::hash("player_hurt"):
		if (attacker == g::local) {
			hitmarker_time = 255;

			rng_factor::player_ishurt = true;

			if (c::misc::misc_hitmarker_sound) {

				switch (c::misc::misc_hitmarker_sound_type) {
				case 0: interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav"); break;
				case 1: interfaces::surface->play_sound("buttons\\button22.wav"); break;
				case 2: interfaces::surface->play_sound("survival\\money_collect_01.wav"); break;
				case 3: interfaces::surface->play_sound("Ui\\beep07.wav"); break;
				}
			}
		}
		break;
	}
}

const char* hitgroup_name(int hitgroup) {
	switch (hitgroup) {
	case hitgroup_head:
		return "head";
	case hitgroup_leftleg:
		return "left leg";
	case hitgroup_rightleg:
		return "right leg ";
	case hitgroup_stomach:
		return "stomach";
	case hitgroup_leftarm:
		return "left arm";
	case hitgroup_rightarm:
		return "right arm";
	default:
		return "body";
	}
}

void features::misc::hit_info(i_game_event* event) {

	if (!c::misc::misc_hit_info || !interfaces::engine->is_in_game())
		return;

	if (!event)
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return;

	auto victim = interfaces::engine->get_player_for_user_id(event->get_int("userid"));
	if (!victim)
		return;

	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker)
		return;

	if (attacker == local_player) {
		auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(victim));
		if (!entity) {
			return;
		}

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		hitinfo.damage = event->get_int("dmg_health");
		if (!hitinfo.damage)
			return;

		hitinfo.health = event->get_int("health");
		if (!hitinfo.health && hitinfo.health != 0)
			return;

		auto hitbox = event->get_int("hitgroup");
		if (!hitbox) {
			hitinfo.player_name = info.name;
			if (c::misc::misc_hit_info[0])
				interfaces::chat_element->chatprintf("#delusional#_print_hit_1");

			    //interfaces::chat_element->chatprintf(0, 0, " %c delusional \x08| hit %s for %d damage (%d remaining). \n", menu::color_p, hitinfo.player_name.c_str(), hitinfo.damage, hitinfo.health);

			if (c::misc::misc_hit_info[1])
			    features::misc::notify((std::stringstream{ } << "it " << hitinfo.player_name.c_str() << " for " << hitinfo.damage << " damage (" << hitinfo.health << " health remaining).").str().c_str(), color(255, 255, 255, 255));
			return;
		}

		hitinfo.hitgroup = hitgroup_name(hitbox);

		hitinfo.player_name = info.name;

		if (c::misc::misc_hit_info[0])
			interfaces::chat_element->chatprintf("#delusional#_print_hit_2");

		    //interfaces::chat_element->chatprintf(0, 0, " %c delusional \x08| hit %s in the %s for %d damage (%d remaining). \n", menu::color_p, hitinfo.player_name.c_str(), hitinfo.hitgroup, hitinfo.damage, hitinfo.health);

		if (c::misc::misc_hit_info[1])
		    features::misc::notify((std::stringstream{ } << "hit " << hitinfo.player_name.c_str() << " in the " << hitinfo.hitgroup << " for " << hitinfo.damage << " damage (" << hitinfo.health << " health remaining).").str().c_str(), color(255, 255, 255, 255));

	}
}

void features::misc::hitmarker::draw() {
	if (!c::misc::misc_hitmarker)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;

		surface::line((width / 2) + 6, (height / 2) + 6, (width / 2) + 3, (height / 2) + 3, color::white(alpha));
		surface::line((width / 2) - 6, (height / 2) + 6, (width / 2) - 3, (height / 2) + 3, color::white(alpha));
		surface::line((width / 2) + 6, (height / 2) - 6, (width / 2) + 3, (height / 2) - 3, color::white(alpha));
		surface::line((width / 2) - 6, (height / 2) - 6, (width / 2) - 3, (height / 2) - 3, color::white(alpha));

		hitmarker_time -= 2;
	}
}

void features::misc::kill_say(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	auto entity = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid"))));

	if (!attacker || !entity || !event_name)
		return;

	switch (event_name) {
	case fnv::hash("player_death"):
		if (attacker == g::local && entity != g::local) {

			rng_factor::players_iskilled = true;

			if (c::misc::misc_kill_msg)
				interfaces::engine->execute_cmd((std::string("say ").append(c::misc::misc_kill_message)).c_str());

			if (c::misc::misc_hitmarker_screen_effect)
				g::local->health_boost_time() = interfaces::globals->cur_time + 0.4f;
		}
		break;
	}
}

void features::misc::force_crosshair() {
	const static auto weapon_debug_spread_show = interfaces::console->get_convar(xs("weapon_debug_spread_show"));

	const auto should_draw_crosshair = c::misc::force_crosshair && g::local &&
		g::local->is_alive() &&
		!g::local->is_scoped();

	weapon_debug_spread_show->set_value(should_draw_crosshair ? 3 : 0);
}

void features::misc::sniper_crosshair() {
	if (!c::misc::sniper_crosshair)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!g::local || !g::local->is_alive())
		return;

	const auto weapon = g::local->active_weapon();

	if (!weapon) 
		return;

	const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());

	if (!data) 
		return;

	if (data->m_iWeaponType != WEAPONTYPE_SNIPER_RIFLE)
		return;

	if (g::local->is_scoped()) 
		return;

	int x, y;
	interfaces::engine->get_screen_size(x, y);

	im_render.drawrectfilled(x / 2, y / 2, 2, 2, color_t(255, 255, 255));
}

void features::misc::vote_revealer(i_game_event* event) {
	/*
	if (!c::misc::vote_revealer || !interfaces::engine->is_in_game())
		return;

	int vote = event->get_int("vote_option");
	int id = event->get_int("entityid");
	player_info_t player;
	interfaces::engine->get_player_info(id, &player);
	interfaces::chat_element->chatprintf(0, 0, " \x07 %s\x08 voted %s\x08.\n", player.name, vote == 0 ? "\x07yes" : "\x02no");
	*/
}

void features::misc::handle_spotify() {
	/*static HWND spotify_hwnd = nullptr;
	static float last_hwnd_time = 0.f;
	if ((!spotify_hwnd || spotify_hwnd == INVALID_HANDLE_VALUE) && last_hwnd_time < interfaces::globals->realtime + 2.f) {
		for (HWND hwnd = GetTopWindow(0); hwnd; hwnd = GetWindow(hwnd, GW_HWNDNEXT)) {
			last_hwnd_time = interfaces::globals->realtime;

			if (!(IsWindowVisible)(hwnd))
				continue;

			int length = (GetWindowTextLengthW)(hwnd);
			if (length == 0)
				continue;

			WCHAR filename[300];
			DWORD pid;
			(GetWindowThreadProcessId)(hwnd, &pid);

			const auto spotify_handle = (OpenProcess)(PROCESS_QUERY_INFORMATION, FALSE, pid);
			(K32GetModuleFileNameExW)(spotify_handle, nullptr, filename, 300);

			std::wstring sane_filename{ filename };

			(CloseHandle)(spotify_handle);

			if (sane_filename.find((L"Spotify.exe")) != std::string::npos)
				spotify_hwnd = hwnd;
		}
	}
	else if (spotify_hwnd && spotify_hwnd != INVALID_HANDLE_VALUE) {
		WCHAR title[300];

		if (!(GetWindowTextW)(spotify_hwnd, title, 300)) {
			spotify_hwnd = nullptr;
		}
		else {
			std::wstring sane_title{ title };
			std::string song(sane_title.begin(), sane_title.end());
			if (sane_title.find((L"-")) != std::string::npos) {
				features::visuals::current_spotify_song = song;
			}
			else if (sane_title.find((L"Advertisment")) != std::string::npos) {
				features::visuals::current_spotify_song = (" advertisment");
			}
			else if (sane_title.find((L"Spotify")) != std::string::npos) {
				features::visuals::current_spotify_song = (" stopped / not playing");
			}
			else {
				features::visuals::current_spotify_song = (" advertisment");
			}
		}
	}

	static std::string old_song = features::visuals::current_spotify_song;
	if (old_song != features::visuals::current_spotify_song) {
		if (c::misc::show_spotify_currently_playing) {
			if (features::visuals::current_spotify_song.find(" stopped / not playing") != std::string::npos)
				interfaces::chat_element->chatprintf("#delusional#_spotify_paused");
			else if (features::visuals::current_spotify_song.find(" advertisment") != std::string::npos)
				interfaces::chat_element->chatprintf("#delusional#_spotify_advertisement");
			else
				interfaces::chat_element->chatprintf("#delusional#_spotify_switch");
		}

		old_song = features::visuals::current_spotify_song;
	}*/
}


