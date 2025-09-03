#include "vec3.hpp"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <numbers>

vec3_t::vec3_t(void) {
	x = y = z = 0.0f;
}

vec3_t::vec3_t(float fx, float fy, float fz) {
	x = fx;
	y = fy;
	z = fz;
}

vec3_t::~vec3_t(void) {
};

void vec3_t::clamp(void) {
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
}

vec3_t vec3_t::clamped() {
	vec3_t clamped = *this;
	clamped.clamp();
	return clamped;
}

float vec3_t::normalize_float() {
	vec3_t res = *this;
	float l = res.length();
	if (l != 0.0f) {
		res /= l;
	}
	else {
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}

float vec3_t::distance_to(const vec3_t& other) {
	vec3_t delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

bool vec3_t::is_zero(void) {
	if (x == 0 && y == 0 && z == 0)
		return true;

	return false;
}

bool  vec3_t::is_valid(void) const {
	return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);

}

void vec3_t::normalize() {
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

float vec3_t::normalize_movement() {
	vec3_t res = *this;
	float l = res.length_r();
	if (l != 0.0f)
	{
		res /= l;
	}
	else
	{
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}

float vec3_t::normalize_in_place() {
	float radius = sqrt(x * x + y * y + z * z);

	float iradius = 1.f / (radius + FLT_EPSILON);

	x *= iradius;
	y *= iradius;
	z *= iradius;

	return radius;
}

vec3_t vec3_t::normalized(void) {
	vec3_t vec(*this);
	vec.normalize();

	return vec;
}

vec3_t vec3_t::normalize2() {
	x = std::isfinite(x) ? std::remainder(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainder(y, 360.0f) : 0.0f;
	z = 0.0f;
	return *this;
}


vec3_t vec3_t::normalize3() {
	this->x = std::clamp(this->x, -89.f, 89.f);

	float rot;
	float& angle = this->y;
	// bad number.
	if (!std::isfinite(angle)) {
		angle = 0.f;
		return *this;
	}

	// no need to normalize this angle.
	if (angle >= -180.f && angle <= 180.f)
		return *this;

	// get amount of rotations needed.
	rot = std::round(std::abs(angle / 360.f));

	// normalize.
	angle = (angle < 0.f) ? angle + (360.f * rot) : angle - (360.f * rot);

	this->z = 0.f;

	return *this;
}

float vec3_t::length(void) {
	float root = 0.0f, sqsr = this->length_sqr();

	__asm        sqrtss xmm0, sqsr
	__asm        movss root, xmm0

	return root;
}

float vec3_t::length_r(void) {
	return sqrtf(lgst());

}

float vec3_t::length_sqr(void) {
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
		};

	return (sqr(x) + sqr(y) + sqr(z));
}

float vec3_t::length_2d_sqr(void) const {
	return (x * x + y * y);
}