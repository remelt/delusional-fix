#include "events.hpp"

#include "../misc/misc.hpp"
#include "../../features/panorama/scaleform.hpp"
#include "../../features/movement/movement.hpp"

hooked_events events;

void hooked_events::fire_game_event(i_game_event* event) {
	auto event_name = event->get_name();

	panorama::scaleform_on_event(event);

	if (!strcmp(event_name, "player_hurt")) {
		features::misc::hitmarker::event(event);
		features::misc::hit_info(event);	}
	if (!strcmp(event_name, "player_death")) {
		features::misc::kill_say(event);
	}
	if (!strcmp(event_name, "round_prestart")) {
		recorder->forcestop();
	}

	panorama::scaleform_after_event(event_name);
}