#pragma once
#pragma warning( disable : 26451 )
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <numbers>

constexpr float deg2rad(float degrees) {
	return degrees * (3.14159265358979323846f / 180.0f);
}

class vec3_t {
public:
	vec3_t();
	vec3_t(float, float, float);
	~vec3_t();

	float x, y, z;

	void init(float ix, float iy, float iz) {
		x = ix; y = iy; z = iz;
	}

	void vec(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	constexpr auto not_null() const noexcept {
		return x || y || z;
	}

	constexpr auto null() const noexcept {
		return !not_null();
	}

	static void vector_cross_product(const vec3_t& a, const vec3_t& b, vec3_t& result) {
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	[[nodiscard]] vec3_t cross(const vec3_t& v_other) const {
		vec3_t res;
		vector_cross_product(*this, v_other, res);
		return res;
	}

	auto length2() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	inline float length_2d(void) const {
		float root = 0.0f;
		float sqst = x * x + y * y;
		__asm {
			sqrtss xmm0, sqst
			movss root, xmm0
		}
		return root;
	}

	[[nodiscard]] float dist_to_2d(const vec3_t& vector) const
	{
		return (*this - vector).length_2d();
	}

	[[nodiscard]] vec3_t to_angle() const {
		return {
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f
		};
	}
	[[nodiscard]] vec3_t to_angle2() const
	{
		double M_PI2 = 3.14159265358979323846;

		float M_PI_F2 = static_cast<float>(M_PI2);
		return vec3_t(std::atan2(-z, std::hypot(x, y)) * (180.0f / M_PI_F2), std::atan2(y, x) * (180.0f / M_PI_F2), 0.0f);
	}

	constexpr auto dot_product(const vec3_t& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline float lgst() {
		return (x * x + y * y + z * z);
	}

	vec3_t forward() const {
		vec3_t result;
		result.x = x;
		result.y = y;
		result.z = z;
		return result;
	}

	vec3_t normalize_const() const {
		vec3_t tmp = *this;
		tmp.x = std::isfinite(tmp.x) ? std::remainderf(tmp.x, 360.0f) : 0.0f;
		tmp.y = std::isfinite(tmp.y) ? std::remainderf(tmp.y, 360.0f) : 0.0f;
		tmp.z = 0.0f;
		return tmp;
	}

	float distance(const vec3_t& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		float dz = z - other.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	[[nodiscard]] float dist_to(const vec3_t& vector) const
	{
		return (*this - vector).length();
	}

	static vec3_t fromAngle(const vec3_t& angle) noexcept {
		float radX = deg2rad(angle.x);
		float radY = deg2rad(angle.y);
		return vec3_t(std::cos(radX) * std::cos(radY), std::cos(radX) * std::sin(radY), -std::sin(radX));
	}

	float dot(const vec3_t other) {
		return (x * other.x + y * other.y + z * other.z);
	}

	void NormalizeInPlace()
	{
		*this = Normalized();
	}
	vec3_t Normalized() const
	{
		vec3_t res = *this;
		float l = res.length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	bool wtf(void)
	{
		if (x == 0 && y == 0 && z == 0)
			return true;
		return false;
	}

	float dot(const float* other) {
		return(x * other[0] + y * other[1] + z * other[2]);
	}

	[[nodiscard]] float* data()
	{
		return reinterpret_cast<float*>(this);
	}

	[[nodiscard]] const float* data() const
	{
		return reinterpret_cast<float*>(const_cast<vec3_t*>(this));
	}

	float& operator[](const std::size_t i)
	{
		return this->data()[i];
	}

	const float& operator[](const std::size_t i) const
	{
		return this->data()[i];
	}

	void clamp();
	vec3_t clamped();
	vec3_t normalized();
	vec3_t normalize2();
	vec3_t normalize3();
	void normalize();
	float normalize_movement();
	float normalize_in_place();
	float normalize_float();
	float distance_to(const vec3_t& other);
	bool is_zero(void);
	bool is_valid(void) const;
	float length();
	float length_sqr();
	float length_2d_sqr(void) const;
	float length_r();

	vec3_t operator-() const noexcept {
		return vec3_t{ -x, -y, -z };
	}

	vec3_t& operator+=(const vec3_t& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	vec3_t& operator-=(const vec3_t& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	vec3_t& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}

	vec3_t operator+(const vec3_t& v) {
		return vec3_t{ x + v.x, y + v.y, z + v.z };
	}

	vec3_t operator-(const vec3_t& v) {
		return vec3_t{ x - v.x, y - v.y, z - v.z };
	}

	vec3_t operator*(float fl) const {
		return vec3_t(x * fl, y * fl, z * fl);
	}

	vec3_t operator*(const vec3_t& v) const {
		return vec3_t(x * v.x, y * v.y, z * v.z);
	}

	vec3_t& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}

	vec3_t& operator=(const vec3_t& vOther) {
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	auto operator-(const vec3_t& other) const -> vec3_t {
		auto buf = *this;

		buf.x -= other.x;
		buf.y -= other.y;
		buf.z -= other.z;

		return buf;
	}

	auto operator/(float other) const {
		vec3_t vec;
		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;
		return vec;
	}

	auto operator+ (const vec3_t& v) const {
		return vec3_t(x + v.x, y + v.y, z + v.z);
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	bool operator==(const vec3_t& other) const noexcept {
		return x == other.x && y == other.y && z == other.z;
	}
};

inline vec3_t operator*(float lhs, const vec3_t& rhs) {
	return vec3_t(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}

class __declspec(align(16)) vector_aligned : public vec3_t {
public:
	vector_aligned() = default;

	explicit vector_aligned(const vec3_t& base) {
		this->x = base.x; this->y = base.y; this->z = base.z; this->w = 0.f;
	}

	constexpr vector_aligned& operator=(const vec3_t& base) {
		this->x = base.x; this->y = base.y; this->z = base.z; this->w = 0.f;
		return *this;
	}

public:
	float w;
};

//

struct matrix_t {
	matrix_t() {}
	matrix_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23) {
		mat_val[0][0] = m00;
		mat_val[0][1] = m01;
		mat_val[0][2] = m02;
		mat_val[0][3] = m03;
		mat_val[1][0] = m10;
		mat_val[1][1] = m11;
		mat_val[1][2] = m12;
		mat_val[1][3] = m13;
		mat_val[2][0] = m20;
		mat_val[2][1] = m21;
		mat_val[2][2] = m22;
		mat_val[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void init(const vec3_t& x_axis, const vec3_t& y_axis, const vec3_t& z_axis, const vec3_t& origin) {
		mat_val[0][0] = x_axis.x;
		mat_val[0][1] = y_axis.x;
		mat_val[0][2] = z_axis.x;
		mat_val[0][3] = origin.x;
		mat_val[1][0] = x_axis.y;
		mat_val[1][1] = y_axis.y;
		mat_val[1][2] = z_axis.y;
		mat_val[1][3] = origin.y;
		mat_val[2][0] = x_axis.z;
		mat_val[2][1] = y_axis.z;
		mat_val[2][2] = z_axis.z;
		mat_val[2][3] = origin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix_t(const vec3_t& x, const vec3_t& y, const vec3_t& z, const vec3_t& origin) {
		init(x, y, z, origin);
	}

	inline void set_origin(vec3_t const& p) {
		mat_val[0][3] = p.x;
		mat_val[1][3] = p.y;
		mat_val[2][3] = p.z;
	}

	vec3_t get_x_axis() const {
		return at(0);
	}

	vec3_t get_y_axis() const {
		return at(1);
	}

	vec3_t get_z_axis() const {
		return at(2);
	}

	vec3_t get_origin() const {
		return at(3);
	}

	vec3_t at(const int i) const {
		return vec3_t{ mat_val[0][i], mat_val[1][i], mat_val[2][i] };
	}

	void invalidate(void) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				mat_val[i][j] = std::numeric_limits<float>::infinity();
			}
		}
	}

	float* operator[](const int i) {
		return mat_val[i];
	}
	const float* operator[](const int i) const {
		return mat_val[i];
	}
	float* base() {
		return &mat_val[0][0];
	}
	[[nodiscard]] const float* base() const {
		return &mat_val[0][0];
	}

	float mat_val[3][4];
};