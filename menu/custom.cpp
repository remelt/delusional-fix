#include "menu.hpp"
#include "../hooks/hooks.hpp"
#include "config/config.hpp"
#include "fonts/menu_font.hpp"
#include "fonts/icon.h"
#include "../features/misc/misc.hpp"
#include "../features/movement/movement.hpp"
#include "../features/skins/skins.hpp"
#include "../features/panorama/scaleform.hpp"
#include "../includes/imgui/imgui_internal.h"
#include "../features/skins/skins.hpp"
#include <string>
#include <map>

void menu::render_tab(const char* szTabBar, const ctab* arrTabs, const std::size_t nTabsCount, int* nCurrentTab, const ImVec4& colActive, ImGuiTabBarFlags flags) {
    // set active tab color
    ImGui::PushStyleColor(ImGuiCol_TabHovered, colActive);
    ImGui::PushStyleColor(ImGuiCol_TabActive, colActive);
    if (ImGui::BeginTabBar(szTabBar, flags)) {
        for (std::size_t i = 0U; i < nTabsCount; i++) {
            // add tab
            if (ImGui::BeginTabItem(arrTabs[i].szName)) {
                // set current tab index
                *nCurrentTab = i;
                ImGui::EndTabItem();
            }
        }

        // render inner tab
        if (arrTabs[*nCurrentTab].render_function != nullptr)
            arrTabs[*nCurrentTab].render_function();

        ImGui::EndTabBar();
    }
    ImGui::PopStyleColor(2);
}

// CHECKING FONTS SAFELY ( s/o flowars )

std::vector<std::string> menu::get_installed_fonts() {
    std::vector<std::string> fonts;
    fonts.push_back("default");

    try {
        for (const auto& entry : std::filesystem::directory_iterator("C:\\Windows\\Fonts")) {
            try {
                std::string fontName = entry.path().filename().string();
                if (fontName.find(".ttf") != std::string::npos) {
                    fonts.push_back(fontName);
                }
            }
            catch (const std::exception& e) {

                continue;
            }
        }
    }
    catch (const std::exception& e) {

    }

    return fonts;
}

//std::vector<std::string> menu::get_installed_fonts() {
//    std::vector<std::string> fonts;
//    fonts.emplace_back("default");
//    for (const auto& entry : std::filesystem::directory_iterator("C:\\Windows\\Fonts\\")) {
//        auto ext = entry.path().extension().string();
//        if (ext == ".ttf" || ext == ".otf")
//        {
//            fonts.emplace_back(entry.path().filename().string());
//        }
//    }
//    return fonts;
//}

