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

	if (strstr(buffer.c_str(), ("knife"))) {
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
	else if (strstr(buffer.c_str(), ("223"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::usp_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::usp_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::usp_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::usp_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::usp_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::usp_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::usp_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::usp_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::usp_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::usp_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::usp_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::usp_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::usp_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("hkp2000"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::p2000_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::p2000_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::p2000_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::p2000_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::p2000_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::p2000_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::p2000_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::p2000_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::p2000_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::p2000_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::p2000_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::p2000_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::p2000_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("glock18"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::glock_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::glock_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::glock_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::glock_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::glock_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::glock_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::glock_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::glock_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::glock_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::glock_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::glock_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::glock_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::glock_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("p250"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::p250_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::p250_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::p250_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::p250_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::p250_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::p250_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::p250_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::p250_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::p250_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::p250_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::p250_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::p250_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::p250_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("fiveseven"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::fiveseven_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::fiveseven_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::fiveseven_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::fiveseven_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::fiveseven_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::fiveseven_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::fiveseven_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::fiveseven_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::fiveseven_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::fiveseven_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::fiveseven_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::fiveseven_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::fiveseven_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("tec9"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::tec_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::tec_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::tec_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::tec_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::tec_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::tec_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::tec_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::tec_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::tec_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::tec_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::tec_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::tec_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::tec_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("cz_75"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::cz_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::cz_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::cz_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::cz_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::cz_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::cz_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::cz_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::cz_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::cz_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::cz_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::cz_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::cz_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::cz_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("elite"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::duals_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::duals_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::duals_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::duals_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::duals_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::duals_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::duals_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::duals_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::duals_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::duals_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::duals_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::duals_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::duals_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("deagle"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::deagle_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::deagle_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::deagle_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::deagle_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::deagle_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::deagle_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::deagle_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::deagle_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::deagle_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::deagle_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::deagle_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::deagle_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::deagle_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("revolver"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::revolver_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::revolver_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::revolver_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::revolver_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::revolver_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::revolver_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::revolver_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::revolver_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::revolver_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::revolver_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::revolver_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::revolver_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::revolver_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("famas"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::famas_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::famas_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::famas_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::famas_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::famas_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::famas_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::famas_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::famas_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::famas_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::famas_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::famas_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::famas_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::famas_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("galilar"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::galil_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::galil_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::galil_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::galil_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::galil_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::galil_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::galil_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::galil_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::galil_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::galil_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::galil_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::galil_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::galil_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("m4a1"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::m4a4_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::m4a4_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::m4a4_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::m4a4_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::m4a4_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::m4a4_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::m4a4_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::m4a4_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::m4a4_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::m4a4_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::m4a4_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::m4a4_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::m4a4_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("m4a1_s"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::m4a1_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::m4a1_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::m4a1_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::m4a1_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::m4a1_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::m4a1_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::m4a1_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::m4a1_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::m4a1_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::m4a1_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::m4a1_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::m4a1_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::m4a1_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("ak47"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::ak47_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::ak47_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::ak47_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::ak47_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::ak47_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::ak47_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::ak47_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::ak47_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::ak47_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::ak47_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::ak47_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::ak47_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::ak47_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("sg556"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::sg553_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::sg553_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::sg553_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::sg553_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::sg553_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::sg553_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::sg553_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::sg553_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::sg553_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::sg553_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::sg553_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::sg553_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::sg553_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("aug"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::aug_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::aug_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::aug_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::aug_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::aug_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::aug_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::aug_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::aug_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::aug_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::aug_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::aug_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::aug_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::aug_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("ssg08"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::ssg08_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::ssg08_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::ssg08_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::ssg08_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::ssg08_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::ssg08_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::ssg08_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::ssg08_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::ssg08_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::ssg08_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::ssg08_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::ssg08_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::ssg08_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("awp"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::awp_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::awp_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::awp_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::awp_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::awp_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::awp_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::awp_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::awp_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::awp_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::awp_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::awp_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::awp_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::awp_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("scar20"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::scar_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::scar_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::scar_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::scar_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::scar_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::scar_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::scar_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::scar_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::scar_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::scar_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::scar_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::scar_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::scar_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("g3sg1"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::g3sg1_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::g3sg1_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::g3sg1_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::g3sg1_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::g3sg1_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::g3sg1_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::g3sg1_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::g3sg1_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::g3sg1_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::g3sg1_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::g3sg1_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::g3sg1_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::g3sg1_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("sawedoff"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::sawoff_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::sawoff_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::sawoff_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::sawoff_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::sawoff_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::sawoff_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::sawoff_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::sawoff_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::sawoff_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::sawoff_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::sawoff_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::sawoff_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::sawoff_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("m249para"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::m249_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::m249_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::m249_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::m249_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::m249_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::m249_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::m249_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::m249_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::m249_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::m249_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::m249_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::m249_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::m249_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("negev"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::negev_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::negev_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::negev_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::negev_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::negev_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::negev_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::negev_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::negev_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::negev_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::negev_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::negev_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::negev_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::negev_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("mag7"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::mag7_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::mag7_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::mag7_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::mag7_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::mag7_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::mag7_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::mag7_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::mag7_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::mag7_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::mag7_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::mag7_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::mag7_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::mag7_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("xm1014"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::xm1014_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::xm1014_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::xm1014_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::xm1014_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::xm1014_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::xm1014_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::xm1014_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::xm1014_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::xm1014_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::xm1014_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::xm1014_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::xm1014_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::xm1014_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("nova"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::nova_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::nova_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::nova_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::nova_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::nova_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::nova_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::nova_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::nova_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::nova_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::nova_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::nova_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::nova_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::nova_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("bizon"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::bizon_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::bizon_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::bizon_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::bizon_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::bizon_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::bizon_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::bizon_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::bizon_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::bizon_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::bizon_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::bizon_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::bizon_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::bizon_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("mp5sd"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::mp5sd_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::mp5sd_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::mp5sd_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::mp5sd_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::mp5sd_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::mp5sd_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::mp5sd_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::mp5sd_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::mp5sd_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::mp5sd_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::mp5sd_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::mp5sd_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::mp5sd_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("mp7"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::mp7_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::mp7_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::mp7_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::mp7_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::mp7_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::mp7_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::mp7_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::mp7_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::mp7_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::mp7_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::mp7_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::mp7_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::mp7_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("mp9"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::mp9_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::mp9_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::mp9_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::mp9_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::mp9_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::mp9_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::mp9_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::mp9_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::mp9_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::mp9_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::mp9_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::mp9_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::mp9_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("mac10"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::mac10_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::mac10_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::mac10_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::mac10_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::mac10_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::mac10_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::mac10_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::mac10_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::mac10_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::mac10_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::mac10_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::mac10_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::mac10_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("p90"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::p90_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::p90_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::p90_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::p90_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::p90_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::p90_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::p90_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::p90_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::p90_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::p90_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::p90_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::p90_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::p90_wpn_skin_modulation4[1] * 255;
	}
	else if (strstr(buffer.c_str(), ("ump45"))) {
		if (!c::skins::weapon_endable)
			return;

		if (!c::skins::ump45_wpn_skin_custom_clr)
			return;

		m_visuals_data->color1_r = c::skins::ump45_wpn_skin_modulation1[2] * 255;
		m_visuals_data->color1_g = c::skins::ump45_wpn_skin_modulation1[0] * 255;
		m_visuals_data->color1_b = c::skins::ump45_wpn_skin_modulation1[1] * 255;

		m_visuals_data->color2_r = c::skins::ump45_wpn_skin_modulation2[2] * 255;
		m_visuals_data->color2_g = c::skins::ump45_wpn_skin_modulation2[0] * 255;
		m_visuals_data->color2_b = c::skins::ump45_wpn_skin_modulation2[1] * 255;

		m_visuals_data->color3_r = c::skins::ump45_wpn_skin_modulation3[2] * 255;
		m_visuals_data->color3_g = c::skins::ump45_wpn_skin_modulation3[0] * 255;
		m_visuals_data->color3_b = c::skins::ump45_wpn_skin_modulation3[1] * 255;

		m_visuals_data->color4_r = c::skins::ump45_wpn_skin_modulation4[2] * 255;
		m_visuals_data->color4_g = c::skins::ump45_wpn_skin_modulation4[0] * 255;
		m_visuals_data->color4_b = c::skins::ump45_wpn_skin_modulation4[1] * 255;
	}

}
