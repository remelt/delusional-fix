#include "draw.hpp"
#include "../../menu/fonts/icon.h"
#include "../../hooks/hooks.hpp"
#include "../../sdk/sdk.hpp"

void imgui_render::setup(IDirect3DDevice9* device) {
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(sdk::hooks::window);
	ImGui_ImplDX9_Init(device);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 4.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 2.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 0.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
	style.MouseCursorScale = 0.75f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);                 // Black
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 0.85f);         // Dark gray
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.10f, 0.85f);       // Dark blue

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);             // Darker background
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.60f);              // Dark background
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.f);              // Dark background

	style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 0.7f);               // Dark gray border
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);         // Transparent

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);              // Darker background
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);       // Dark blue

	//style.Colors[ImGuiCol_ControlBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);            // Darker background
	//style.Colors[ImGuiCol_ControlBgHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);     // Dark gray
	//style.Colors[ImGuiCol_ControlBgActive] = ImVec4(0.60f, 0.60f, 0.60f, 0.10f);     // Light gray

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);              // Darker background for title
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.f);        // Dark blue background for active title
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.f);     // Dark blue background for collapsed title

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.70f);             // Dark background for menu bar

	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.30f);           // Darker background for scrollbar
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.90f); // Dark gray for scrollbar grab when hovered
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 0.10f); // Light gray for scrollbar grab when active

	style.Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);                // Darker background for buttons
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);         // Dark gray for buttons when hovered

	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);         // Dark gray for header when hovered

	style.Colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);             // Dark gray separator
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);      // Dark gray separator when hovered

	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);      // Dark gray resize grip when hovered

	style.Colors[ImGuiCol_Tab] = ImVec4(0.05f, 0.05f, 0.05f, 0.80f);                   // Darker background for tabs
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.10f, 0.10f, 0.10f, 0.80f);            // Dark gray for tabs when hovered
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.70f);          // Dark gray for unfocused tabs

	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);       // Darker background for modal windows

	style.Colors[ImGuiCol_CheckMark] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);              // checkmark color

	style.Colors[ImGuiCol_SliderGrab] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);             // color for slider grab
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);       // color for slider grab when active

	style.Colors[ImGuiCol_ButtonActive] = ImColor(0.10f, 0.10f, 0.10f, 1.00f);           // color for buttons when active

	style.Colors[ImGuiCol_Header] = ImColor(0.10f, 0.10f, 0.10f, 1.00f);                // color for header
	style.Colors[ImGuiCol_HeaderActive] = ImColor(0.10f, 0.10f, 0.10f, 1.00f);          // color for header when active

	style.Colors[ImGuiCol_SeparatorActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);       // separator when active

	style.Colors[ImGuiCol_ResizeGrip] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);             // resize grip color
	style.Colors[ImGuiCol_ResizeGripActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);       // resize grip color when active

	style.Colors[ImGuiCol_TabActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);            // for active tab
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);   // for unfocused active tab

	style.Colors[ImGuiCol_PlotLines] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);              // for plot lines
	style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);       // for plot lines when hovered
	style.Colors[ImGuiCol_PlotHistogram] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);          // for plot histogram
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);   // for plot histogram when hovered

	style.Colors[ImGuiCol_DragDropTarget] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);         // for drag and drop target

	style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(menu::menu_accent[0], menu::menu_accent[1], menu::menu_accent[2]);         // Accent color for scrollbar grab

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg_menu;
	cfg_menu.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;

	ImFontConfig cfg_screen;
	cfg_screen.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome;

	char windows_directory[MAX_PATH];
	GetWindowsDirectoryA(windows_directory, MAX_PATH);

	std::string menu_font_directory = "C:/windows/fonts/verdana.ttf";
	std::string spec_font_directory = "C:/windows/fonts/tahoma.ttf";

	//menu
	fonts::assist_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 12, &cfg_menu, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//watermark
	ImFontConfig watermark_flag;
	menu::switch_font_cfg(watermark_flag, c::fonts::watermark_font_flag);
	fonts::watermark_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_watermark.c_str(), 12, &watermark_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//lb assist
	ImFontConfig assist_flag;
	menu::switch_font_cfg(assist_flag, c::fonts::assist_font_flag);
	fonts::assist_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_assist_font.c_str(), 12, &assist_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//recorder
	ImFontConfig recorder_flag;
	menu::switch_font_cfg(recorder_flag, c::fonts::recorder_font_flag);
	fonts::recorder_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_recorder.c_str(), 12, &assist_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//lb player
	ImFontConfig lb_player_flag;
	menu::switch_font_cfg(lb_player_flag, c::fonts::lb_player_font_flag);
	fonts::lb_player_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_lb_player_font.c_str(), 12, &lb_player_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//sub spec
	ImFontConfig specfont_flag;
	menu::switch_font_cfg(specfont_flag, c::fonts::spec_font_flag);
	fonts::spec_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_sub_spec.c_str(), 12, &specfont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	
	//main spec    
	fonts::main_spec_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_spec.c_str(), 12, &specfont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//menu main
	fonts::menu_font_bold = io.Fonts->AddFontFromFileTTF(fonts::font_directory_menu_main.c_str(), 12, &cfg_menu, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//main indicators    
	ImFontConfig indifont_flag;
	menu::switch_font_cfg(indifont_flag, c::fonts::indi_font_flag);
	fonts::indicator_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_indicator.c_str(), c::fonts::indi_size, &indifont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//sub indicators    
	ImFontConfig sub_indifont_flag;
	menu::switch_font_cfg(sub_indifont_flag, c::fonts::sub_indi_font_flag);
	fonts::sub_indicator_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_sub_indicator.c_str(), c::fonts::sub_indi_size, &sub_indifont_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//screen logs
	ImFontConfig sc_logs_font_flag;
	menu::switch_font_cfg(sc_logs_font_flag, c::fonts::sc_logs_flag);
	fonts::logs_font_flag = io.Fonts->AddFontFromFileTTF(fonts::font_directory_logs.c_str(), 12, &sc_logs_font_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp name
	ImFontConfig name_flag;
	menu::switch_font_cfg(name_flag, c::fonts::esp_name_font_flag);
	fonts::esp_name_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_name.c_str(), c::fonts::esp_name_size, &name_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp hp
	ImFontConfig hp_flag;
	menu::switch_font_cfg(hp_flag, c::fonts::esp_font_flag);
	fonts::esp_hp_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_health.c_str(), c::fonts::esp_hp_size, &hp_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp wpn
	ImFontConfig wpn_flag;
	menu::switch_font_cfg(wpn_flag, c::fonts::esp_wpn_font_flag);
	fonts::esp_wpn_font = io.Fonts->AddFontFromFileTTF(fonts::font_directory_wpn.c_str(), c::fonts::esp_wpn_size, &wpn_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp dropped wpn
	ImFontConfig dropped_wpn_flag;
	menu::switch_font_cfg(dropped_wpn_flag, c::fonts::esp_dropped_wpn_font_flag);
	fonts::esp_dropped = io.Fonts->AddFontFromFileTTF(fonts::font_directory_dropped_wpn.c_str(), c::fonts::esp_dropped_wpn_size, &dropped_wpn_flag, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp misc
	fonts::esp_misc = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 12, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//keystrokes misc
	fonts::key_strokes_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 12, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//iwebz
	fonts::iwebz_tittle = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 12, &cfg_screen, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	//esp icon
	constexpr ImWchar ranges[] = { 0xE000, 0xF8FF, 0 };

	ImFontConfig cfg_icon;
	cfg_icon.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting;
	fonts::esp_wpn_icon = io.Fonts->AddFontFromMemoryCompressedTTF(icon_font, icon_font_size, 12.f, &cfg_icon, ranges);

	ImGuiFreeType::BuildFontAtlas(io.Fonts);
}

void imgui_render::drawline(const float x, const float y, const float x2, const float y2, const color_t& color, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Line>(ImVec2{ x, y }, ImVec2{ x2, y2 }, color_t::U32(color), thickness));
}

void imgui_render::text(const float x, const float y, const float fontSize, ImFont* font, const std::string& text, const bool centered, const color_t& color, const bool dropShadow, const bool outline) {
	m_draw_data.emplace_back(std::make_unique<drawing::TextSize>(fontSize, font, ImVec2{ x, y }, color_t::U32(color), text, dropShadow, centered, outline));
}

void imgui_render::drawrectfilled(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags) {
	m_draw_data.emplace_back(std::make_unique<drawing::RectangleFilled>(ImVec2{ x, y }, ImVec2{ x + w, y + h }, color_t::U32(color), 0.0f, flags));
}

void imgui_render::drawrectfilledgradient(const float x, const float y, const float w, const float h, const color_t& colUprLeft, const color_t& colUprRight, const color_t& colBotRight, const color_t& colBotLeft) {
	m_draw_data.emplace_back(std::make_unique<drawing::RectangleMultiColor>(ImVec2{ x, y }, ImVec2{ x + w, y + h },
		color_t::U32(colUprLeft), color_t::U32(colUprRight), color_t::U32(colBotRight), color_t::U32(colBotLeft)));
}

void imgui_render::drawrect(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Rectangle>(ImVec2{ x, y }, ImVec2{ x + w, y + h }, color_t::U32(color), 0.0f, flags, thickness));
}

void imgui_render::drawcornerbox(const float x, const float y, const float w, const float h, const color_t& colour) {
	im_render.drawline(x, y, x, y + (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x, y, x + (w / c::visuals::corner_lenght), y, color_t(colour));
	im_render.drawline(x + w, y, x + w - (w / c::visuals::corner_lenght), y, color_t(colour));
	im_render.drawline(x + w, y, x + w, y + (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x, y + h, x + (w / c::visuals::corner_lenght), y + h, color_t(colour));
	im_render.drawline(x, y + h, x, y + h - (h / c::visuals::corner_lenght), color_t(colour));
	im_render.drawline(x + w, y + h, x + w - (w / c::visuals::corner_lenght), y + h, color_t(colour));
	im_render.drawline(x + w, y + h, x + w, y + h - (h / c::visuals::corner_lenght), color_t(colour));
}

void imgui_render::drawcirclefilled(const float x, const float y, const float radius, const int points, const color_t& color) {
	m_draw_data.emplace_back(std::make_unique<drawing::CircleFilled>(ImVec2{ x, y }, radius, points, color_t::U32(color)));
}

void imgui_render::drawcircle(const float x, const float y, const float radius, const int points, const color_t& color, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Circle>(ImVec2{ x, y }, radius, points, color_t::U32(color), thickness));
}

void imgui_render::drawtriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Triangle>(p1, p2, p3, color_t::U32(color), thickness));
}

