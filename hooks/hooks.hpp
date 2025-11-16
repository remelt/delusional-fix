#pragma once
#include <d3d9.h>

#include "../sdk/classes/c_view_setup.hpp"
#include "../sdk/classes/view_setup_t.hpp"
#include "../sdk/interfaces/isurface.hpp"
#include "../includes/minhook/minhook.h"
#include "../sdk/interfaces/ivmodelinfo.hpp"
#include "../features/panorama/scaleform.hpp"
#include "../features/aimbot/aimbot.hpp"
#include "../sdk/structs/materials.hpp"
#include "../sdk/structs/usercmd.hpp"
#include "../sdk/math/vec3.hpp"
#include "../utils/vfunc.hpp"
#include "../sdk/sdk.hpp"

#define registers void* ecx, void* edx

namespace sdk {
	namespace hooks {

		inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*reinterpret_cast<int**>(_class))[index]); }

		namespace create_move {
			using fn = bool(__fastcall* )(void*, void*, float, c_usercmd* );
			inline fn ofunc;
			bool __fastcall create_move(registers, float sample_time, c_usercmd* cmd );
		}

		namespace get_color_modulation {
			using fn = void(__fastcall*)(i_material*, void*, float*, float*, float* );
			inline fn ofunc;
			void __fastcall get_color_modulation(i_material* material, void* edx, float* r, float* g, float* b);
		}

		namespace get_alpha_modulation {
			using fn = float(__fastcall*)(i_material*, void*);
			inline fn ofunc;
			float __fastcall get_alpha_modulation(i_material* material, void* edx);
		}

		namespace should_draw_viewmodel {
			using fn = bool(__stdcall*)();
			inline fn ofunc;
			bool __stdcall should_draw_viewmodel();
		}

		namespace get_screen_aspect_ratio {
			using fn = float(__fastcall* )(void*, void*, int, int);
			inline fn ofunc;
			float __fastcall get_screen_aspect_ratio(registers, int width, int height);
		}

		namespace present {
			using fn = long( __stdcall* )( IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA* );
			inline fn ofunc;
			long __stdcall present( IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region );
		}

		namespace override_mouse_input {
			using fn = void( __fastcall* )( void*, int, float*, float* );
			inline fn ofunc;
			void __fastcall override_mouse_input( void* this_, int edx, float* x, float* y );
		}

		namespace push_notice {
			using fn = void(__fastcall*)(int, int, const char*, int, const char*);
			inline fn ofunc;
			void __fastcall push_notice(int ecx, int edx, const char* text, int str_len, const char* null);
		}

		namespace get_unverified_file_hashes {
			using fn = int(__stdcall*)(void*, int);
			inline fn ofunc;
			int __stdcall get_unverified_file_hashes(void* someClass, int maxFiles);
		}

		namespace can_load_third_party_files {
			using fn = int(__fastcall*)(void*, void*);
			inline fn ofunc;
			int __fastcall can_load_third_party_files(void* thisPointer, void* edx);
		}

		namespace emit_sound {
			using fn = void( __fastcall* )( void*, void*, void*, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const vec3_t*, const vec3_t*, vec3_t*, bool, float, int, void* );
			inline fn ofunc;
			void __fastcall emit_sound(registers, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* fds );
		}

		namespace vsnprintf {
			using fn = int( __cdecl* )( char*, int, const char*, ... );
			inline fn ofunc;
			int __cdecl vsnprintf( char* dest, int text_len, const char* fmt, ... );
		}

		namespace frame_stage_notify {
			using fn = void( __thiscall* )( i_base_client_dll*, int );
			inline fn ofunc;
			void __stdcall frame_stage_notify( int frame_stage );
		}

		namespace reset {
			using fn = long( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
			inline fn ofunc;
			long __stdcall reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp );
		}

		namespace lock_cursor {
			using fn = void( __fastcall* )( i_surface* );
			inline fn ofunc;
			void __fastcall lock_cursor( i_surface* _this );
		}

		namespace draw_model_execute {
			using fn = void(__fastcall*)(void*, int, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
			inline fn draw_model_execute_original;
			void __fastcall draw_model_execute(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& pInfo, matrix_t* pCustomBoneToWorld);
		}

		namespace override_view {
			using fn = void* ( __fastcall* )( i_client_mode*, void* _this, view_setup_t* setup );
			inline fn ofunc;
			void __fastcall override_view( void* _this, void* _edx, view_setup_t* setup );
		}

		namespace alloc_key_values_memory {
			using fn = void* (__thiscall*)(void*, const std::int32_t);
			inline fn ofunc;
			void* __stdcall alloc_key_values_memory(const std::int32_t size);
		}

		namespace list_leaves_in_box {
			using fn = int(__thiscall*)(void*, const vec3_t&, const vec3_t&, unsigned short*, int);
			inline fn ofunc;
			int __fastcall list_leaves_in_box(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max);
		}

		namespace set_visuals_data {
			using fn = void(__fastcall*)(void*, void*, const char*);
			inline fn ofunc;
			void __fastcall set_visuals_data(void* ecx, void* edx, const char* shader_name);
		}

		namespace is_connected {
			using fn = bool(__thiscall*)(iv_engine_client*);
			inline fn ofunc;
			bool __stdcall is_connected();
		}

		namespace supports_full_depth {
			using fn = bool(__stdcall*)();
			inline fn ofunc;
			bool __stdcall supports_full_depth();
		}

		namespace is_depth_of_field_enabled {
			using fn = void* (__thiscall*)(void*);
			inline fn ofunc;
			bool __stdcall is_depth_of_field_enabled();
		}

		namespace modify_eye_position {
			using fn = void(__fastcall*)(void*, void*, vec3_t&);
			inline fn ofunc;
			void __fastcall modify_eye_position(void* thisPointer, void* edx, vec3_t& input_eye_pos);
		}

		namespace calcultate_view {
			using fn = void(__fastcall*)(player_t*, void*, vec3_t&, vec3_t&, float&, float&, float&);
			inline fn ofunc;
			void __fastcall calcultate_view(player_t* player, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov);
		}

		namespace level_init_pre_entity {
			using fn = void(__fastcall*)(void*, void*, const char*);
			inline fn ofunc;
			void __fastcall level_init_pre_entity(void* self, void* edx, const char* map);
		}

		namespace level_shutdown {
			using fn = void(__fastcall*)(void*, void*);
			inline fn ofunc;
			void __fastcall level_shutdown(void* self, void* edx);
		}

		namespace set_image_data_r8g8b8a8 {
			using fn = bool(__fastcall*)(const uint8_t*, uint32_t, const char*, int, int, void*, int);
			inline fn ofunc;
			bool __fastcall set_image_data_r8g8b8a8(const uint8_t* data, uint32_t len, const char* filename, int w, int h, void* arg1, int arg);
		}

		namespace on_screen_size_changed {
			using fn = void(__thiscall*)(i_surface*, int, int);
			inline fn ofunc;
			void __stdcall on_screen_size_changed(int old_width, int old_height);
		}

		namespace do_post_screen_effects {
			using fn = bool(__thiscall*)(void*, view_setup_t*);
			inline fn ofunc;
			bool __stdcall do_post_screen_effects(view_setup_t* setup);
		}

		namespace paint_traverse {
			using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
			inline fn ofunc;
			void __stdcall paint_traverse(unsigned int panel, bool force_repaint, bool allow_force);
		}

		namespace draw_set_color {
			using fn = void(__fastcall*)(void*, void*, int, int, int, int);
			inline fn ofunc;
			void __fastcall draw_set_color(void* ecx, void* edx, int r, int g, int b, int a);
		}

		namespace send_datagram {
			using fn = int(__fastcall*)(i_net_channel*, int, bf_write*);
			inline fn ofunc;
			int __fastcall send_datagram(i_net_channel* thisptr, int edx, bf_write* datagram);
		}

		namespace fire_event {
			using fn = bool(__thiscall*)(i_game_event_manager2*, i_game_event*);
			inline fn ofunc;
			bool __stdcall fire_event(i_game_event* gameEvent);
		}

		namespace get_vcollide {
			using fn = void* (__fastcall*)(void*, void*, int);
			inline fn ofunc;
			void* __fastcall get_vcollide(void* ecx, void* edx, int model_index);
		}

		namespace insecure {
			using fn = char(__stdcall*)(void);
			inline fn ofunc;
			char __stdcall insecure_bypass();
		}

        #pragma region sv_pure
		namespace loose_files_allowed {
			using fn = void(__thiscall*)(void*);
			inline fn ofunc;
			bool __stdcall loose_files_allowed();
		}

		namespace check_for_pure_server_white_list {
			using fn = void(__thiscall*)(void*);
			inline fn ofunc;
			void __fastcall check_for_pure_server_white_list(void* edx, void* ecx);
		}
        #pragma endregion

		inline WNDPROC wndproc_original;
		inline HWND window;
		LRESULT __stdcall wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
		void init( );
		void unload( );
	}
}