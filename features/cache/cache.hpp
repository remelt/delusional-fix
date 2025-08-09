#pragma once
#include "../../sdk/sdk.hpp"
#include "../../hooks/hooks.hpp"

enum e_cache_types {
	cache_type_player = 0,
	cache_type_edicts
};

struct cache_t {
	cache_t() {
		m_edicts.reserve(2048);
		m_players.reserve(64);
	}

	std::vector< player_t* > m_edicts = { };
	std::vector< player_t* > m_players = { };
};

inline cache_t cache_s = { };

namespace features::cache {
	void on_add_entity(player_t* entity);
	void on_remove_entity(player_t* entity);

	//void enumerate(e_cache_types type, const std::function< void(player_t*) >& function) const;
}