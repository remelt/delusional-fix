#include "../hooks.hpp"
#include "../../menu/config/config.hpp"

bool __stdcall sdk::hooks::is_connected::is_connected() {
	static void* force_inventory_open = find_pattern("client.dll", "84 C0 75 05 B0 01 5F") - 2;
	if (!interfaces::engine || !interfaces::engine->is_in_game())
		return ofunc(interfaces::engine);

	if (c::misc::unlock_inventory && _ReturnAddress() == force_inventory_open) {
		return false;
	}

	return ofunc(interfaces::engine);
}