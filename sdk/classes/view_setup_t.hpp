#include <cstddef>

struct view_setup_t {
	int			x;
	int			unscaled_x;
	int			y;
	int			unscaled_y;
	int			width;
	int			unscaled_width;
	int			height;
	int			unscaled_height;
	bool		ortho;
	float		ortho_left;
	float		ortho_top;
	float		ortho_right;
	float		ortho_bottom;
private:
	std::byte	pad0[0x7C];
public:
	float		fov;
	float		view_model_fov;
	vec3_t		origin;
	vec3_t		view;
	float		near_z;
	float		far_z;
	float		near_viewmodel_z;
	float		far_viewmodel_z;
	float		aspect_ratio;
	float		near_blur_depth;
	float		near_focus_depth;
	float		far_focus_depth;
	float		far_blur_depth;
	float		near_blur_radius;
	float		far_blur_radius;
	float		do_f_quality;
	int			motion_blur_mode;
	float		shutter_time;
	vec3_t		shutter_open_position;
	vec3_t		shutter_open_angles;
	vec3_t		shutter_close_position;
	vec3_t		shutter_close_angles;
	float		off_center_top;
	float		off_center_bottom;
	float		off_center_left;
	float		off_center_right;
	bool		off_center : 1;
	bool		render_to_subrect_of_larger_screen : 1;
	bool		do_bloom_and_tone_mapping : 1;
	bool		do_depth_of_field : 1;
	bool		hdr_target : 1;
	bool		draw_world_normal : 1;
	bool		cull_font_faces : 1;
	bool		cache_full_scene_state : 1;
	bool		csm_view : 1;
};