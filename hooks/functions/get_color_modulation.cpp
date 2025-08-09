#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/structs/materials.hpp"

void __fastcall sdk::hooks::get_color_modulation::get_color_modulation(i_material* material, void* edx, float* r, float* g, float* b) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx, r, g, b);

	ofunc(material, edx, r, g, b);

	if (!c::visuals::world_modulate)
		return ofunc(material, edx, r, g, b);

	if (!material || material->is_error_material())
		return ofunc(material, edx, r, g, b);

	const auto group = material->get_texture_group_name();

	if (strstr(group, ("World textures"))
		|| strstr(group, ("StaticProp textures"))
		|| strstr(group, ("SkyBox textures"))) {
		const auto is_prop = group == ("StaticProp textures");

		*r *= is_prop ? 0.5f * (c::visuals::world_color[0]) : 0.23f * (c::visuals::world_color[0]);
		*g *= is_prop ? 0.5f * (c::visuals::world_color[1]) : 0.23f * (c::visuals::world_color[1]);
		*b *= is_prop ? 0.5f * (c::visuals::world_color[2]) : 0.23f * (c::visuals::world_color[2]);
	}
}

float __fastcall sdk::hooks::get_alpha_modulation::get_alpha_modulation(i_material* material, void* edx) {
	if (!g::local || !interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return ofunc(material, edx);

	if (!c::visuals::world_modulate)
		return ofunc(material, edx);

	if (!material || material->is_error_material())
		return ofunc(material, edx);

	float alpha = c::visuals::world_color[3];

	const auto group = material->get_texture_group_name();
	if (strstr(group, ("World textures"))
		|| strstr(group, ("StaticProp textures"))
		|| strstr(group, ("SkyBox textures")))
		return alpha;

	return ofunc(material, edx);
}
