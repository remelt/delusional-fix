#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/movement/lobotomy_eb.h"

void __fastcall sdk::hooks::override_mouse_input::override_mouse_input( void* this_, int edx, float* x, float* y ) {
	lobotomy_eb::edgebug_lock(*x, *y);
	recorder->camera_lock(*x, *y);
	if (features::movement::detect_data.ticks_left) {
		switch (c::movement::edge_bug_lock_type) {
		case 0: //static
			*x *= 1 - c::movement::edge_bug_lock_amount / 100;
			break;
		case 1: 
			*x *= 0;
			break;
		}
	}

	features::movement::mouse_strafe_limiter(x, y);

	return ofunc( this_, edx, x, y );
}