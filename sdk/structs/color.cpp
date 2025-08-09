#include "color.hpp"
#include "../../includes/imgui/imgui_impl_dx9.h"

ImVec4 color_t::get_imgui_color(const color_t& color) {
	return ImVec4{ color.r(), color.g(), color.b(), color.a() };
}

color_t color_t::get_color_edit_alpha(const float amount) const {
	color_t clr = *this;
	clr.at(3) = amount;
	return clr;
}

color_t color_t::get_color_edit_alpha_int(const int amount) const {
	color_t clr = *this;
	clr.at(3) = amount / 255.0f;
	return clr;
}

color_t color_t::fromHSB(float hue, float saturation, float brightness) {
	float h = hue == 1.0f ? 0.0f : hue * 6.0f;
	float f = h - h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1.0f) {
		return color_t{ brightness, t, p };
	}
	else if (h < 2.0f) {
		return color_t{ q, brightness, p };
	}
	else if (h < 3.0f) {
		return color_t{ p, brightness, t };
	}
	else if (h < 4.0f) {
		return color_t{ p, q, brightness };
	}
	else if (h < 5.0f) {
		return color_t{ t, p, brightness };
	}
	else {
		return color_t{ brightness, p, q };
	}
}

color_t color_t::hslToRGB(float hue, float saturation, float lightness) {
	auto huetoRGB = [](float p, float q, float t) {
		if (t < 0.0f)
			t += 1.0f;
		if (t > 1.0f)
			t -= 1.0f;
		if (t < 1.0f / 6.0f)
			return p + (q - p) * 6.0f * t;
		if (t < 1.0f / 2.0f)
			return q;
		if (t < 2.0f / 3.0f)
			return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
		return p;
	};

	float r, g, b;

	if (saturation == 0.0f)
		r = g = b = lightness;
	else {
		float q = lightness < 0.5f ? lightness * (1.0f + saturation) : lightness + saturation - lightness * saturation;
		float p = 2.0f * lightness - q;
		r = huetoRGB(p, q, hue + 1.0f / 3.0f);
		g = huetoRGB(p, q, hue);
		b = huetoRGB(p, q, hue - 1.0f / 3.0f);
	}

	return color_t{ r, g, b };
}

ImU32 color_t::U32(const color_t& color) {
	return ImGui::GetColorU32(color_t::get_imgui_color(color));
}

#include <cmath>
#include <numbers>

color_t color_t::rainbow_color(const float gameTime, const float multiply) {
	return color_t(
		std::cos(gameTime * multiply) * 0.5f + 0.5f,
		std::cos(gameTime * multiply - 2.0f * std::numbers::pi_v<float> / 3.0f) * 0.5f + 0.5f,
		std::cos(gameTime * multiply - 4.0f * std::numbers::pi_v<float> / 3.0f) * 0.5f + 0.5f);
}

color_t color_t::health_based(uint8_t health, uint8_t alpha) {
	int g = static_cast<int>(health * 2.55f);
	int r = 255 - g;
	return color_t(r, g, 0, alpha);
}
