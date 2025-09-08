#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/misc/misc.hpp"

void __stdcall sdk::hooks::paint_traverse::paint_traverse(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):

		//features::misc::handle_spotify();
		features::misc::hitmarker::draw();
		features::misc::spectators_list_iwebz();

		break;
	case fnv::hash("FocusOverlayPanel"):

		if (interfaces::engine->is_in_game() && interfaces::engine->is_connected() && g::local) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					math::view_matrix[i][j] = interfaces::engine->world_to_screen_matrix()[i][j];
		}

		im_render.add_to_render([]() {
			features::misc::sniper_crosshair();
			features::visuals::key_strokes();
			features::visuals::player::run();
			features::grenade_prediction::draw();
			features::movement::stamina_graph_draw();
			features::movement::velocity_graph_draw();
			features::movement::velocity_indicator();
			features::movement::stamina_indicator();
			features::movement::visualize_eb();
			features::movement::indicators();
			if (c::assist::assist) {
				features::movement::check_ps(nullptr);
				features::movement::assist_render();
				features::movement::RenderPoints();
			}
			recorder->drawroute();
			//features::visuals::rng_factor();
		});

		break;

	case fnv::hash("GameConsole"): case fnv::hash("CompletionList"):

		features::visuals::console_vis = true;
		ofunc(interfaces::panel, panel, force_repaint, allow_force);
		features::visuals::console_vis = false;

		return;
	}
	ofunc(interfaces::panel, panel, force_repaint, allow_force);
}