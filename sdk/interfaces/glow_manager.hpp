#pragma once
#include "../../sdk/math/vec3.hpp"

class glow_object_definition_t {
public:
	void set(float r, float g, float b, float a) {
		color = vec3_t(r, g, b);
		alpha = a;
		render_when_occluded = true;
		render_when_unoccluded = false;
		bloom_amount = 1.0f;
	}
	bool unused() {
		return next_free_slot != -2;
	}
	int next_free_slot;
	void* entity;
	vec3_t color;
	float alpha;
	bool glow_alpha_capped_by_render_alpha;
	float glow_alpha_function_of_max_velocity;
	float bloom_amount;
	float glow_pulse_overdrive;
	bool render_when_occluded;
	bool render_when_unoccluded;
	bool full_bloom_render;
	int full_bloom_stencil_test_value;
	int glow_style;
	int split_screen_slot;
};

class glow_manager_t {
public:
	glow_object_definition_t* objects;
	char pad[8];
	int size;
};