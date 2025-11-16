#pragma once
#include "../sdk.hpp"
#include "../math/view_matrix.hpp"
#include "directxmath.h"

#define tick_interval			( interfaces::globals->interval_per_tick )
#define time_to_ticks( t )		( static_cast<int>( 0.5f + static_cast<float>( t ) / tick_interval ) )
#define ticks_to_time( t )		( tick_interval * static_cast<float>( t ) )
#define round_to_ticks( t )		( tick_interval * time_to_ticks( t ) )
#define tick_never_think		( -1 )

namespace math {
    #define m_pi		3.14159265358979323846
    #define M_PI_R		0.01745329251
    #define M_PI_F		((float)(m_pi))
    #define M_RADPI     57.295779513082f

	inline auto deg2rad(float degrees) { return degrees * (M_PI_F / 180.0f); }
	inline auto rad2deg(float radians) { return radians * (180.0f / M_PI_F); }

	inline vec3_t calc_angle( const vec3_t& src, const vec3_t& dst ) {
		vec3_t angle;
		vec3_t delta( ( src.x - dst.x ), ( src.y - dst.y ), ( src.z - dst.z ) );
		double hyp = sqrt( delta.x * delta.x + delta.y * delta.y );

		angle.x = float( atanf( float( delta.z / hyp ) ) * 57.295779513082f );
		angle.y = float( atanf( float( delta.y / delta.x ) ) * 57.295779513082f );
		angle.z = 0.0f;

		if (delta.x >= 0.0)
			angle.y += 180.0f;

		return angle;
	}

	inline vec3_t Vpenivatel(const vec3_t& source, const vec3_t& destination, const vec3_t& vievAngles)
	{
		vec3_t delta = source - destination;
		vec3_t angles;

		angles.x = rad2deg(atanf(delta.z / std::hypotf(delta.x, delta.y))) - vievAngles.x;
		angles.y = rad2deg(atanf(delta.y / delta.x)) - vievAngles.y;
		angles.z = 0.0f;

		if (delta.x >= 0.0)
			angles.y += 180.0f;

		return angles;
	}

