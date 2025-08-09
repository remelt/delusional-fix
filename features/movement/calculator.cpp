#include "movement.hpp"
#include "../../menu/menu.hpp"
#include "../../menu/config/config.hpp"
#include "../movement/prediction/prediction.hpp"

void features::calculator::draw_info() {


}

void features::calculator::run() {
	if (!c::calculator::ps_calcualtor) {
		return;
	}

	if (!g::local || !g::local->is_alive() || !interfaces::engine->is_in_game()) {
		return;
	}

	static convar* mat_wirefram = interfaces::console->get_convar("mat_wirefram");
	static convar* r_drawclipbrushes = interfaces::console->get_convar("r_drawclipbrushes");

	if (menu::checkkey(c::calculator::toggle_wireframe_key, c::calculator::toggle_wireframe_key_s))
		mat_wirefram->set_value(2);
	else
		mat_wirefram->set_value(0);

	if (menu::checkkey(c::calculator::toggle_clipbrushe_key, c::calculator::toggle_clipbrushe_key_s))
		r_drawclipbrushes->set_value(2);
	else
		r_drawclipbrushes->set_value(0);


    int x, y;
    static int alpha = 0;
    if (alpha < 0)
        alpha = 0;
    if (alpha > 255)
        alpha = 255;

}