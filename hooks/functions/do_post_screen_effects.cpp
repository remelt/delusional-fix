#include "../hooks.hpp"
#include "../../menu/menu.hpp"
#include "../../sdk/math/math.hpp"
#include "../../features/visuals/visuals.hpp"

bool __stdcall sdk::hooks::do_post_screen_effects::do_post_screen_effects(view_setup_t* setup) {

	features::visuals::gravity_ragdoll();
	features::visuals::glow::run();

	return ofunc(interfaces::client_mode, setup);
}