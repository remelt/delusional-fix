#include "../visuals/visuals.hpp"
#include "../../menu/config/config.hpp"

void features::visuals::glow::run() {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];
		if (glow.unused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.entity);
		auto client_class = glow_entity->client_class();
		if (!glow_entity || glow_entity->dormant())
			continue;

		auto is_enemy = glow_entity->team() != g::local->team();

		switch (client_class->class_id) {
		case ccsplayer: {
			if (c::visuals::glow) {

				const auto player = static_cast<player_t*>(glow.entity);

				if (player->team() == g::local->team())
					continue;

				if (player->visible()) {
					glow.glow_style = c::visuals::glow_style;
					glow.set(c::visuals::glow_visible_clr[0], c::visuals::glow_visible_clr[1], c::visuals::glow_visible_clr[2], c::visuals::glow_visible_clr[3]);
				}
				else {
					glow.set(c::visuals::glow_invisible_clr[0], c::visuals::glow_invisible_clr[1], c::visuals::glow_invisible_clr[2], c::visuals::glow_invisible_clr[3]);
				}
			}
			
			break;
		}
		default: {
			if ((client_class->class_id >= 232 && client_class->class_id <= 273) || client_class->class_id == cak47 || client_class->class_id == cdeagle) {
				if (c::visuals::glow_weapon)
					glow.set(c::visuals::glow_weapon_clr[0], c::visuals::glow_weapon_clr[1], c::visuals::glow_weapon_clr[2], c::visuals::glow_weapon_clr[3]);
				break;
			}
		}
		}
	}
}