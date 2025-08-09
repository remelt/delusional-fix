#pragma once
#include "../../sdk/sdk.hpp"

enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

namespace surface {
	void initialize();

	void cornered_rect(int x, int y, int w, int h, color colour);
	void line(int x1, int y1, int x2, int y2, color colour);
	void line(vec2_t v0, vec2_t v1, color color);
	void text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour);
	void text(int x, int y, unsigned long font, const wchar_t* text, bool text_centered, color colour);
	void draw_text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour);
	void rect(int x, int y, int w, int h, color color);
	void filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color);
	void filled_rect_fade(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color Top, color Bottom);
	void circle(int x, int y, int r, int s, color col);
	vec2_t get_text_size(unsigned long font, std::string text);
	void add_textured_polygon(int n, vertex_t* vertice, int r, int g, int b, int a);
	void draw_filled_triangle(std::array< vec2_t, 3 >points, color colour);
	void fill_rgba(int x, int y, int w, int h, int r, int g, int b, int a);
	void polygn(int count, std::vector< vertex_t > vertices, int r, int g, int b, int a);

	namespace fonts {
		inline unsigned long watermark;
		inline unsigned long esp;
	}
};
