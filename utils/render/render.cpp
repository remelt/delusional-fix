#include "../render/render.hpp"
#include "../../menu/config/config.hpp"

void surface::initialize() {
	surface::fonts::watermark = interfaces::surface->font_create();
	surface::fonts::esp = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(surface::fonts::watermark, ("verdana"), 12, 800, 0, 0, font_flags::fontflag_outline | font_flags::fontflag_custom);
	interfaces::surface->set_font_glyph(surface::fonts::esp, ("tahoma"), 13, 0, 0, 0, font_flags::fontflag_outline | font_flags::fontflag_custom);
}

void surface::text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour) {
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, temp, 128) > 0) {
		interfaces::surface->draw_text_font(font);
		if (text_centered) {
			interfaces::surface->get_text_size(font, temp, text_width, text_height);
			interfaces::surface->draw_text_pos(x - text_width / 2, y);
		}
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
		interfaces::surface->draw_render_text(temp, wcslen(temp));
	}
}

void surface::text(int x, int y, unsigned long font, const wchar_t* text, bool centered, color color) {
	interfaces::surface->draw_text_font(font);
	int text_width, text_height;

	if (centered) {
		interfaces::surface->get_text_size(font, text, text_width, text_height);
		interfaces::surface->draw_text_pos(x - text_width / 2, y);
	}
	else
		interfaces::surface->draw_text_pos(x, y);

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_render_text(text, wcslen(text));
}

void surface::draw_text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(text_centered ? (x - (width / 2)) : x, y);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}

void surface::fill_rgba(int x, int y, int w, int h, int r, int g, int b, int a) {
	interfaces::surface->set_drawing_color(r, g, b, a);
	interfaces::surface->draw_filled_rectangle(x, y, x + w, y + h);
}

vec2_t surface::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

void surface::line(int x1, int y1, int x2, int y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void surface::line(vec2_t v0, vec2_t v1, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_line(v0.x, v0.y, v1.x, v1.y);
}

void surface::rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void surface::filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void surface::filled_rect_fade(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color Top, color Bottom) {
	interfaces::surface->set_drawing_color(Top.r, Top.g, Top.b, Top.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 255, 255, false);

	interfaces::surface->set_drawing_color(Bottom.r, Bottom.g, Bottom.b, Bottom.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 0, 255, false);
}

void surface::circle(int x, int y, int r, int s, color col) {
	float step = m_pi * 2.0 / s;
	for (float a = 0; a < (m_pi * 2.0); a += step) {
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + step) + x;
		float y2 = r * sin(a + step) + y;
		interfaces::surface->set_drawing_color(col.r, col.g, col.b, col.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

void surface::add_textured_polygon(int n, vertex_t* vertice, int r, int g, int b, int a) {
	static int texture_id = interfaces::surface->create_new_texture_id(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	interfaces::surface->set_drawing_color(r, g, b, a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_textured_polygon(n, vertice);
}

void surface::draw_filled_triangle(std::array< vec2_t, 3 >points, color colour) {
	std::array< vertex_t, 3 >vertices{ vertex_t(points.at(0)), vertex_t(points.at(1)), vertex_t(points.at(2)) };
	add_textured_polygon(3, vertices.data(), colour.r, colour.g, colour.b, colour.a);
}

void surface::polygn(int count, std::vector< vertex_t > vertices, int r, int g, int b, int a) {
	static int texture = interfaces::surface->create_new_texture_id(true);

	interfaces::surface->set_drawing_color(r, g, b, a);
	interfaces::surface->set_texture(texture);

	interfaces::surface->draw_textured_polygon(vertices.size(), vertices.data());
}