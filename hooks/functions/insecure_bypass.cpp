#include "../hooks.hpp"
#include "../../features/visuals/visuals.hpp"

char __stdcall sdk::hooks::insecure::insecure_bypass()
{
	//https://www.unknowncheats.me/forum/counterstrike-global-offensive/487890-insecure-mode-bypass.html
	if (c::misc::insecure_bypass) {
		return true;
	}
	return sdk::hooks::insecure::ofunc();
}

void* __fastcall sdk::hooks::get_vcollide::get_vcollide(void* ecx, void* edx, int model_index)
{

	if (model_index == -1)
		return features::weather::getv_collideble();

	return ofunc(ecx, edx, model_index);
}