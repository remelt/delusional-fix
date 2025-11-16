#pragma once
#include <any>
#include <deque>
#include <mutex>
#include <functional>
#include <shared_mutex>
#include "structures.hpp"
#include "../../menu/menu.hpp"
#include "../includes/imgui/imgui_impl_dx9.h"
#include "../includes/imgui/imgui_impl_win32.h"
#include "../includes/imgui/misc/freetype/imgui_freetype.h"
#include "../../sdk/math/vec3.hpp"

class imgui_render {
public:
	void setup(IDirect3DDevice9* device);
	void unload();

	bool world_to_screen(const vec3_t& origin, ImVec2* screen);
	bool world_to_screen_vec3(const vec3_t& in, vec3_t& out);

	void drawline(const float x, const float y, const float x2, const float y2, const color_t& color, const float thickness = 1.0f);
	void text(const float x, const float y, const float fontsize, ImFont* font, const std::string& text, const bool centered, const color_t& color, const bool dropshadow = true, const bool outline = false);
	void drawrectfilled(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags = 0);
	void drawrectfilledgradient(const float x, const float y, const float w, const float h, const color_t& colUprLeft, const color_t& colUprRight, const color_t& colBotRight, const color_t& colBotLeft);
	void drawrect(const float x, const float y, const float w, const float h, const color_t& color, const ImDrawFlags flags = 0, const float thickness = 1.0f);
	void drawcornerbox(const float x, const float y, const float w, const float h, const color_t& color);
	void drawcircle(const float x, const float y, const float radius, const int points, const color_t& color, const float thickness = 1.0f);
	void drawcirclefilled(const float x, const float y, const float radius, const int points, const color_t& color);
	void drawtriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color, const float thickness = 1.0f);
	void drawtrianglefilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const color_t& color);
	void drawpolygon(const std::vector<ImVec2>& verts, const color_t& color);
	void drawpolyline(const std::vector<ImVec2>& verts, const color_t& color, const ImDrawFlags flags = ImDrawListFlags_AntiAliasedFill, const float thickness = 1.0f);
	int  get_text_size(std::string_view text, ImFont* font, float wrap_width = 0.0f, float font_size = -1.0f);
	/*void circle_3d(const vec3_t& origin, float radius, const color_t& color, const float thickness = 1.0f);
	void circle_filled_3d(const vec3_t& origin, float radius, const color_t& color);*/
	void drawcircle_3d(const vec3_t& location, float radius, const color_t& color, const float thickness = 1.0f);
	void drawcirclefilled_3d(const vec3_t& location, float radius, const color_t& color);
	ImVec2 measure_text(std::string_view text, ImFont* font, float font_size = -1.0f);

	// avatar
	void clear_textures();
	void clear_unused_avatars();
	static void clear_avatar_textures();

	// add to present
	void render_present(ImDrawList* draw);

	// add to surface
	void add_to_render(const std::function<void()>& fun);

	IDirect3DDevice9* m_device = nullptr;
private:
	void clear_data();
	void swawp_data();

	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data;
	std::deque<std::unique_ptr<drawing::Draw>> m_draw_data_safe;
	std::shared_mutex m_mutex;
};

inline imgui_render im_render;

namespace fonts {
	void reset_fonts();

	inline ImFont* menu_font_thin;
	inline ImFont* menu_font_bold;
	inline ImFont* indicator_font;
	inline ImFont* sub_indicator_font;
	inline ImFont* spec_font;
	inline ImFont* main_spec_font;
	inline ImFont* logs_font_flag;
	inline ImFont* watermark_font;
	inline ImFont* assist_font;
	inline ImFont* lb_player_font;
	inline ImFont* icon_font;
	inline ImFont* esp_name_font;
	inline ImFont* esp_hp_font;
	inline ImFont* esp_misc;
	inline ImFont* esp_wpn_font;
	inline ImFont* esp_dropped;
	inline ImFont* esp_wpn_icon;
	inline ImFont* key_strokes_font;
	inline ImFont* iwebz_tittle;
	inline ImFont* iwebz_specs;
	inline ImFont* recorder_font;

	inline std::string font_directory_watermark = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_assist_font = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_lb_player_font = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_menu_main = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_indicator = "C:/windows/fonts/verdanab.ttf";
	inline std::string font_directory_sub_indicator = "C:/windows/fonts/verdanab.ttf";
	inline std::string font_directory_misc = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_logs = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_spec = "C:/windows/fonts/tahomabd.ttf";
	inline std::string font_directory_sub_spec = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_health = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_name = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_wpn = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_dropped_wpn = "C:/windows/fonts/tahoma.ttf";
	inline std::string font_directory_recorder = "C:/windows/fonts/tahoma.ttf";

	// i
	inline int selected_font_index_main_indi = -1;
	inline int selected_font_index_sub_indi = -1;
	inline int selected_font_index_logs = -1;
	inline int selected_font_index_spec = -1;
	inline int selected_font_index_name = -1;
	inline int selected_font_index_hp = -1;
	inline int selected_font_index_wpn = -1;
	inline int selected_font_index_dropped_wpn = -1;
	inline int selected_font_index_watermark = -1;
	inline int selected_font_index_assist_font = -1;
	inline int selected_font_index_lb_player_font = -1;
	inline int selected_font_index_recorder = -1;
}

using ImTextureID = void*;

class ctexture {
	ImTextureID texture = nullptr;
public:
	ctexture() = default;
    ~ctexture() { clear(); }
	ctexture(const ctexture&) = delete;
	ctexture& operator=(const ctexture&) = delete;
	ctexture(ctexture&& other) noexcept : texture{ other.texture } { other.texture = nullptr; }
	ctexture& operator=(ctexture&& other) noexcept { clear(); texture = other.texture; other.texture = nullptr; return *this; }

    void init(int width, int height, const std::uint8_t* data);
    void clear();
    [[nodiscard]] ImTextureID get() const noexcept { return texture; }
};