void imgui_render::drawtrianglefilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color) {
	m_draw_data.emplace_back(std::make_unique<drawing::TriangleFilled>(p1, p2, p3, color_t::U32(color)));
}

void imgui_render::drawpolygon(const std::vector<ImVec2>& verts, const color_t& color) {
	m_draw_data.emplace_back(std::make_unique<drawing::Polygon>(verts, color_t::U32(color)));
}

void imgui_render::drawpolyline(const std::vector<ImVec2>& verts, const color_t& color, const ImDrawFlags flags, const float thickness) {
	m_draw_data.emplace_back(std::make_unique<drawing::Polyline>(verts, color_t::U32(color), flags, thickness));
}

int imgui_render::get_text_size(std::string_view text, ImFont* font, float wrap_width, float font_size) {
	const auto font_ptr = font;

	if (font_size < 0.0f)
		font_size = font_ptr->FontSize;

	const auto text_size = font_ptr->CalcTextSizeA(font_size, 10.0e+10f, wrap_width, text.data());

	return { static_cast<int>(text_size.x) };
}

ImVec2 imgui_render::measure_text(std::string_view text, ImFont* font, float font_size) {
	const auto font_ptr = font;

	if (font_size < 0.0f)
		font_size = font_ptr->FontSize;

	const auto text_size = font_ptr->CalcTextSizeA(font_size, 10.0e+10f, 0.0f, text.data());

	return { (float)text_size.x, (float)text_size.y };
	return { (float)text_size.x, (float)text_size.y };
}

