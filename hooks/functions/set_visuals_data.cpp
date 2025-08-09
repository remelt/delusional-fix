#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/skins/skins.hpp"

void __fastcall sdk::hooks::set_visuals_data::set_visuals_data(void* ecx, void* edx, const char* shader_name) {
	ofunc(ecx, edx, shader_name);
	auto m_visuals_data = reinterpret_cast<wpn_visual_data*>((uintptr_t(ecx) - 0x4));
	std::string buffer;

	if (!m_visuals_data)
		return;

	for (int i = 0; i <= 260; i++)
		buffer.append(&m_visuals_data->weapon_path[i]);

	if (!strstr(buffer.c_str(), ("knife")))
		return;

	if (!c::skins::knife_changer_enable)
		return;

	if (c::skins::knife_changer_model == 0)
		return;

	if (!c::skins::skin_custom_clr)
		return;

	m_visuals_data->color1_r = c::skins::skin_modulation1[2] * 255;
	m_visuals_data->color1_g = c::skins::skin_modulation1[0] * 255;
	m_visuals_data->color1_b = c::skins::skin_modulation1[1] * 255;

	m_visuals_data->color2_r = c::skins::skin_modulation2[2] * 255;
	m_visuals_data->color2_g = c::skins::skin_modulation2[0] * 255;
	m_visuals_data->color2_b = c::skins::skin_modulation2[1] * 255;

	m_visuals_data->color3_r = c::skins::skin_modulation3[2] * 255;
	m_visuals_data->color3_g = c::skins::skin_modulation3[0] * 255;
	m_visuals_data->color3_b = c::skins::skin_modulation3[1] * 255;

	m_visuals_data->color4_r = c::skins::skin_modulation4[2] * 255;
	m_visuals_data->color4_g = c::skins::skin_modulation4[0] * 255;
	m_visuals_data->color4_b = c::skins::skin_modulation4[1] * 255;
}