void menu::load_font_index() {
    fonts::selected_font_index_main_indi = c::fonts::indi_font;
    if (fonts::selected_font_index_main_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_main_indi] == "default") {
            fonts::font_directory_indicator = "C:/windows/fonts/verdanab.ttf";
        }
        else {
            fonts::font_directory_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_main_indi];
        }
        c::fonts::indi_font = fonts::selected_font_index_main_indi;
    }

    fonts::selected_font_index_sub_indi = c::fonts::sub_indi_font;
    if (fonts::selected_font_index_sub_indi >= 0) {
        if (menu::fonts[fonts::selected_font_index_sub_indi] == "default") {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/verdanab.ttf";
        }
        else {
            fonts::font_directory_sub_indicator = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_sub_indi];
        }
        c::fonts::sub_indi_font = fonts::selected_font_index_sub_indi;
    }

    fonts::selected_font_index_spec = c::fonts::spec_font;
    if (fonts::selected_font_index_spec >= 0) {
        if (menu::fonts[fonts::selected_font_index_spec] == "default") {
            fonts::font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
        }
        else {
            fonts::font_directory_spec = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_spec];
        }
        c::fonts::spec_font = fonts::selected_font_index_spec;
    }

    fonts::selected_font_index_name = c::fonts::esp_name_font;
    if (fonts::selected_font_index_name >= 0) {
        if (menu::fonts[fonts::selected_font_index_name] == "default") {
            fonts::font_directory_name = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_name = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_name];
        }
        c::fonts::esp_name_font = fonts::selected_font_index_name;
    }

    fonts::selected_font_index_wpn = c::fonts::esp_wpn_font;
    if (fonts::selected_font_index_wpn >= 0) {
        if (menu::fonts[fonts::selected_font_index_wpn] == "default") {
            fonts::font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_wpn];
        }
        c::fonts::esp_wpn_font = fonts::selected_font_index_wpn;
    }

    fonts::selected_font_index_dropped_wpn = c::fonts::esp_dropped_wpn_font;
    if (fonts::selected_font_index_dropped_wpn >= 0) {
        if (menu::fonts[fonts::selected_font_index_dropped_wpn] == "default") {
            fonts::font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_dropped_wpn = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_dropped_wpn];
        }
        c::fonts::esp_dropped_wpn_font = fonts::selected_font_index_dropped_wpn;
    }

    fonts::selected_font_index_hp = c::fonts::esp_hp_font;
    if (fonts::selected_font_index_hp >= 0) {
        if (menu::fonts[fonts::selected_font_index_hp] == "default") {
            fonts::font_directory_health = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_health = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_hp];
        }
        c::fonts::esp_hp_font = fonts::selected_font_index_hp;
    }

    fonts::selected_font_index_logs = c::fonts::sc_logs_font;
    if (fonts::selected_font_index_logs >= 0) {
        if (menu::fonts[fonts::selected_font_index_logs] == "default") {
            fonts::font_directory_logs = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_logs = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_logs];
        }
        c::fonts::sc_logs_font = fonts::selected_font_index_logs;
    }

    fonts::selected_font_index_watermark = c::fonts::watermark_font;
    if (fonts::selected_font_index_watermark >= 0) {
        if (menu::fonts[fonts::selected_font_index_watermark] == "default") {
            fonts::font_directory_watermark = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_watermark = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_watermark];
        }
        c::fonts::watermark_font = fonts::selected_font_index_watermark;
    }
    fonts::selected_font_index_assist_font = c::fonts::assist_font;
    if (fonts::selected_font_index_assist_font >= 0) {
        if (menu::fonts[fonts::selected_font_index_assist_font] == "default") {
            fonts::font_directory_assist_font = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_assist_font = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_assist_font];
        }
        c::fonts::assist_font = fonts::selected_font_index_assist_font;
    }
    fonts::selected_font_index_lb_player_font = c::fonts::lb_player_font;
    if (fonts::selected_font_index_lb_player_font >= 0) {
        if (menu::fonts[fonts::selected_font_index_lb_player_font] == "default") {
            fonts::font_directory_lb_player_font = "C:/windows/fonts/tahoma.ttf";
        }
        else {
            fonts::font_directory_lb_player_font = "C:/windows/fonts/" + menu::fonts[fonts::selected_font_index_lb_player_font];
        }
        c::fonts::lb_player_font = fonts::selected_font_index_lb_player_font;
    }
}


bool menu::iskeydown(int key) {
    return HIWORD(GetKeyState(key));
}

bool menu::iskeyup(int key) {
    return !HIWORD(GetKeyState(key));
}

bool menu::checkkey(int key, int keystyle) {
    switch (keystyle) {
    case 0:
        return true;
    case 1:
        return menu::iskeydown(key);
    case 2:
        return LOWORD(GetKeyState(key));
    case 3:
        return menu::iskeyup(key);
    default:
        return true;
    }
}

void  menu::switch_font_cfg(ImFontConfig& f, bool cfg[9]) {
    f.FontBuilderFlags = 0;
    if (cfg[0]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
    if (cfg[1]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoAutoHint;
    if (cfg[2]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LightHinting;
    if (cfg[3]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;
    if (cfg[4]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
    if (cfg[5]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Oblique;
    if (cfg[6]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;
    if (cfg[7]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
    if (cfg[8]) f.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
}
