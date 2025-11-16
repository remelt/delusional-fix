#pragma once
#include "config/config.hpp"
#include "../utils/render/draw.hpp"
#include "../includes/imgui/imgui.h"
#include "../includes/imgui/imgui_impl_dx9.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/misc/freetype/imgui_freetype.h"

#define IMGUI_DEFINE_MATH_OPERATORS

class ctab
{
public:
	const char* szName = nullptr;
	void (*render_function)() = nullptr;
};

namespace menu {
	inline bool open = true;
	inline float delta_time = 0;
	inline int main_tab = 0;
	inline bool initialized = false;
	inline bool unhook = true;
	inline int indicator_tab = 0;
	inline int indicators_tab = 0;
	inline int movement_tab = 0;
	inline int chams_tab = 0;
	inline int esp_tab = 0;
	inline int weapon_selection = 0;
	inline int skin_selection = 0;
	inline bool fonts_initialized;
	inline float menu_accent[3]{ 245 / 255.f, 40 / 255.f, 40 / 255.f };
	inline float menu_accent2[3]{ 255 / 255.f, 255 / 255.f, 255 / 255.f };
	inline float menu_accent3[3]{ 9 / 255.f, 11 / 255.f, 8 / 255.f };
	inline float fade_speed = 1.f / 0.25f;
	inline float alpha = 0.f;
	static const auto red = '\x07';
	inline auto color_p = red;
	inline static int m_tabs = 0;
	inline static int font_tab = 0;
	inline ImVec2 current_size = ImVec2(0, 0);

	inline ImVec2 menu_pos = ImVec2();
	inline ImVec2 menu_size = ImVec2();

	void load_font_index();
	void render();
	void render_tab(const char* szTabBar, const ctab* arrTabs, const std::size_t nTabsCount, int* nCurrentTab, const ImVec4& colActive, ImGuiTabBarFlags flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip);
	bool iskeydown(int key);
	bool iskeyup(int key);
	bool checkkey(int key, int keystyle);
	void switch_font_cfg(ImFontConfig& f, bool cfg[9]);
	inline std::vector<std::string> get_installed_fonts();
	inline static std::vector<std::string> fonts = get_installed_fonts();

	inline static const char* agents[] = {
		"none",
		"Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman",
		"Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman",
		"Lieutenant Rex Krikey | SEAL Frogman",
		"Michael Syfers | FBI Sniper",
		"Operator | FBI SWAT",
		"Special Agent Ava | FBI",
		"Markus Delrow | FBI HRT",
		"Sous-Lieutenant Medic | Gendarmerie Nationale",
		"Chem-Haz Capitaine | Gendarmerie Nationale",
		"Chef d'Escadron Rouchard | Gendarmerie Nationale",
		"Aspirant | Gendarmerie Nationale",
		"Officer Jacques Beltram | Gendarmerie Nationale",
		"D Squadron Officer | NZSAS",
		"B Squadron Officer | SAS",
		"Seal Team 6 Soldier | NSWC SEAL",
		"Buckshot | NSWC SEAL",
		"Lt. Commander Ricksaw | NSWC SEAL",
		"'Blueberries' Buckshot | NSWC SEAL",
		"3rd Commando Company | KSK",
		"'Two Times' McCoy | TACP Cavalry",
		"''Two Times' McCoy | USAF TACP",
		"'Primeiro Tenente | Brazilian 1st Battalion",
		"'Cmdr. Mae 'Dead Cold' Jamison | SWAT",
		"'1st Lieutenant Farlow | SWAT",
		"'John 'Van Healen' Kask | SWAT",
		"'Bio-Haz Specialist | SWAT",
		"'Sergeant Bombson | SWAT",
		"'Chem-Haz Specialist | SWAT", //
		"'Lieutenant 'Tree Hugger' Farlow | SWAT",
		"Getaway Sally | The Professionals",
		"Number K | The Professionals",
		"Little Kev | The Professionals",
		"Safecracker Voltzmann | The Professionals",
		"Bloody Darryl The Strapped | The Professionals",
		"Sir Bloody Loudmouth Darryl | The Professionals",
		"Sir Bloody Darryl Royale | The Professionals",
		"Sir Bloody Skullhead Darryl | The Professionals",
		"Sir Bloody Silent Darryl | The Professionals",
		"Sir Bloody Miami Darryl | The Professionals",
		"Street Soldier | Phoenix",
		"Soldier | Phoenix",
		"Slingshot | Phoenix",
		"Enforcer | Phoenix",
		"Mr. Muhlik | Elite Crew",
		"Prof. Shahmat | Elite Crew",
		"Osiris | Elite Crew",
		"Ground Rebel | Elite Crew",
		"The Elite Mr. Muhlik | Elite Crew",
		"Trapper | Guerrilla Warfare",
		"Trapper Aggressor | Guerrilla Warfare",
		"Vypa Sista of the Revolution | Guerrilla Warfare",
		"Col. Mangos Dabisi | Guerrilla Warfare",
		"Arno The Overgrown | Guerrilla Warfare",
		"'Medium Rare' Crasswater | Guerrilla Warfare",
		"Crasswater The Forgotten | Guerrilla Warfare",
		"Elite Trapper Solman | Guerrilla Warfare",
		"'The Doctor' Romanov | Sabre",
		"Blackwolf | Sabre",
		"Maximus | Sabre",
		"Dragomir | Sabre",
		"Rezan The Ready | Sabre",
		"Rezan the Redshirt | Sabre",
		"Dragomir | Sabre Footsoldier",
	};
}