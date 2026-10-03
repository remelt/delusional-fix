#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::override_view::override_view( void* _this, void* _edx, view_setup_t* setup ) {
	if (c::misc::enable_fov && g::local && g::local->is_alive( ) && !g::local->is_scoped())
		setup->fov += c::misc::field_of_view;
	
	features::misc::view_model();
	features::visuals::freecam(setup);

	ofunc( interfaces::client_mode, _this, setup );
}

void __fastcall sdk::hooks::draw_view_models::draw_view_models(void* ecx, void* edx, view_setup_t& setup, bool draw_view_model, bool draw_scope_lens_mask) {
	
	features::visuals::motion_blur(&setup);

	if (c::visuals::apply_zoom && g::local && g::local->fov() < 45 && g::local->fov_start() < 45)
		draw_view_model = false;

	ofunc(ecx, edx, setup, draw_view_model, draw_scope_lens_mask);
}