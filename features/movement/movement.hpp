#pragma once
#include "../../sdk/sdk.hpp"
#include <filesystem>
#include <cstddef>
#include <cstdio>

namespace features::movement {
	inline bool should_edge_bug;
	inline bool detected_ladder_glide;
	inline bool detected_normal_jump_bug = false;
	inline bool detected_normal_edge_bug = false;
	inline bool detected_normal_pixel_surf = false;
	inline bool should_ps;
	inline bool should_lj;
	inline bool should_ej;
	inline bool should_mj;
	inline bool should_lb;
	inline bool do_long_jump = false;
	inline bool do_mini_jump = false;
	inline bool did_jump_last = false;
	inline static int previous_tick = 0;
	inline int last_tick = 0;
	inline int last_delta = 0;
	inline int last_vel = 0;
	inline int pre = 0;
	inline int edgebugs = 0;
	inline int edgebug_combo = 0;
	inline int flags_backup;
	inline int saved_tick_jb = 0;
	inline int saved_tick_lj = 0;
	inline int saved_tick_ej = 0;
	inline int saved_tick_mj = 0;
	inline float last_update_time = 0.f;
	inline static float last_edgebug;
	inline float effect_alpha;
	inline float hiteffect_alpha;
	inline float last_tick_y;
	inline vec3_t first_viewangles;

	void bhop(c_usercmd* cmd);//
	void delay_hop(c_usercmd* cmd);
	void edge_bug(c_usercmd* cmd);
	void pixel_surf(c_usercmd* cmd);
	void pixel_surf_lock(c_usercmd* cmd);
	void pixel_surf_fix(c_usercmd* cmd);
	void jump_bug(c_usercmd* cmd);
	void edge_jump(c_usercmd* cmd);
	void crouch_bug(c_usercmd* cmd);
	void long_jump(c_usercmd* cmd );
	void ladder_jump(c_usercmd* cmd);
	void ladder_bug(c_usercmd* cmd);
	void null_strafing(c_usercmd* cmd);
	void mini_jump(c_usercmd* cmd);
	void auto_duck(c_usercmd* cmd);
	void fake_backwards(c_usercmd* cmd);
	void auto_strafe(c_usercmd* cmd, vec3_t& current_angle);
	void fix_movement(c_usercmd* cmd, vec3_t& angle);
	void blockbot(c_usercmd* cmd);
	void strafe_optimizer(c_usercmd* user_cmd);
	void mouse_strafe_limiter(float* x, float* y);
	void velocity_indicator();
	void stamina_indicator();
	void indicators();
	void auto_align(c_usercmd* cmd);
	void gather_vel_graph_data( );
	void velocity_graph_draw( );
	void gather_stam_graph_data( );
	void stamina_graph_draw( );
	void visualize_eb();

	struct ps_data_t {
		bool predicted_ps = false;
		bool pixelsurfing = false;
		bool pixelducking = false;
		int counter = 0;
		int pixeltick = 0;
		float last_set = 0.f;
		c_usercmd* pixelsurf_cmd = nullptr;
		vec3_t view_point = vec3_t(0, 0, 0);
		float forward_move = 0.f;
		float side_move = 0.f;
	};
	inline ps_data_t ps_data;

	struct edge_bug_detection {
		bool crouched;
		int detecttick;
		int edgebugtick;
		bool strafing;
		float yawdelta;
		float forwardmove, sidemove;
		float startingyaw;
		int eblength;
		int ticks_left;
	};
	inline edge_bug_detection detect_data;

	struct velocity_data_t {
		float speed;
		bool on_ground;
		bool edge_bug;
		bool jump_bug;
		bool pixel_surf;
	};

	inline std::vector<velocity_data_t> velocity_data;

	struct tick_info {
		tick_info() {
			velocity = g::local->velocity();
			origin = g::local->origin();
			stamina = g::local->stamina();
			in_air = !(g::local->flags() & fl_onground);
		}
		vec3_t velocity;
		vec3_t origin;
		float stamina;
		bool in_air;
	};

	inline std::deque<tick_info> move_info;
}

namespace features::calculator {
	void run();
	void draw_info();


}

class savingroute {
public:
	explicit savingroute(const char*);
	void load(size_t);
	void save(size_t, std::string name) const;
	void add(const char*);
	void remove(size_t);
	void rename(size_t, const char*);
	void listroutes();

	constexpr auto& getRoutes() {
		return routes;
	}

private:
	std::filesystem::path path;
	std::vector<std::string> routes;
};

typedef savingroute;

inline std::unique_ptr<savingroute> route;

class mrecorder {
public:
	void draw();
	void create_move(c_usercmd* cmd);
	void drawroute();
	void infowindow();
	void forcestop();
	vec3_t origin_r;
};

inline mrecorder* recorder;
