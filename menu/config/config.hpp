#pragma once
#include <string_view>
#include <vector>
#include "../../includes/nlohmann/json.hpp"

namespace c {
	void create_directory();
	void update_configs();
	void create_file(const std::string_view name);
	void delete_file(const std::size_t index);
	void save(const std::size_t index);
	void load(const std::size_t index);
	void load_aimbot(const std::size_t index);
	void load_mvm(const std::size_t index);
	void load_visuals(const std::size_t index);
	void load_skins(const std::size_t index);
	void load_misc(const std::size_t index);
	void load_indicators(const std::size_t index);
	inline std::string directory_path_fonts;
	inline std::string directory_path;
	inline std::string directory;

	namespace backtrack {
		inline bool backtrack = false;
		inline int time = 0;
		inline float fake = 0;
		inline bool fake_latency = false;
		inline bool selected_tick = false;
	}

	namespace triggerbot {
		inline bool enabled = false;
		inline int triggerbot_key;
		inline int triggerbot_key_s = 1;
	}

	namespace assist
	{
		inline bool assist = false;
		inline bool pixelsurf_assist = false;
		inline bool bounce_assist = false;

		inline bool assist_broke_hop = false;
		inline float assist_stamina_value = 100.0f;
		inline bool assist_render = false;
		inline bool bounce_assist_render = false;
		inline int assist_render_style = 0;

		inline bool assist_bounce_broke_hop = false;

		inline int pixelsurf_assist_ticks = 12;

		inline int pixelsurf_max_points = 1;

		inline int pixelsurf_assist_type = 0;

		inline int pixelsurf_assist_key;
		inline int pixelsurf_assist_key_s = 1;

		inline int pixelsurf_line_key;
		inline int pixelsurf_line_key_s = 1;

		inline int pixelsurf_point_key;
		inline int pixelsurf_point_key_s = 1;

		inline int bounce_assist_key;
		inline int bounce_assist_key_s = 1;

		inline int bounce_point_key;
		inline int bounce_point_key_s = 1;

	}

	namespace aimbot {
		inline bool aimbot;
		inline bool aimbot_panic;
		inline int aimbot_key;
		inline int aimbot_key_s = 1;
		inline int aimbot_panic_key;
		inline int aimbot_panic_key_s = 1;
		inline int aimbot_only_enemy;
		inline bool aimbot_silent;
		inline bool aimbot_autoshoot = 0;
		inline bool rcs;
		inline bool non_sticky_aimbot;
		inline bool aim_at_bt;
		inline bool pistol_autowall;
		inline int pistol_autowall_dmg = 1;
		inline bool pistol_autowall_lethal;
		inline int pistol_aimbot_fov;
		inline bool pistol_aimbot_rcs;
		inline int pistol_aimbot_rcs_p;
		inline bool pistol_aimbot_silent;
		inline int pistol_aimbot_smooth;
		inline bool hitboxes_pistol[4];
		inline int pistol_hitbox;
		inline bool heavy_pistol_autowall;
		inline int heavy_pistol_autowall_dmg = 1;
		inline bool heavy_pistol_autowall_lethal;
		inline int heavy_pistol_aimbot_fov;
		inline bool heavy_pistol_aimbot_rcs;
		inline int heavy_pistol_aimbot_rcs_p;
		inline bool heavy_pistol_aimbot_silent;
		inline int heavy_pistol_aimbot_smooth;
		inline bool hitboxes_heavy_pistol[4];
		inline int heavy_pistol_hitbox;
		inline bool shotgun_autowall;
		inline int shotgun_autowall_dmg = 1;
		inline bool shotgun_autowall_lethal;
		inline int shotgun_aimbot_fov;
		inline bool shotgun_aimbot_rcs;
		inline int shotgun_aimbot_rcs_p;
		inline bool shotgun_aimbot_silent;
		inline int shotgun_aimbot_smooth;
		inline bool hitboxes_shotgun[4];
		inline int shotgun_hitbox;
		inline bool heavy_autowall;
		inline int heavy_autowall_dmg = 1;
		inline bool heavy_autowall_lethal;
		inline int heavy_aimbot_fov;
		inline bool heavy_aimbot_rcs;
		inline int heavy_aimbot_rcs_p;
		inline bool heavy_aimbot_silent;
		inline int heavy_aimbot_smooth;
		inline bool hitboxes_heavy[4];
		inline int heavy_hitbox;
		inline bool smg_autowall;
		inline int smg_autowall_dmg = 1;
		inline bool smg_autowall_lethal;
		inline int smg_aimbot_fov;
		inline bool smg_aimbot_rcs;
		inline int smg_aimbot_rcs_p;
		inline bool smg_aimbot_silent;
		inline int smg_aimbot_smooth;
		inline bool hitboxes_smg[4];
		inline int smg_hitbox;
		inline bool rifle_autowall;
		inline int rifle_autowall_dmg = 1;
		inline bool rifle_autowall_lethal;
		inline int rifle_aimbot_fov;
		inline bool rifle_aimbot_rcs;
		inline int rifle_aimbot_rcs_p;
		inline bool rifle_aimbot_silent;
		inline int rifle_aimbot_smooth;
		inline bool hitboxes_rifle[4];
		inline int rifle_hitbox;
		inline bool sniper_autowall;
		inline int sniper_autowall_dmg = 1;
		inline bool sniper_autowall_lethal;
		inline int sniper_aimbot_fov;
		inline bool sniper_aimbot_rcs;
		inline int sniper_aimbot_rcs_p;
		inline bool sniper_aimbot_silent;
		inline int sniper_aimbot_smooth;
		inline bool hitboxes_sniper[4];
		inline int sniper_hitbox;
		inline bool autosniper_autowall;
		inline int autosniper_autowall_dmg = 1;
		inline bool autosniper_autowall_lethal;
		inline int autosniper_aimbot_fov;
		inline bool autosniper_aimbot_rcs;
		inline int autosniper_aimbot_rcs_p;
		inline bool autosniper_aimbot_silent;
		inline int autosniper_aimbot_smooth;
		inline bool hitboxes_autosniper[4];
		inline int autosniper_hitbox;
	}