//void imgui_render::circle_3d(const vec3_t& origin, float radius, const color_t& color, const float thickness) {
//	std::vector< vec3_t > Points3D;
//	float step = static_cast<float>(std::numbers::pi) * 2.0f / 256;
//
//	for (float a = 0; a < (std::numbers::pi * 2.0f); a += step) {
//		vec3_t start(radius * cosf(a) + origin.x, radius * sinf(a) + origin.y, origin.z);
//		vec3_t end(radius * cosf(a + step) + origin.x, radius * sinf(a + step) + origin.y, origin.z);
//
//		ImVec2 out, out1, pos3d;
//
//		if (world_to_screen(end, &out1) && world_to_screen(start, &out)) {
//			if (world_to_screen(origin, &pos3d)) {
//				im_render.drawtriangle(out, out1, pos3d, color, thickness);
//			}
//		}
//	}
//}
//
//void imgui_render::circle_filled_3d(const vec3_t& origin, float radius, const color_t& color) {
//	std::vector< vec3_t > Points3D;
//	float step = static_cast<float>(std::numbers::pi) * 2.0f / 256;
//
//	for (float a = 0; a < (std::numbers::pi * 2.0f); a += step) {
//		vec3_t start(radius * cosf(a) + origin.x, radius * sinf(a) + origin.y, origin.z);
//		vec3_t end(radius * cosf(a + step) + origin.x, radius * sinf(a + step) + origin.y, origin.z);
//
//		ImVec2 out, out1, pos3d;
//
//		if (world_to_screen(end, &out1) && world_to_screen(start, &out)) {
//			if (world_to_screen(origin, &pos3d)) {
//				im_render.drawtrianglefilled(out, out1, pos3d, color);
//			}
//		}
//	}
//}

