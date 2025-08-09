#include "../hooks.hpp"
#include "../../menu/menu.hpp"

void __fastcall sdk::hooks::lock_cursor::lock_cursor( i_surface* surface ) {
	if (menu::open) {
		interfaces::surface->unlock_cursor( );
		interfaces::input_system->enable_input( false );
		return;
	}
	interfaces::input_system->enable_input( true );	
	ofunc( surface );
}