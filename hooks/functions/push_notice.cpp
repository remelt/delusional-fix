#include "../../features/movement/movement.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

std::string rgb_to_hex(int r, int g, int b, bool with_hash) {
	std::stringstream ss;
	if (with_hash)
		ss << "#";

	ss << std::hex << (r << 16 | g << 8 | b);
	return ss.str();
}

void __fastcall sdk::hooks::push_notice::push_notice(int ecx, int edx, const char* text, int str_len, const char* null) {
	auto print_text = [](int m_ecx, int m_edx, std::string str) {
		auto color1 = rgb_to_hex(menu::menu_accent[0] * 255, menu::menu_accent[1] * 255, menu::menu_accent[2] * 255, true);
		auto color2 = rgb_to_hex(190, 190, 190, true);

		std::string this_str = std::vformat(
			xs("<font color=\"{}\">delusional</font><font color=\"#7d7d7d\"> |</font><font color=\"{}\"> {}</font>"),
			std::make_format_args(color1, color2, str));

		return ofunc(m_ecx, m_edx, this_str.data(), strlen(this_str.data()), this_str.data());
		};

	auto spotify_text = [](int m_ecx, int m_edx, std::string str) {
		auto color1 = rgb_to_hex(menu::menu_accent[0] * 255, menu::menu_accent[1] * 255, menu::menu_accent[2] * 255, true);
		auto color2 = rgb_to_hex(190, 190, 190, true);

		std::string this_str = std::vformat(
			xs("<font color=\"{}\">delusional</font><font color=\"#7d7d7d\"> |</font><font color=\"{}\"> {}</font>"),
			std::make_format_args(color1, color2, str));

		return ofunc(m_ecx, m_edx, this_str.data(), strlen(this_str.data()), this_str.data());
	};

	switch (FNV1A::Hash(text)) {
	case FNV1A::HashConst(("#delusional#_print_created")):
		return print_text(ecx, edx, xs("created config"));
		break;
	case FNV1A::HashConst(("#delusional#_print_saved")):
		return print_text(ecx, edx, xs("saved config"));
		break;
	case FNV1A::HashConst(("#delusional#_print_loaded")):
		return print_text(ecx, edx, xs("loaded config"));
		break;
	case FNV1A::HashConst(("#delusional#_print_refreshed")):
		return print_text(ecx, edx, xs("refreshed config"));
		break;
	case FNV1A::HashConst(("#delusional#_print_reloaded")):
		return print_text(ecx, edx, xs("relaoded fonts"));
		break;
	case FNV1A::HashConst(("#delusional#_print_updated_hud")):
		return print_text(ecx, edx, xs("force updated"));
		break;
	case FNV1A::HashConst(("#delusional#_print_saved_c")):
		return print_text(ecx, edx, xs("saved checkpoint."));
		break;
	case FNV1A::HashConst(("#delusional#_print_save_c")):
		return print_text(ecx, edx, xs("failed to save position."));
		break;
	case FNV1A::HashConst(("#delusional#_print_tp_c")):
		return print_text(ecx, edx, xs("teleported to saved checkpoint."));
		break;
	case FNV1A::HashConst(("#delusional#_print_next_c")):
		return print_text(ecx, edx, xs("next position."));
		break;
	case FNV1A::HashConst(("#delusional#_print_prev_c")):
		return print_text(ecx, edx, xs("previous position."));
		break;
	case FNV1A::HashConst(("#delusional#_print_undid_c")):
		return print_text(ecx, edx, xs("undid position."));
		break;
	case FNV1A::HashConst(("#delusional#_print_jumpbugged")):
		return print_text(ecx, edx, xs("jumpbugged"));
		break;
	case FNV1A::HashConst(("#delusional#_print_edgebugged")):
		return print_text(ecx, edx, xs("edgebugged"));
		break;
	case FNV1A::HashConst(("#delusional#_print_pixelsurfed")):
		return print_text(ecx, edx, xs("pixelsurfed"));
		break;
	case FNV1A::HashConst(("#delusional#_print_hit_1")):
		//return print_text(ecx, edx, std::vformat(xs("hit {} for {} damage ({} remaining)."), std::make_format_args(features::misc::hitinfo.player_name.c_str(), features::misc::hitinfo.damage, features::misc::hitinfo.health)));
		break;
	case FNV1A::HashConst(("#delusional#_print_hit_2")):
	//	return print_text(ecx, edx, std::vformat(xs("hit {} in the {} for {} damage ({} remaining)."), std::make_format_args(features::misc::hitinfo.player_name.c_str(), features::misc::hitinfo.hitgroup, features::misc::hitinfo.damage, features::misc::hitinfo.health)));
		break;
	case FNV1A::HashConst(("#delusional#_spotify_paused")):
		return spotify_text(ecx, edx, xs("now paused"));
		break;
	case FNV1A::HashConst(("#delusional#_spotify_advertisement")):
		return spotify_text(ecx, edx, xs("advertisement is now playing"));
		break;
	case FNV1A::HashConst(("#delusional#_spotify_switch")):
		return spotify_text(ecx, edx, std::vformat(xs("now playing: {}"), std::make_format_args(features::visuals::current_spotify_song)));
		break;
	}

	ofunc(ecx, edx, text, str_len, null);
}