//TODO: UNDERSTAND HOW TO MAKE 3d FILLED CIRCLES CORRECTLY (SHOULD USE POLYLINE AND POLYLINE FILLED IG)
//K IM DUMB ASL

void imgui_render::drawcircle_3d(const vec3_t& location, float radius, const color_t& color, const float thickness) {
	static constexpr float Step = m_pi * 2.0f / 60;
	std::vector<ImVec2> points;
	for (float lat = 0.f; lat <= m_pi * 2.0f; lat += Step) {
		const auto& point3d = vec3_t(sin(lat), cos(lat), 0.f) * radius;
		vec3_t point2d;

		if (world_to_screen_vec3(location + point3d, point2d))
			points.push_back(ImVec2(point2d.x, point2d.y));
	}

	im_render.drawpolyline(points, color, true, thickness);
};

void imgui_render::drawcirclefilled_3d(const vec3_t& location, float radius, const color_t& color) {
	static constexpr float Step = m_pi * 2.0f / 60;
	std::vector<ImVec2> points;
	for (float lat = 0.f; lat <= m_pi * 2.0f; lat += Step) {
		const auto& point3d = vec3_t(sin(lat), cos(lat), 0.f) * radius;
		vec3_t point2d;

		if (world_to_screen_vec3(location + point3d, point2d))
			points.push_back(ImVec2(point2d.x, point2d.y));
	}

	im_render.drawpolygon(points, color);
};

bool imgui_render::world_to_screen(const vec3_t& origin, ImVec2* screen) {
	const view_matrix_t& world_to_screen_matrix = interfaces::engine->world_to_screen_matrix();
	const float flWidth = world_to_screen_matrix[3][0] * origin.x + world_to_screen_matrix[3][1] * origin.y + world_to_screen_matrix[3][2] * origin.z + world_to_screen_matrix[3][3];

	// check is point can't fit on screen, because it's behind us
	if (flWidth < 0.001f)
		return false;

	// compute the scene coordinates of a point in 3D
	const float flInverse = 1.0f / flWidth;
	screen->x = (world_to_screen_matrix[0][0] * origin.x + world_to_screen_matrix[0][1] * origin.y + world_to_screen_matrix[0][2] * origin.z + world_to_screen_matrix[0][3]) * flInverse;
	screen->y = (world_to_screen_matrix[1][0] * origin.x + world_to_screen_matrix[1][1] * origin.y + world_to_screen_matrix[1][2] * origin.z + world_to_screen_matrix[1][3]) * flInverse;

	// screen transform
	// get the screen position in pixels of given point
	const ImVec2 vecDisplaySize = ImGui::GetIO().DisplaySize;
	screen->x = (vecDisplaySize.x * 0.5f) + (screen->x * vecDisplaySize.x) * 0.5f;
	screen->y = (vecDisplaySize.y * 0.5f) - (screen->y * vecDisplaySize.y) * 0.5f;
	return true;
}

