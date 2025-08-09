#pragma once
#include "vec2.hpp"
#include "view_matrix.hpp"
class vec2_t {
public:
	float x, y;

	vec2_t() {
		x = 0; y = 0;
	};
	vec2_t(float _x, float _y) {
		x = _x; y = _y;
	};
	vec2_t(vec3_t vec) {
		x = vec.x; y = vec.y;
	}

	inline vec2_t operator*(const float n) const {
		return vec2_t(x * n, y * n);
	}
	inline vec2_t operator+(const vec2_t& v) const {
		return vec2_t(x + v.x, y + v.y);
	}
	inline vec2_t operator-(const vec2_t& v) const {
		return vec2_t(x - v.x, y - v.y);
	}
	inline void operator+=(const vec2_t& v) {
		x += v.x;
		y += v.y;
	}
	inline vec2_t& operator/=(const vec2_t& v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}
	inline vec2_t& operator/=(float f) {
		x /= f;
		y /= f;
		return *this;
	}
	inline void operator-=(const vec2_t& v) {
		x -= v.x;
		y -= v.y;
	}


	inline void Vector2DDivide(const vec2_t& a, vec_t b, vec2_t& c)const
	{
		vec_t oob = 1.0f / b;
		c.x = a.x * oob;
		c.y = a.y * oob;
	}

	inline vec2_t operator/(float fl) const
	{
		vec2_t res;
		Vector2DDivide(*this, fl, res);
		return res;
	}

	bool operator==(const vec2_t& v) const {
		return (v.x == x && v.y == y);
	}
	bool operator!=(const vec2_t& v) const {
		return (v.x != x || v.y != y);
	}

	inline float length() {
		return sqrt((x * x) + (y * y));
	}

};
