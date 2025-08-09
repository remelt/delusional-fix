#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/panorama/scaleform.hpp"
#include "../../features/events/events.hpp"

bool __stdcall sdk::hooks::fire_event::fire_event(i_game_event* gameEvent) {

	events.fire_game_event(gameEvent);

	return sdk::hooks::fire_event::ofunc(interfaces::event_manager, gameEvent);
}