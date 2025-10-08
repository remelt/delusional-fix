#pragma once
#include "../math/vec3.hpp"
class c_overlay_text;
class color_t;
class iv_debug_overlay {
public:

	virtual void add_entity_text_overlay(int i_entity_index, int i_line_offset, float fl_duration, int r, int g, int b, int a, const char* fmt,
		...) = 0;
	virtual void add_box_overlay(const vec3_t& vec_origin, const vec3_t& vec_abs_min, const vec3_t& vec_abs_max,
		const vec3_t& ang_orientation, int r, int g, int b, int a, float fl_duration) = 0;
	virtual void add_sphere_overlay(const vec3_t& vec_origin, float fl_radius, int n_theta, int n_phi, int r, int g, int b, int a,
		float fl_duration) = 0;
	virtual void add_triangle_overlay(const vec3_t& p1, const vec3_t& p2, const vec3_t& p3, int r, int g, int b, int a, bool b_no_depth_test,
		float fl_duration) = 0;
	virtual void add_line_overlay(const vec3_t& vec_origin, const vec3_t& vec_dest, int r, int g, int b, bool b_no_depth_test,
		float fl_duration) = 0;
	virtual void add_text_overlay(const vec3_t& vec_origin, float fl_duration, const char* fmt, ...) = 0;
	virtual void add_text_overlay(const vec3_t& vec_origin, int i_line_offset, float fl_duration, const char* fmt, ...) = 0;
	virtual void add_screen_text_overlay(float fl_x_pos, float fl_y_pos, float fl_duration, int r, int g, int b, int a, const char* sz_text) = 0;
	virtual void add_swept_box_overlay(const vec3_t& vec_start, const vec3_t& vec_end, const vec3_t& vec_min, const vec3_t& vec_max,
		const vec3_t& angles, int r, int g, int b, int a, float fl_duration) = 0;
	virtual void add_grid_overlay(const vec3_t& vec_origin) = 0;
	virtual void add_coord_frame_overlay(const matrix3x4_t& mat_frame, float fl_scale, int v_color_table[3][3] = nullptr) = 0;
	virtual int screen_position(const vec3_t& vec_point, vec3_t& vec_screen) = 0;
	virtual int screen_position(float fl_x_pos, float fl_y_pos, vec3_t& vec_screen) = 0;
	virtual c_overlay_text* get_first() = 0;
	virtual c_overlay_text* get_next(c_overlay_text* p_current) = 0;
	virtual void clear_dead_overlays() = 0;
	virtual void clear_all_overlays() = 0;
	virtual void add_text_overlay_rgb(const vec3_t& vec_origin, int i_line_offset, float fl_duration, float r, float g, float b, float a,
		const char* fmt, ...) = 0;
	virtual void add_text_overlay_rgb(const vec3_t& vec_origin, int i_line_offset, float fl_duration, int r, int g, int b, int a, const char* fmt,
		...) = 0;
	virtual void add_line_overlay_alpha(const vec3_t& vec_origin, const vec3_t& dest, int r, int g, int b, int a, bool b_no_depth_test,
		float fl_duration) = 0;
	virtual void add_box_overlay2(const vec3_t& vec_origin, const vec3_t& vec_abs_min, const vec3_t& vec_abs_max,
		const vec3_t& ang_orientation, const color_t& face_color, const color_t& edge_color, float fl_duration) = 0;
	virtual void add_line_overlay(const vec3_t& vec_origin, const vec3_t& vec_dest, int r, int g, int b, int a, float fl_thickness,
		float fl_duration) = 0;
	virtual void purge_text_overlays() = 0;
	virtual void add_capsule_overlay(const vec3_t& vec_abs_min, const vec3_t& vec_abs_max, const float& fl_radius, int r, int g, int b, int a,
		float fl_duration) = 0;
	virtual void draw_pill(vec3_t& vec_abs_min, vec3_t& vec_abs_max, float fl_radius, int r, int g, int b, int a, float fl_duration) = 0;



	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn**)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}

};