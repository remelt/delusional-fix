#include "cache.hpp"

void features::cache::on_add_entity(player_t* entity) {
	if (!entity || entity->index() == interfaces::engine->get_local_player())
		return;

	if (!entity->is_player()) {
		const auto it_found = std::ranges::find(cache_s.m_edicts, entity);
		if (it_found == cache_s.m_edicts.cend() && entity->index() >= 1 && entity->index() <= 2048)
			cache_s.m_edicts.push_back(entity);
	}
	else {
		const auto it_found = std::ranges::find(cache_s.m_players, entity);
		if (it_found == cache_s.m_players.cend() && entity->index() >= 1 && entity->index() <= 64)
			cache_s.m_players.push_back(entity);
	}
}

void features::cache::on_remove_entity(player_t* entity) {
	if (!entity || entity->index() == interfaces::engine->get_local_player())
		return;

	if (!entity->is_player()) {
		const auto it_found = std::ranges::find(cache_s.m_edicts, entity);
		if (it_found != cache_s.m_edicts.cend())
			cache_s.m_edicts.erase(it_found);
	}
	else {
		const auto it_found = std::ranges::find(cache_s.m_players, entity);
		if (it_found != cache_s.m_players.cend())
			cache_s.m_players.erase(it_found);
	}
}