bool imgui_render::world_to_screen_vec3(const vec3_t& in, vec3_t& out) {
	const auto result = math::screen_transform(in, out);
	int w, h;
	interfaces::engine->get_screen_size(w, h);

	out.x = (w / 2.0f) + (out.x * w) / 2.0f;
	out.y = (h / 2.0f) - (out.y * h) / 2.0f;

	return result;
}

void imgui_render::render_present(ImDrawList* draw) {
	std::unique_lock<std::shared_mutex> lock{ m_mutex };

	if (m_draw_data_safe.empty())
		return;

	for (const auto& data : m_draw_data_safe)
		data->draw(draw);
}

void imgui_render::clear_data() {
	if (!m_draw_data.empty())
		m_draw_data.clear();
}

void imgui_render::swawp_data() {
	std::unique_lock<std::shared_mutex> lock{ m_mutex };
	m_draw_data.swap(m_draw_data_safe);
}

void imgui_render::add_to_render(const std::function<void()>& fun) {
	clear_data();
	fun();
	swawp_data();
}

void imgui_render::unload() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void fonts::reset_fonts( ) {
	// reset font variables to default values
	font_directory_watermark = "C:/windows/fonts/tahoma.ttf";
	font_directory_lb_player_font = "C:/windows/fonts/tahoma.ttf";
	font_directory_menu_main = "C:/windows/fonts/tahomabd.ttf";
	font_directory_indicator = "C:/windows/fonts/verdanab.ttf";
	font_directory_sub_indicator = "C:/windows/fonts/verdanab.ttf";
	font_directory_misc = "C:/windows/fonts/tahomabd.ttf";
	font_directory_logs = "C:/windows/fonts/tahoma.ttf";
	font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
	font_directory_sub_spec = "C:/windows/fonts/tahoma.ttf";
	font_directory_health = "C:/windows/fonts/tahoma.ttf";
	font_directory_name = "C:/windows/fonts/tahoma.ttf";
    font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
	font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";

	// reset other font-related variables to default values
	c::fonts::indi_font = 0;
	c::fonts::sub_indi_font = 0;
	c::fonts::spec_font = 0;
	c::fonts::sub_spec_font = 0;
	c::fonts::sc_logs_font = 0;
	c::fonts::esp_name_font = 0;
	c::fonts::esp_hp_font = 0;
	c::fonts::esp_wpn_font = 0;
	c::fonts::esp_dropped_wpn_font = 0;
	c::fonts::watermark_font = 0;
	c::fonts::assist_font = 0;
	c::fonts::lb_player_font = 0;
	c::fonts::recorder_font = 0;

	//reset size
	c::fonts::indi_size = 28;
	c::fonts::sub_indi_size = 28;
	c::fonts::spec_size = 12;
	c::fonts::esp_name_size = 12;
	c::fonts::esp_hp_size = 12;
	c::fonts::esp_wpn_size = 12;
	c::fonts::esp_dropped_wpn_size = 12;
	c::fonts::recorder_size = 12;
}

void imgui_render::clear_textures() {

}

void imgui_render::clear_unused_avatars() {

}

//

void ctexture::init(int width, int height, const std::uint8_t* data)  {
	texture = ImGui_CreateTextureRGBA(width, height, data);
}

void ctexture::clear() {
	if (texture)
		ImGui_DestroyTexture(texture);
	texture = nullptr;
}
