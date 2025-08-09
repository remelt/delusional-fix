#pragma once
#include <cstdint>
#include <d3d9.h>
#include <stdexcept>
#include <array>

using ImU32 = unsigned int;
struct ImVec4;

struct color {
    int a, r, g, b;
    color() = default;
    color(int r, int g, int b, int a = 255) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    int& at(size_t idx) {
        switch (idx) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: throw std::out_of_range("color index out of range");
        }
    }

    static color black(int a = 255) { return { 0, 0, 0, a }; }
    static color white(int a = 255) { return { 255, 255, 255, a }; }
    static color red(int   a = 255) { return { 255, 0, 0, a }; }
    static color green(int a = 255) { return { 0, 255, 0, a }; }
    static color blue(int  a = 255) { return { 0, 0, 255, a }; }
    static color yellow(int a = 255) { return  { 247, 202, 24, a }; }
};

class color_t {
public:
	color_t() = default;
	constexpr color_t(float r, float g, float b, float a = 1.0f) {
		setcolor(r, g, b, a);
	}
	constexpr color_t(int r, int g, int b, int a = 255) {
		setcolor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
	constexpr color_t(const std::array<float, 4>& colArr) {
		setcolor(colArr.at(0), colArr.at(1), colArr.at(2), colArr.at(3));
	}
	constexpr color_t(float colArr[4]) {
		setcolor(colArr[0], colArr[1], colArr[2], colArr[3]);
	}
	constexpr color_t(const color_t& col, float alpha) {
		setcolor(col.r(), col.g(), col.b(), alpha);
	}

	constexpr void setcolor(float r, float g, float b, float a = 1.0f) {
		m_color.at(0) = r; m_color.at(1) = g; m_color.at(2) = b; m_color.at(3) = a;
	}

	constexpr void setAlpha(float a) { m_color.at(3) = a; }
	constexpr void setAlphaInt(int a) { m_color.at(3) = a / 255.0f; }

	constexpr auto get() const { return m_color; }
	constexpr auto& getRef() { return m_color; }
	constexpr auto data() { return m_color.data(); }

	constexpr float r() const { return m_color.at(0); }
	constexpr float g() const { return m_color.at(1); }
	constexpr float b() const { return m_color.at(2); }
	constexpr float a() const { return m_color.at(3); }

	static ImVec4 get_imgui_color(const color_t& color);

	// edits alpha
	color_t get_color_edit_alpha(const float amount) const;
	color_t get_color_edit_alpha_int(const int amount) const;

	constexpr uint8_t rMultiplied() const { return static_cast<uint8_t>(m_color.at(0) * 255.0f); }
	constexpr uint8_t gMultiplied() const { return static_cast<uint8_t>(m_color.at(1) * 255.0f); }
	constexpr uint8_t bMultiplied() const { return static_cast<uint8_t>(m_color.at(2) * 255.0f); }
	constexpr uint8_t aMultiplied() const { return static_cast<uint8_t>(m_color.at(3) * 255.0f); }

	constexpr const float operator[](size_t index) const { return m_color.at(index); }
	constexpr float& operator[](size_t index) { return m_color.at(index); }
	constexpr const float at(size_t index) const { if (index >= m_color.size()) throw std::runtime_error("out of range!"); return m_color.at(index); } // as std, at() is safe
	constexpr float& at(size_t index) { if (index >= m_color.size()) throw std::runtime_error("out of range!"); return m_color.at(index); }
	constexpr bool operator==(const color_t& rhs) const { return (*((uintptr_t*)this) == *((uintptr_t*)&rhs)); }
	constexpr bool operator!=(const color_t& rhs) const { return !(operator==(rhs)); }
	static color_t fromHSB(float hue, float saturation, float brightness);
	static color_t hslToRGB(float hue, float saturation, float lightness);
	static color_t rainbow_color(const float gameTime, const float multiply = 0.5f); 
	static color_t health_based(uint8_t health, uint8_t alpha = 255);
	static ImU32 U32(const color_t& color);

private:
	std::array<float, 4> m_color;
};

namespace clrs {
	inline color_t White = color_t(255, 255, 255, 255);
	inline color_t Black = color_t(0, 0, 0, 255);
	inline color_t Red = color_t(255, 0, 0, 255);
	inline color_t Green = color_t(0, 255, 0, 255);
	inline color_t DarkBlue = color_t(0, 0, 255, 255);
	inline color_t LightBlue = color_t(0, 140, 250, 255);
	inline color_t Grey = color_t(128, 128, 128, 128);
	inline color_t Yellow = color_t(255, 255, 0, 255);
	inline color_t Purple = color_t(140, 20, 252, 255);
	inline color_t Turquoise = color_t(60, 210, 200, 255);
	inline color_t Palevioletred = color_t(220, 110, 150, 255);
	inline color_t Pink = color_t(255, 100, 180, 255);
	inline color_t Coral = color_t(255, 127, 80, 255);
	inline color_t Cyan = color_t(0, 255, 255, 255);
	inline color_t Blank = color_t(0, 0, 0, 0);
	inline color_t Orange = color_t(255, 125, 0, 255);
}