	namespace movement {
		inline int dh_tick = 1;
		inline float stamina_fade_clr[4]{ 1.f, 1.f, 1.f };
		inline bool auto_strafe = false;
		inline int  auto_strafe_key = 0;
		inline int  auto_strafe_key_s = 1;
		inline bool movement_fix = true;
		inline bool edgebug_pena = false;
		inline int fix_type = 0;
		inline bool bhop = false;
		inline bool bhopmiss = false;
		inline bool bhopfix = false;
		inline int whathopmiss = 6;
		inline bool null_strafing = false;
		inline bool ladder_bug = false;
		inline int ladder_bug_key = 0;
		inline int ladder_bug_key_s = 1;
		inline bool air_stuck = false;
		inline int air_stuck_key = 0;
		inline int air_stuck_key_s = 1;
		inline bool fireman = false;
		inline bool fireman_plus_jump_activates_early = false;
		inline bool fireman_manual_jump = false;
		inline int fireman_key = 0;
		inline int fireman_key_s = 1;
		inline bool delay_hop = false;
		inline int  delay_hop_key = 0;
		inline int  delay_hop_key_s = 1;
		inline bool crouch_bug = false;
		inline int crouch_bug_key = 0;
		inline int crouch_bug_key_s = 1;
		inline bool visualize_edge_bug = false;
		inline float visualize_edge_bug_clr[4]{ 1.f, 1.f, 1.f };
		inline bool edge_bug = false;
		inline bool edge_bug_advanced_search = false;
		inline bool edge_bug_strafe = false;
		inline int edge_bug_priority = 0;
		inline int edge_bug_key = 0;
		inline int edge_bug_key_s = 1;
		inline int edgebug_type = 0;
		inline bool AutoStrafeEdgeBug = false;
		inline bool EdgeBugAdvanceSearch = false;
		inline bool SiletEdgeBug = false;
		inline bool MegaEdgeBug = false;
		inline int EdgeBugCircle = 2;
		inline float deltascaler = 1.f;
		inline int DeltaType = 0;
		inline int EdgeBugTicks = 64;
		inline float EdgeBugMouseLock = 0.f;
		inline int edge_bug_ticks = 64;
		inline int edge_bug_rape = 5;
		inline bool silent_eb_hacked = false;
		inline float edge_bug_angle_limit = 50.0f;
		inline float edge_bug_lock_amount = 0.01f;
		inline int edge_bug_lock_type = 0;
		inline int edge_bug_type = 0;
		inline float edge_bug_health_boost_effect = 0.f;
		inline int edge_bug_detection_sound = 0;
		inline bool edge_bug_counter = false;
		inline bool edge_jump = false;
		inline bool edge_jump_on_ladder = false;
		inline int edge_jump_key = 0;
		inline int edge_jump_key_s = 1;
		inline int long_jump_key = 0;
		inline int long_jump_key_s = 1;
		inline bool long_jump_on_edge = false;
		inline bool lj_null[4] = { false, false, false, false };
		inline bool mini_jump = false;
		inline bool mini_jump_ej = false;
		inline bool mini_jump_remain_crouched = false;
		inline int mini_jump_key = 0;
		inline int mini_jump_key_s = 1;
		inline bool adaptive_key_cancelling = false;
		inline bool adaptive_key_for[3] = { false, false, false };
		inline bool strafe_optimizer = false;
		inline int strafe_optimizer_key = 0;
		inline int strafe_optimizer_key_s = 1;
		inline float strafe_optimizer_desired_gain = 0.f;
		inline float strafe_optimizer_required_speed = 0.f;
		inline bool mouse_strafe_limiter = false;
		inline bool mouse_strafe_limiter_affect_pitch = false;
		inline float mouse_strafe_limiter_value = 20.f;
		inline int mouse_strafe_limiter_key = 0;
		inline int mouse_strafe_limiter_key_s = 1;
		inline bool jump_bug = false;
		inline int jump_bug_key = 0;
		inline int jump_bug_key_s = 1;
		inline bool fastduck = false;
		inline bool auto_align = false;
		inline int align_selection = 1;
		inline int px_selection = 1;
		inline bool freelook_surf = true;
		inline bool fast_ladder = false;
		inline int fast_ladder_key = 0;
		inline int fast_ladder_key_s = 1;
		inline bool auto_duck = false;
		inline bool auto_duck_collision = false;
		inline int auto_duck_collision_key = 0;
		inline int auto_duck_collision_key_s = 1;
		inline int auto_duck_collision_ticks = 6;
		inline int  auto_duck_key = 0;
		inline int  auto_duck_key_s = 1;
		inline int  auto_duck_ticks = 6;
		inline bool pixel_surf_fix = false;
		inline bool pixel_surf = false;
		inline bool adjust_view = true;
		inline bool adjust_view_always = false;
		inline bool crouch_fix = false;
		inline int  pixel_surf_key = 0;
		inline int  pixel_surf_key_s = 1;
		inline int pixel_surf_ticks = 16;
		inline int lb_pixel_surf_ticks = 8;
		inline bool fakebackwards = false;
		inline int  fakebackwardskey = 0;
		inline int  fakebackwardskey_s = 1;
		inline int  fakebackwards_angle = 0;
		inline bool edge_bug_detection_printf = false;
		inline bool jump_bug_detection_printf = false;
		inline bool long_jump_detection_printf = false;
		inline bool mini_jump_detection_printf = false;
		inline bool pixel_surf_detection_printf = false;
		inline bool ladder_bug_detection_printf = false;
		inline bool velocity_indicator = false;
		inline int velocity_indicator_position = 50;
		inline int indicators_position = 50;
		inline int indicators_gap = 4;
		inline float indicators_fading_speed = 65.f;
		inline bool indicators_allow_animation = false;
		inline bool velocity_indicator_show_pre = false;
		inline bool velocity_indicator_disable_ong_show_pre = false;
		inline bool velocity_indicator_custom_color = false;
		inline float velocity_indicator_positive_clr[4]{ 0.11765f, 1.00000f, 0.42745f };
		inline float velocity_indicator_negative_clr[4]{ 1.00000f, 0.46667f, 0.46667f };
		inline float velocity_indicator_neutral_clr[4]{ 1.00000f, 0.78039f, 0.34902f };
		inline float velocity_indicator_custom_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float velocity_indicator_custom_clr2[4]{ 1.f, 1.f, 1.f };
		inline float velocity_indicator_fade_clr3[4]{ 1.f, 1.f, 1.f, 0.f };
		inline float indicator_detect_clr[3]{ 0.195f, 0.750f, 0.269f };
		inline bool indicators_show[14] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false };
		inline bool allow_detection_clr = false;
		inline int detection_saved_tick = 15;
		inline bool detection_clr_for[14] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false };
		inline bool stamina_indicator = false;
		inline bool stamina_indicator_fade = false;
		inline bool stamina_indicator_show_pre = false;
		inline bool stamina_indicator_disable_ong_show_pre = false;
		inline int stamina_indicator_position = 275;
		inline float stamina_indicator_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool stamina_graph = false;
		inline float stamina_graph_color[3]{ 1.f, 1.f, 1.f };
		inline int stamina_graph_offset = -700;
		inline int stamina_graph_height = 100;
		inline int stamina_graph_offset_x = 500;
		inline int graph_fade = 1;
		inline bool velocity_graph = false;
		inline float velocity_graph_color[3]{ 1.f, 1.f, 1.f };
		inline bool velocity_graph_show_landed_speed = false;
		inline bool velocity_graph_show_edge_bug = false;
		inline bool velocity_graph_show_jump_bug = false;
		inline bool velocity_graph_show_pixel_surf = false;
		inline float velocity_linewidth = 3.f;
		inline int velocity_thickness = 1;
		inline float velocity_height = 4.f;
		inline int velocity_size = 185;
		inline float graph_xscreenpos = 0.800f;
		inline float velocity_tofade = 1.f;
		inline bool key_strokes;
		inline int key_strokes_position = 200;
	}

	namespace misc {
		inline bool headshot_spoofer = false;
		inline bool anti_untrusted = true;
		inline bool insecure_bypass = true;
		inline bool unlock_inventory = false;
		inline bool custom_region = false;
		inline int custom_region_selection = 0;
		inline bool mouse_fix = false;
		inline bool keybind_list = false;
		inline bool door_spam = false;
		inline int door_spam_key = 0;
		inline int door_spam_key_s = 1;
		inline bool force_crosshair = false;
		inline bool sniper_crosshair = false;
		inline bool watermark = true;
		inline bool show_spotify_currently_playing = false;
		inline bool spotify_fps_boost = false;
		inline bool progressbar_enable = false;
		inline int player_type = 0;
		inline bool spectators_list = true;
		inline float spectators_list_color_1[3]{ 1.f, 1.f, 1.f };
		inline float spectators_list_color_2[3]{ 1.f, 1.f, 1.f };
		inline float spectators_list_clr[3]{ 0.11765, 0.11765, 0.11765 };
		inline float spectators_list_clr_2[3]{ 0.11765, 0.11765, 0.11765 };
		inline int spectatorlist_type = 0;
		inline bool spectator_local = false;
		inline bool spectatorlist_show_target = true;
		inline int spectatorlist_x = 100;
		inline int spectatorlist_y = 50;
		inline int spectatorlist_w;
		inline int spectatorlist_h;
		inline bool practice = false;
		inline bool practice_window = false;
		inline int  savepos = 0;
		inline int  savepos_s = 1;
		inline int  loadpos = 0;
		inline int  loadpos_s = 1;
		inline int  nextkey = 0;
		inline int  nextkey_s = 1;
		inline int  prevkey = 0;
		inline int  prevkey_s = 1;
		inline int  undokey = 0;
		inline int  undokey_s = 1;
		inline bool enable_fov = false;
		inline float field_of_view = 0.f;
		inline bool view_model = false;
		inline float view_model_x = 0.f;
		inline float view_model_y = 0.f;
		inline float view_model_z = 0.f;
		inline bool aspect_ratio = false;
		inline float aspect_ratio_amount = 1.f;
		inline bool custom_console = false;
		inline float custom_console_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool discord_rpc = false;
		inline bool radaringame = false;
		inline bool jumpstats;
		inline bool jumpstats_show_clr_fail;
		inline bool jumpstats_show_fail;
		inline bool swayscale = false;
		inline float swayscale_value = 0.f;
		inline char misc_clantag_text[256] = "";
		inline bool misc_animated_clantag = false;
		inline bool misc_clantag_spammer = false;
		inline bool misc_clantag_rotation = false;
		inline int misc_clantag_type = 0;
		inline float misc_clantag_speed = 1.0f;
		inline bool misc_hitmarker = false;
		inline bool misc_hitmarker_sound = false;
		inline bool misc_hitmarker_screen_effect = false;
		inline bool misc_hit_info[2] = { false, false};
		inline int misc_hitmarker_sound_type = 0;
		inline bool misc_reveal_ranks = false;
		inline bool nadepred = false;
		inline float nadepred_clr[3]{ 1.f, 1.f, 1.f };
		inline bool autoaccept = false;
		inline bool vote_revealer = false;
		inline bool thirdperson = false;
		inline bool thirdperson_disabled_on_weapon = false;
		inline int thirdperson_distance = 150;
		inline int thirdperson_key = 0;
		inline int thirdperson_key_s = 1;
		inline bool freecam = false;
		inline int freecam_key = 0;
		inline int freecam_key_s = 1;
		inline bool misc_kill_msg = false;
		inline char misc_kill_message[256] = "Hello World!";
		inline int menu_key = 0x2D; //ins
		inline bool movement_rec = false;
		inline bool movement_rec_render = true;
		inline int movement_rec_position = 0;
		inline bool movement_rec_show_line = true;
		inline bool movement_rec_infowindow = false;
		inline float movement_rec_smoothing = 1.f;
		inline int movement_rec_maxrender = 528;
		inline float movement_rec_infowindowx;
		inline float movement_rec_infowindowy;
		inline bool movement_rec_lockva = true;
		inline bool movement_rec_lockgoingtostart = false;
		inline float movement_rec_ringsize = 40;
		inline int movement_rec_keystartrecord = 0;
		inline int movement_rec_keystartrecord_s = 1;
		inline int movement_rec_keystoprecord = 0;
		inline int movement_rec_keystoprecord_s = 1;
		inline int movement_rec_keystartplayback = 0;
		inline int movement_rec_keystartplayback_s = 1;
		inline int movement_rec_keystopplayback = 0;
		inline int movement_rec_keystopplayback_s = 1;
		inline int movement_rec_keyclearrecord = 0;
		inline int movement_rec_keyclearrecord_s = 1;
		inline int movement_rec_angletype = 0;
	}

	namespace skins {
		inline bool agent_changer = false;
		inline int agent_t = 0;
		inline int agent_ct = 0;

		inline bool knife_changer_enable = false;
		inline int knife_changer_wear = 0;
		inline int knife_changer_model = 0;
		inline int knife_changer_skin_id = 0;
		inline int knife_changer_paint_kit = 0;
		inline int knife_changer_vector_paint_kit = 0;
		inline int knife_skin = 0;

		inline bool gloves_endable = false;
		inline int gloves_model = 0;
		inline int gloves_skin = 0;
		inline int gloves_skin_id = 0;
		inline int gloves_wear = 0;
		inline int agent_model = 0;

		//1
		inline int weapon_model = 0;

		//usp
		inline int wear_usp = 0;
		inline int vector_paint_kit_usp = 0;
		inline int paint_kit_index_usp = 0;
		inline bool usp_wpn_skin_custom_clr = false;
		inline float usp_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float usp_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float usp_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float usp_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//p2000
		inline int wear_p2000 = 0;
		inline int vector_paint_kit_p2000 = 0;
		inline int paint_kit_index_p2000 = 0;
		inline bool p2000_wpn_skin_custom_clr = false;
		inline float p2000_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float p2000_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float p2000_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float p2000_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//glock
		inline int wear_glock = 0;
		inline int vector_paint_kit_glock = 0;
		inline int paint_kit_index_glock = 0;
		inline bool glock_wpn_skin_custom_clr = false;
		inline float glock_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float glock_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float glock_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float glock_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//p250
		inline int wear_p250 = 0;
		inline int vector_paint_kit_p250 = 0;
		inline int paint_kit_index_p250 = 0;
		inline bool p250_wpn_skin_custom_clr = false;
		inline float p250_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float p250_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float p250_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float p250_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//fiveseven
		inline int wear_fiveseven = 0;
		inline int vector_paint_kit_fiveseven = 0;
		inline int paint_kit_index_fiveseven = 0;
		inline bool fiveseven_wpn_skin_custom_clr = false;
		inline float fiveseven_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float fiveseven_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float fiveseven_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float fiveseven_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//tec
		inline int wear_tec = 0;
		inline int vector_paint_kit_tec = 0;
		inline int paint_kit_index_tec = 0;
		inline bool tec_wpn_skin_custom_clr = false;
		inline float tec_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float tec_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float tec_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float tec_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//cz
		inline int wear_cz = 0;
		inline int vector_paint_kit_cz = 0;
		inline int paint_kit_index_cz = 0;
		inline bool cz_wpn_skin_custom_clr = false;
		inline float cz_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float cz_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float cz_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float cz_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//duals
		inline int wear_duals = 0;
		inline int vector_paint_kit_duals = 0;
		inline int paint_kit_index_duals = 0;
		inline bool duals_wpn_skin_custom_clr = false;
		inline float duals_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float duals_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float duals_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float duals_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//deagle
		inline int wear_deagle = 0;
		inline int vector_paint_kit_deagle = 0;
		inline int paint_kit_index_deagle = 0;
		inline bool deagle_wpn_skin_custom_clr = false;
		inline float deagle_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float deagle_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float deagle_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float deagle_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//revolver
		inline int wear_revolver = 0;
		inline int vector_paint_kit_revolver = 0;
		inline int paint_kit_index_revolver = 0;
		inline bool revolver_wpn_skin_custom_clr = false;
		inline float revolver_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float revolver_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float revolver_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float revolver_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//famas
		inline int wear_famas = 0;
		inline int vector_paint_kit_famas = 0;
		inline int paint_kit_index_famas = 0;
		inline bool famas_wpn_skin_custom_clr = false;
		inline float famas_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float famas_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float famas_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float famas_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//galil
		inline int wear_galil = 0;
		inline int vector_paint_kit_galil = 0;
		inline int paint_kit_index_galil = 0;
		inline bool galil_wpn_skin_custom_clr = false;
		inline float galil_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float galil_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float galil_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float galil_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//m4a4
		inline int wear_m4a4 = 0;
		inline int vector_paint_kit_m4a4 = 0;
		inline int paint_kit_index_m4a4 = 0;
		inline bool m4a4_wpn_skin_custom_clr = false;
		inline float m4a4_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float m4a4_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float m4a4_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float m4a4_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//m4a1
		inline int wear_m4a1 = 0;
		inline int vector_paint_kit_m4a1 = 0;
		inline int paint_kit_index_m4a1 = 0;
		inline bool m4a1_wpn_skin_custom_clr = false;
		inline float m4a1_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float m4a1_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float m4a1_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float m4a1_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//ak47
		inline int wear_ak47 = 0;
		inline int vector_paint_kit_ak47 = 0;
		inline int paint_kit_index_ak47 = 0;
		inline bool ak47_wpn_skin_custom_clr = false;
		inline float ak47_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float ak47_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float ak47_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float ak47_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//sg553
		inline int wear_sg553 = 0;
		inline int vector_paint_kit_sg553 = 0;
		inline int paint_kit_index_sg553 = 0;
		inline bool sg553_wpn_skin_custom_clr = false;
		inline float sg553_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float sg553_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float sg553_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float sg553_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//aug
		inline int wear_aug = 0;
		inline int vector_paint_kit_aug = 0;
		inline int paint_kit_index_aug = 0;
		inline bool aug_wpn_skin_custom_clr = false;
		inline float aug_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float aug_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float aug_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float aug_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//ssg08
		inline int wear_ssg08 = 0;
		inline int vector_paint_kit_ssg08 = 0;
		inline int paint_kit_index_ssg08 = 0;
		inline bool ssg08_wpn_skin_custom_clr = false;
		inline float ssg08_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float ssg08_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float ssg08_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float ssg08_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//awp
		inline int wear_awp = 0;
		inline int vector_paint_kit_awp = 0;
		inline int paint_kit_index_awp = 0;
		inline bool awp_wpn_skin_custom_clr = false;
		inline float awp_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float awp_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float awp_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float awp_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//scar
		inline int wear_scar = 0;
		inline int vector_paint_kit_scar = 0;
		inline int paint_kit_index_scar = 0;
		inline bool scar_wpn_skin_custom_clr = false;
		inline float scar_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float scar_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float scar_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float scar_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//g3sg1
		inline int wear_g3sg1 = 0;
		inline int vector_paint_kit_g3sg1 = 0;
		inline int paint_kit_index_g3sg1 = 0;
		inline bool g3sg1_wpn_skin_custom_clr = false;
		inline float g3sg1_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float g3sg1_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float g3sg1_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float g3sg1_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//sawoff
		inline int wear_sawoff = 0;
		inline int vector_paint_kit_sawoff = 0;
		inline int paint_kit_index_sawoff = 0;
		inline bool sawoff_wpn_skin_custom_clr = false;
		inline float sawoff_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float sawoff_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float sawoff_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float sawoff_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//m249
		inline int wear_m249 = 0;
		inline int vector_paint_kit_m249 = 0;
		inline int paint_kit_index_m249 = 0;
		inline bool m249_wpn_skin_custom_clr = false;
		inline float m249_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float m249_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float m249_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float m249_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//negev
		inline int wear_negev = 0;
		inline int vector_paint_kit_negev = 0;
		inline int paint_kit_index_negev = 0;
		inline bool negev_wpn_skin_custom_clr = false;
		inline float negev_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float negev_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float negev_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float negev_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//mag7
		inline int wear_mag7 = 0;
		inline int vector_paint_kit_mag7 = 0;
		inline int paint_kit_index_mag7 = 0;
		inline bool mag7_wpn_skin_custom_clr = false;
		inline float mag7_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float mag7_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float mag7_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float mag7_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//xm1014
		inline int wear_xm1014 = 0;
		inline int vector_paint_kit_xm1014 = 0;
		inline int paint_kit_index_xm1014 = 0;
		inline bool xm1014_wpn_skin_custom_clr = false;
		inline float xm1014_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float xm1014_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float xm1014_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float xm1014_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//nova
		inline int wear_nova = 0;
		inline int vector_paint_kit_nova = 0;
		inline int paint_kit_index_nova = 0;
		inline bool nova_wpn_skin_custom_clr = false;
		inline float nova_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float nova_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float nova_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float nova_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//bizon
		inline int wear_bizon = 0;
		inline int vector_paint_kit_bizon = 0;
		inline int paint_kit_index_bizon = 0;
		inline bool bizon_wpn_skin_custom_clr = false;
		inline float bizon_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float bizon_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float bizon_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float bizon_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//mp5sd
		inline int wear_mp5sd = 0;
		inline int vector_paint_kit_mp5sd = 0;
		inline int paint_kit_index_mp5sd = 0;
		inline bool mp5sd_wpn_skin_custom_clr = false;
		inline float mp5sd_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float mp5sd_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float mp5sd_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float mp5sd_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//mp7
		inline int wear_mp7 = 0;
		inline int vector_paint_kit_mp7 = 0;
		inline int paint_kit_index_mp7 = 0;
		inline bool mp7_wpn_skin_custom_clr = false;
		inline float mp7_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float mp7_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float mp7_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float mp7_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//mp9
		inline int wear_mp9 = 0;
		inline int vector_paint_kit_mp9 = 0;
		inline int paint_kit_index_mp9 = 0;
		inline bool mp9_wpn_skin_custom_clr = false;
		inline float mp9_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float mp9_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float mp9_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float mp9_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//mac10
		inline int wear_mac10 = 0;
		inline int vector_paint_kit_mac10 = 0;
		inline int paint_kit_index_mac10 = 0;
		inline bool mac10_wpn_skin_custom_clr = false;
		inline float mac10_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float mac10_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float mac10_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float mac10_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//p90
		inline int wear_p90 = 0;
		inline int vector_paint_kit_p90 = 0;
		inline int paint_kit_index_p90 = 0;
		inline bool p90_wpn_skin_custom_clr = false;
		inline float p90_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float p90_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float p90_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float p90_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };

		//ump45
		inline int wear_ump45 = 0;
		inline int vector_paint_kit_ump45 = 0;
		inline int paint_kit_index_ump45 = 0;
		inline bool ump45_wpn_skin_custom_clr = false;
		inline float ump45_wpn_skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float ump45_wpn_skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float ump45_wpn_skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float ump45_wpn_skin_modulation4[3]{ 1.f, 1.f, 1.f };


		inline bool skin_custom_clr = false;
		inline float skin_modulation1[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation2[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation3[3]{ 1.f, 1.f, 1.f };
		inline float skin_modulation4[3]{ 1.f, 1.f, 1.f };

		inline bool weapon_endable = false;
		inline int weapons_page = 0;
		inline int weapons_model = 0;

	}

	namespace visuals {
		inline bool enable_weather = false;
		inline int weather_type = 0;
		inline bool enable_visuals = false;
		inline bool fade_animation = false;
		inline bool enemy_only = false;
		inline bool playerbox = false;
		inline bool apply_zoom = false;
		inline int apply_zoom_key = 0;
		inline int apply_zoom_key_s = 1;
		inline int boxtype = 0;
		inline float corner_lenght = 5.f;
		inline bool playerarmor = false;
		inline float playerarmor_clr[3]{ 0.f, 0.31f, 1.0f };
		inline bool playerammo = false;
		inline float playerammo_clr[3]{ 1.f, 1.f, 1.f };
		inline float player_weapon_color[3]{ 1.f, 1.f, 1.f };
		inline int dropped_weapon_type = 0;
		inline bool player_box_outline[2] = { false, false};
		inline float playerbox_color[3]{ 1.f, 1.f, 1.f };
		inline bool playername = false;
		inline float playername_color[3]{ 1.f, 1.f, 1.f };
		inline bool playerweapon = false;
		inline bool player_weapon_type[2] = { false, false };
		inline float dropped_weapon_icon_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool dropped_weapon_name = false;
		inline bool dropped_weapon_icon = false;
		inline float dropped_weapon_name_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float dropped_weapon_distance = 150.f;
		inline bool dropped_weapon_glow = false;
		inline float dropped_weapon_glow_clr[3]{ 1.f, 1.f, 1.f };
		inline bool bomb_esp = false;
		inline bool healthesp = false;
		inline int healthesp_s = 0;
		inline float health_bar[3]{ 1.f, 1.f, 1.f };
		inline float health_color[3]{ 1.f, 1.f, 1.f };
		inline float health_bar_gradient[3]{ 1.f, 1.f, 1.f };
		inline int health_bar_width = 1;
		inline bool override_bar = false;
		inline bool gradient_bar = false;
		inline bool skeleton = false;
		inline bool skeletonesp = false;
		inline float skeletonesp_clr[3]{ 1.f, 1.f, 1.f };
		inline float skeletonbt_clr[3]{ 1.f, 1.f, 1.f };
		inline float skeleton_last_clr[3]{ 1.f, 1.f, 1.f };
		inline float selected_tick_clr[3]{ 1.f, 1.f, 1.f };
		inline bool skeleton_last = false;
		inline bool glow = false;
		inline bool glow_weapon = false;
		inline int glow_style = 0;
		inline bool invisibleglow = false;
		inline bool visibleglow = false;
		inline float glow_visible_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float glow_invisible_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float glow_weapon_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool oof_arrows;
		inline int oof_arrows_size;
		inline int oof_arrows_dist;
		inline float oof_arrows_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float oof_arrows_clr2[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool  radar = false;
		inline bool  mbenabled = false;
		inline bool  mbforwardEnabled = false;
		inline float mbfallingMin = 10.0f;
		inline float mbfallingMax = 20.0f;
		inline float mbfallingIntensity = 1.0f;
		inline float mbrotationIntensity = 1.0f;
		inline float mbstrength = 1.0f;
		inline bool fog;
		inline int fog_distance;
		inline int fog_density;
		inline float fog_color[4]{ 1.f, 1.f, 1.f, 1.f };
		inline int skybox;
		inline int flashalpha;
		inline bool change_flashalpha = false;
		inline bool nosmoke = false;
		inline bool trails;
		inline float trails_clr1[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float trails_clr2[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool visuals_flags[6] = { false, false, false, false, false, false };
		inline bool removals[5] = { false, false , false , false , false };
		inline float world_color[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool world_modulate = false;
		inline bool fullbright;
		inline bool shadows = false;
		inline bool dynamic_shadows = false;
		inline float shadow_rot_x = 0.f;
		inline float shadow_rot_y = 0.f;
		inline float shadow_rot_z = 0.f;
		inline float shadow_rotation_speed = 1.f;
		inline bool dlight = false;
		inline float dlight_clr[4]{ 1.f, 1.f, 1.f, 255.f };
		inline bool gravity_ragdoll = false;
	}

	namespace chams {
		inline bool backtrack_chams = false;
		inline bool backtrack_chams_gradient = false;
		inline bool backtrack_chams_invisible = false;
		inline bool backtrack_chams_draw_all_ticks = false;
		inline float backtrack_chams_clr1[4]{ 1.f, 1.f, 1.f, 1.f };
		inline float backtrack_chams_clr2[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool visible_chams = false;
		inline bool visible_wireframe = false; //
		inline float visible_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool visible_chams_ov = false; //
		inline bool visible_wireframe_ov = false; //
		inline float visible_chams_clr_ov[4]{ 1.f, 1.f, 1.f, 1.f }; //
		inline bool invisible_chams = false;
		inline float invisible_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool invisible_wireframe = false; //
		inline bool sleeve_chams = false; //
		inline float sleeve_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool sleeve_wireframe = false; //
		inline bool arms_chams = false; //
		inline float arms_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool arms_wireframe = false; //
		inline bool sleeve_chams_ov = false;
		inline float sleeve_chams_clr_ov[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool sleeve_wireframe_ov = false; //
		inline bool arms_chams_ov = false;
		inline float arms_chams_clr_ov[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool arms_wireframe_ov = false; //
		inline bool wpn_chams = false; //
		inline float wpn_chams_clr[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool wpn_wireframe = false; //
		inline bool wpn_chams_ov = false; //
		inline float wpn_chams_clr_ov[4]{ 1.f, 1.f, 1.f, 1.f };
		inline bool wpn_wireframe_ov = false; //
		inline int cham_type_bt = 0;
		inline int cham_type = 0;
		inline int cham_type_overlay = 0; //
		inline int cham_type_sleeve = 0;
		inline int cham_type_sleeve_ov = 0;
		inline int cham_type_wpn = 0;
		inline int cham_type_wpn_ov = 0;
		inline int cham_type_arms = 0; //
		inline int cham_type_arms_ov = 0; //
		inline int chams_backrack_ticks = 0;
		inline int localtype = 0;
		inline int type_p = 0;
	}

	namespace fonts {

		inline int indi_size = 28.;
		inline int indi_font = 0;
		inline bool indi_font_flag[11] = { false, false, false, false, false, false, false, false, false, true, false };

		inline int sub_indi_size = 28.;
		inline int sub_indi_font = 0;
		inline bool sub_indi_font_flag[11] = { false, false, false, false, false, false, false, false, false, true, false };

		inline int esp_hp_font = 0;
		inline int esp_hp_size = 12;
		inline bool esp_font_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int sc_logs_font = 0;
		inline int sc_logs_size = 12;
		inline bool sc_logs_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int esp_name_font = 0;
		inline int esp_name_size = 12;
		inline bool esp_name_font_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int esp_wpn_font = 0;
		inline int esp_wpn_size = 12;
		inline bool esp_wpn_font_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int esp_dropped_wpn_font = 0;
		inline int esp_dropped_wpn_size = 12;
		inline bool esp_dropped_wpn_font_flag[11] = { false, false, false, true, false, false, true, false, false, true, false };

		inline int esp_font = 0;

		inline int spec_font = 0;
		inline int spec_size = 12;
		inline bool spec_font_flag[9] = { false, false, false, true, false, false, true, false, false };

		inline int watermark_font = 0;
		inline int watermark_size = 12;
		inline bool watermark_font_flag[9] = { false, false, false, true, false, false, true, false, false };

		inline int assist_font = 0;
		inline int assist_size = 12;
		inline bool assist_font_flag[9] = { false, false, false, true, false, false, true, false, false };

		inline int recorder_font = 0;
		inline int recorder_size = 12;
		inline bool recorder_font_flag[9] = { false, false, false, true, false, false, true, false, false };

		inline int lb_player_font = 0;
		inline int lb_player_size = 12;
		inline bool lb_player_font_flag[9] = { false, false, false, true, false, false, true, false, false };

		inline int sub_spec_font = 0;
		inline bool sub_spec_font_flag[9] = { false, false, false, true, false, false, true, false, false };
	}

	namespace sfui {
		inline bool sfui_on = false;
	}

	namespace calculator {
		inline bool ps_calcualtor = false;
		inline int  toggle_wireframe_key = 0;
		inline int  toggle_wireframe_key_s = 2;
		inline int  toggle_clipbrushe_key = 0;
		inline int  toggle_clipbrushe_key_s = 2;
		inline int  set_point_key = 0;
		inline int  set_point_key_s = 1;
		inline int  calculate_point_key = 0;
		inline int  calculate_point_key_s = 1;

	}

	inline std::vector<std::string> configs;
}

struct keybind_t {
	bool enabled;
	int id;
	int type;
};

enum keybind_type {
	off,
	hold,
	toggle,
	always
};

struct cvar_t;
struct cvars_t;