	inline void angle_vectors_alternative(const vec3_t& angles, vec3_t* forward) {
		float	sp, sy, cp, cy;

		sy = sin(deg2rad(angles[1]));
		cy = cos(deg2rad(angles[1]));

		sp = sin(deg2rad(angles[0]));
		cp = cos(deg2rad(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	inline void sin_cos(float r, float* s, float* c) {
		*s = sin(r);
		*c = cos(r);
	}

	inline void angle_vectors_alternative(vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		float sp, sy, sr, cp, cy, cr;

		sin_cos(deg2rad(angles.x), &sp, &cp);
		sin_cos(deg2rad(angles.y), &sy, &cy);
		sin_cos(deg2rad(angles.z), &sr, &cr);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = -1 * sr * sp * cy + -1 * cr * -sy;
			right->y = -1 * sr * sp * sy + -1 * cr * cy;
			right->z = -1 * sr * cp;
		}

		if (up) {
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}

	inline float view_matrix[4][4] = { 0 };
	inline bool screen_transform(const vec3_t& in, vec3_t& out) {
		auto w2_s_matrix = view_matrix;
		out.x = w2_s_matrix[0][0] * in[0] + w2_s_matrix[0][1] * in[1] + w2_s_matrix[0][2] * in[2] + w2_s_matrix[0][3];
		out.y = w2_s_matrix[1][0] * in[0] + w2_s_matrix[1][1] * in[1] + w2_s_matrix[1][2] * in[2] + w2_s_matrix[1][3];
		out.z = 0.0f;

		const auto w = w2_s_matrix[3][0] * in.x + w2_s_matrix[3][1] * in.y + w2_s_matrix[3][2] * in.z + w2_s_matrix[3][3];

		if (w < 0.001f) {
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		const auto invw = 1.0f / w;
		out.x *= invw;
		out.y *= invw;

		return true;
	}

	inline void vectorangles(const vec3_t& forward, vec3_t& angles) {
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / m_pi);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / m_pi);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline vec3_t calculate_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) {
		vec3_t delta = source - destination;
		vec3_t angles;

		angles.x = rad2deg(atanf(delta.z / std::hypotf(delta.x, delta.y))) - viewAngles.x;
		angles.y = rad2deg(atanf(delta.y / delta.x)) - viewAngles.y;
		angles.z = 0.0f;

		if (delta.x >= 0.0)
			angles.y += 180.0f;

		return angles;
	}

	inline void angle_vectors(const vec3_t& angles, vec3_t& forward) {
		float sp, sy, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, deg2rad(angles.x));
		DirectX::XMScalarSinCos(&sy, &cy, deg2rad(angles.y));

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;
	}

	inline void vector_angles(const vec3_t& forward, vec3_t& angles) {
		float tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0) {
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else {
			yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline float angle_delta_rad(float a, float b) {
		float delta;
		delta = isfinite(a - b) ? remainder(a - b, 360) : 0;

		if (a > b) {
			if (delta >= m_pi)  delta -= m_pi * 2;
		}
		else {
			if (delta <= -m_pi) delta += m_pi * 2;
		}

		return delta;
	}

	inline float normalize_yaw(float yaw) {
		if (!std::isfinite(yaw))
			return 0.0f;

		if (yaw >= -180.f && yaw <= 180.f)
			return yaw;

		const float rot = std::round(std::abs(yaw / 360.f));

		yaw = (yaw < 0.f) ? yaw + (360.f * rot) : yaw - (360.f * rot);
		return yaw;
	}

	inline void transform_vector(vec3_t& a, matrix_t& b, vec3_t& out) {
		out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
		out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
		out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
	}

	inline void vector_transform(vec3_t& in1, matrix_t& in2, vec3_t& out) {
		out[0] = in1.dot(in2[0]) + in2[0][3];
		out[1] = in1.dot(in2[1]) + in2[1][3];
		out[2] = in1.dot(in2[2]) + in2[2][3];
	}

	inline void clampangles(vec3_t& angles) {
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}

	template<class T>
	inline void normalize(T& vec) {
		for (auto i = 0; i < 2; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] > 180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}

	inline void fix_angles(vec3_t& angles) {
		normalize(angles);
		clampangles(angles);
	}

	inline vec3_t normalize_angle(vec3_t angle) {
		while (angle.x > 89.f) {
			angle.x -= 180.f;
		}
		while (angle.x < -89.f) {
			angle.x += 180.f;
		}
		if (angle.y > 180) {
			angle.y -= (round(angle.y / 360) * 360.f);
		}
		else if (angle.y < -180) {
			angle.y += (round(angle.y / 360) * -360.f);
		}
		if ((angle.z > 50) || (angle.z < 50)) {
			angle.z = 0;
		}
		return angle;
	}

	inline bool normalize_angles(vec3_t& angles) {
		if (std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)) {
			angles.x = std::remainder(angles.x, 360.0f);
			angles.y = std::remainder(angles.y, 360.0f);

			return true;
		}

		return false;
	}

	inline bool clamp_angles(vec3_t& angles) {
		if (std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)) {
			angles.x = std::clamp(angles.x, -89.0f, 89.0f);
			angles.y = std::clamp(angles.y, -180.0f, 180.0f);
			angles.z = 0.0f;

			return true;
		}

		return false;
	}

	inline float strafe_opti_normalize_angle(float angle, float max) {
		while (angle > max)
			angle -= max * 2;
		while (angle < -max)
			angle += max * 2;
		return angle;
	}

	inline void angle_vector(const vec3_t& angles, vec3_t* forward)
	{
		float	sp, sy, cp, cy;

		sy = sin(deg2rad(angles[1]));
		cy = cos(deg2rad(angles[1]));

		sp = sin(deg2rad(angles[0]));
		cp = cos(deg2rad(angles[0]));

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	inline vec3_t angle_vector(vec3_t angle) {
		auto sy = sin(angle.y / 180.f * static_cast<float>(m_pi));
		auto cy = cos(angle.y / 180.f * static_cast<float>(m_pi));

		auto sp = sin(angle.x / 180.f * static_cast<float>(m_pi));
		auto cp = cos(angle.x / 180.f * static_cast<float>(m_pi));

		return vec3_t(cp * cy, cp * sy, -sp);
	}

	inline void angle_vectors(const vec3_t& angles, vec3_t* forward) {
		float sp, sy, cp, cy;
		sin_cos(deg2rad(angles[0]), &sp, &cp);
		sin_cos(deg2rad(angles[1]), &sy, &cy);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	inline void angle_to_vectors(const vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up) {
		float sp, sy, sr, cp, cy, cr;

		sin_cos(deg2rad(angles.x), &sp, &cp);
		sin_cos(deg2rad(angles.y), &sy, &cy);
		sin_cos(deg2rad(angles.z), &sr, &cr);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;

		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;

		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
	
	inline void angle_vectors(const vec3_t& angle, vec3_t* forward, vec3_t* right, vec3_t* up) {
		float sr = std::sin(deg2rad(angle.z))
			, sp = std::sin(deg2rad(angle.x))
			, sy = std::sin(deg2rad(angle.y))
			, cr = std::cos(deg2rad(angle.z))
			, cp = std::cos(deg2rad(angle.x))
			, cy = std::cos(deg2rad(angle.y));

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up) {
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	inline float fromhsvtor(float h, float s, float v) {
		float out_r; float out_g; float out_b;
		if (s == 0.0f)
		{
			out_r = out_g = out_b = v;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0: out_r = v; out_g = t; out_b = p; break;
		case 1: out_r = q; out_g = v; out_b = p; break;
		case 2: out_r = p; out_g = v; out_b = t; break;
		case 3: out_r = p; out_g = q; out_b = v; break;
		case 4: out_r = t; out_g = p; out_b = v; break;
		case 5: default: out_r = v; out_g = p; out_b = q; break;
		}
		return out_r;
	}

	inline float fromhsvtog(float h, float s, float v) {
		float out_r; float out_g; float out_b;
		if (s == 0.0f)
		{
			out_r = out_g = out_b = v;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0: out_r = v; out_g = t; out_b = p; break;
		case 1: out_r = q; out_g = v; out_b = p; break;
		case 2: out_r = p; out_g = v; out_b = t; break;
		case 3: out_r = p; out_g = q; out_b = v; break;
		case 4: out_r = t; out_g = p; out_b = v; break;
		case 5: default: out_r = v; out_g = p; out_b = q; break;
		}
		return out_g;
	}

	inline float fromhsvtob(float h, float s, float v) {
		float out_r; float out_g; float out_b;
		if (s == 0.0f)
		{
			out_r = out_g = out_b = v;
		}

		h = fmodf(h, 1.0f) / (60.0f / 360.0f);
		int   i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i)
		{
		case 0: out_r = v; out_g = t; out_b = p; break;
		case 1: out_r = q; out_g = v; out_b = p; break;
		case 2: out_r = p; out_g = v; out_b = t; break;
		case 3: out_r = p; out_g = q; out_b = v; break;
		case 4: out_r = t; out_g = p; out_b = v; break;
		case 5: default: out_r = v; out_g = p; out_b = q; break;
		}
		return out_b;
	}

	inline float rgbtoh(float rr, float gg, float bb) {
		const auto r = rr;
		const auto g = gg;
		const auto b = bb;

		auto h = 0.0f, s = 0.0f, v = 0.0f;

		const auto max = std::max < float >(r, std::max < float >(g, b));
		const auto min = std::min < float >(r, std::min < float >(g, b));

		v = max;

		if (max == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else if (max - min == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else {
			s = (max - min) / max;

			if (max == r) {
				h = 60.0f * ((g - b) / (max - min)) + 0.0f;
			}
			else if (max == g) {
				h = 60.0f * ((b - r) / (max - min)) + 120.0f;
			}
			else {
				h = 60.0f * ((r - g) / (max - min)) + 240.0f;
			}
		}

		if (h < 0.0f)
			h += 360.0f;

		return h / 2.0f / 180.f;
	}

	inline float rgbtos(float rr, float gg, float bb) {
		const auto r = rr;
		const auto g = gg;
		const auto b = bb;

		auto h = 0.0f, s = 0.0f, v = 0.0f;

		const auto max = std::max < float >(r, std::max < float >(g, b));
		const auto min = std::min < float >(r, std::min < float >(g, b));

		v = max;

		if (max == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else if (max - min == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else {
			s = (max - min) / max;

			if (max == r) {
				h = 60.0f * ((g - b) / (max - min)) + 0.0f;
			}
			else if (max == g) {
				h = 60.0f * ((b - r) / (max - min)) + 120.0f;
			}
			else {
				h = 60.0f * ((r - g) / (max - min)) + 240.0f;
			}
		}

		if (h < 0.0f)
			h += 360.0f;

		return s;
	}

	inline float rgbtov(float rr, float gg, float bb) {
		const auto r = rr;
		const auto g = gg;
		const auto b = bb;

		auto h = 0.0f, s = 0.0f, v = 0.0f;

		const auto max = std::max < float >(r, std::max < float >(g, b));
		const auto min = std::min < float >(r, std::min < float >(g, b));

		v = max;

		if (max == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else if (max - min == 0.0f) {
			s = 0.0f;
			h = 0.0f;
		}
		else {
			s = (max - min) / max;

			if (max == r) {
				h = 60.0f * ((g - b) / (max - min)) + 0.0f;
			}
			else if (max == g) {
				h = 60.0f * ((b - r) / (max - min)) + 120.0f;
			}
			else {
				h = 60.0f * ((r - g) / (max - min)) + 240.0f;
			}
		}

		if (h < 0.0f)
			h += 360.0f;

		return v;
	}

}
