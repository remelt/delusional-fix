#include "../hooks.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::draw_set_color::draw_set_color(void* ecx, void* edx, int r, int g, int b, int a) {
	/*
	if (interfaces::engine->is_console_visible() && c::misc::custom_console) {
		int new_r = min(255, r + (c::misc::custom_console_clr[0] * 255));
		int new_g = min(255, g + (c::misc::custom_console_clr[1] * 255));
		int new_b = min(255, b + (c::misc::custom_console_clr[2] * 255));
		ofunc(ecx, edx, new_r, new_g, new_b, a);
		return;
	}

	ofunc(ecx, edx, r, g, b, a);
	*/
}