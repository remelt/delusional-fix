#include "../hooks.hpp"
#include "../../menu/config/config.hpp"

float __fastcall sdk::hooks::get_screen_aspect_ratio::get_screen_aspect_ratio(registers, int width, int height) {
	if (!c::misc::aspect_ratio)
		return ofunc( ecx, edx, width, height );

	return c::misc::aspect_ratio_amount;
}