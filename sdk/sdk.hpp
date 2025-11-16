#pragma once
#include <windows.h>
#include <d3d9.h>
#include <cstdio>
#include "interfaces/ivcliententitylist.hpp"
#include "interfaces/cglobalvarsbase.hpp"
#include "interfaces/iplayermovement.hpp"
#include "interfaces/imaterialsystem.hpp"
#include "interfaces/ivengineclient.hpp"
#include "interfaces/ibaseclientdll.hpp"
#include "interfaces/ivmodelinfo.hpp"
#include "interfaces/isurface.hpp"
#include "interfaces/i_game_event_manager.hpp"
#include "interfaces/i_filesystem.hpp"
#include "interfaces/chudchat.hpp"
#include "interfaces/i_trace.hpp"
#include "interfaces/iv_render_beams.hpp"
#include "interfaces/panorama.hpp"
#include "interfaces/physics.hpp"
#include "interfaces/ilocalize.hpp"
#include "interfaces/iv_debug_overlay.hpp"
#include "interfaces/iconsole.hpp"
#include "interfaces/imdlcache.hpp"
#include "interfaces/iclientstate.hpp"
#include "interfaces/i_weapon_system.hpp"
#include "interfaces/iv_model_render.hpp"
#include "interfaces/iv_effects.hpp"
#include "interfaces/ipanel.hpp"
#include "structs/usercmd.hpp"
#include "netvars/netvars.hpp"
#include "structs/weaponinfo.hpp"
#include "structs/color.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include "classes/collideable.hpp"
#include "classes/animstate.hpp"
#include "math/math.hpp"
#include "interfaces/glow_manager.hpp"
#include "interfaces/i_input.hpp"
#include "interfaces/i_render_view.hpp"
#include "interfaces/i_steam_api.hpp"
#include "../utils/render/render.hpp"
#include "../utils/render/draw.hpp"
#include "../utils/vfunc.hpp"
#include "../utils/key_values.hpp"
#include "../utils/xor.hpp"
#include "../utils/utilities.hpp"
#include "interfaces/c_physics_collison.h"

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename ret, interface_type type>
	ret* get_interface(const std::string& module_name, const std::string& interface_name) {
		using create_interface_fn = void* (*)(const char*, int*);
		const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

		if (fn) {
			void* result = nullptr;

			switch (type) {
			case interface_type::index:
				result = fn(interface_name.c_str(), nullptr);

				break;
			case interface_type::bruteforce:
				char buf[128];

				for (uint32_t i = 0; i <= 100; ++i) {
					memset(static_cast<void*>(buf), 0, sizeof buf);

					result = fn(interface_name.c_str(), nullptr);

					if (result)
						break;
				}

				break;
			}

			if (!result)
				throw std::runtime_error(interface_name + " wasn't found in " + module_name);

			return static_cast<ret*>(result);
		}

		throw std::runtime_error(module_name + " wasn't found");
	}

	inline i_base_client_dll* client = nullptr;
	inline iv_client_entity_list* ent_list = nullptr;
	inline iv_engine_client* engine = nullptr;
	inline iv_model_info* model_info = nullptr;
	inline i_material_system* material_system = nullptr;
	inline i_client_mode* client_mode = nullptr;
	inline i_surface* surface = nullptr;
	inline i_localize* localize = nullptr;
	inline i_input_system* input_system = nullptr;
	inline c_global_vars_base* globals = nullptr;
	inline i_console* console = nullptr;
	inline i_mdl_cache* mdl_cache = nullptr;
	inline i_client_state* client_state = nullptr;
	inline i_panel* panel = nullptr;
	inline c_hud_chat* chat_element = nullptr;
	inline i_panoramaui_engine* panorama = nullptr;
	inline player_move_helper* move_helper = nullptr;
	inline player_game_movement* game_movement = nullptr;
	inline i_weapon_system* weapon_system;
	inline player_prediction* prediction = nullptr;
	inline trace* trace_ray = nullptr;
	inline IDirect3DDevice9* device = nullptr;
	inline iv_debug_overlay* debug_overlay = nullptr;
	inline iv_model_render* model_render = nullptr;
	inline i_render_view* render_view = nullptr;
	inline i_game_event_manager2* event_manager = nullptr;
	inline i_physics_surface_props* surface_props = nullptr;
	inline i_view_render_beams* render_beams = nullptr;
	inline glow_manager_t* glow_manager = nullptr;
	inline iv_effects* effects = nullptr;
	inline i_input* input = nullptr;
	inline i_filesystem* filesystem;
	inline c_physics_collison* physics_collision = nullptr;
	inline std::uint8_t* key_values_engine = nullptr;
	inline std::uint8_t* key_values_client = nullptr;

	inline i_steam_user* steam_user = nullptr;
	inline i_steam_friends* steam_friend = nullptr;
	inline i_steam_utils* steam_utils = nullptr;

	inline void* engine_sound = nullptr;
	inline void* key_values_system = nullptr;
	inline HWND game_hwnd = nullptr;

	bool initialize();
}

enum data_update_type_t {
	data_update_created = 0,
	data_update_datatable_changed,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	frame_undefined = -1,
	frame_start,
	// a network packet is being recieved
	frame_net_update_start,
	// data has been received and we are going to start calling postdataupdate
	frame_net_update_postdataupdate_start,
	// data has been received and called postdataupdate on all data recipients
	frame_net_update_postdataupdate_end,
	// received all packets, we can now do interpolation, prediction, etc
	frame_net_update_end,
	// start rendering the scene
	frame_render_start,
	// finished rendering the scene
	frame_render_end,
	frame_net_full_frame_update_on_remove
};

enum observer_modes {
	obs_mode_none = 0,
	obs_mode_deathcam,
	obs_mode_freezecam,
	obs_mode_fixed,
	obs_mode_in_eye,
	obs_mode_chase,
	obs_mode_roaming
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};

enum e_effects {
	ef_bone_merge = 0x001,
	ef_bright_light = 0x002,
	ef_dimlight = 0x004,
	ef_no_interp = 0x008,
	ef_no_shadow = 0x010,
	ef_no_draw = 0x020,
	ef_no_receive_shadow = 0x040,
	ef_bone_merge_fastcull = 0x080,
	ef_item_blink = 0x100,
	ef_parent_animates = 0x200,
	ef_marked_for_fast_reflection = 0x400,
	ef_no_shadow_depth = 0x800,
	ef_shadow_depth_no_cache = 0x1000,
	ef_no_flash_light = 0x2000,
	ef_no_csm = 0x4000,
	ef_max_bits = 15
};

enum e_think_methods : int {
	think_fire_all_functions = 0,
	think_fire_base_only,
	think_fire_all_but_base,
};

enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};

enum LifeState : unsigned char
{
	LIFE_ALIVE = 0,// alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2, // dead. lying still.
	MAX_LIFESTATE
};

enum item_definition_indexes {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_TALON,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

// How many bits to use to encode an edict.
#define MAX_EDICT_BITS 11 // # of bits needed to represent max edicts
// Max # of edicts in a level
#define MAX_EDICTS (1 << MAX_EDICT_BITS)

// types of precipitation
enum precipitation_type_t {
	PRECIPITATION_TYPE_RAIN = 0,
	PRECIPITATION_TYPE_SNOW,
	PRECIPITATION_TYPE_ASH,
	PRECIPITATION_TYPE_SNOWFALL,
	PRECIPITATION_TYPE_PARTICLERAIN,
	PRECIPITATION_TYPE_PARTICLEASH,
	PRECIPITATION_TYPE_PARTICLERAINSTORM,
	PRECIPITATION_TYPE_PARTICLESNOW,
	NUM_PRECIPITATION_TYPES
};

struct entity_handle_t {

	uintptr_t handle;

	constexpr entity_handle_t() : handle(0xFFFFFFFF) {
	}

	constexpr entity_handle_t(unsigned long handle) : handle(handle) {
	}

	class entity_t* get() const;

	bool operator==(const entity_handle_t& other) const;
	bool operator==(class entity_t* entity) const;
	bool operator!=(const entity_handle_t& other) const;
	bool operator!=(class entity_t* entity) const;
};

struct renderable_info_t {
	entity_t* renderable;
	std::byte pad[18];
	uint16_t flags;
	uint16_t flags2;
};

class entity_t {
public:

	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}

	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}

	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}

	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[158](this);
	}

	bool is_weapon() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[165](this);
	}

	bool in_buyzone() {
		return *(bool*)((uintptr_t)this + 0x99B5);
	}

	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}



	vec3_t get_absolute_origin() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}

	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}

	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}

	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}

	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)find_pattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}

	int index() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}

	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void pre_data_change(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[4](networkable(), update_type);
	}

	void on_data_changed(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[5](networkable(), update_type);
	}

	void post_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[7](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}

	bool dormant() {
		using original_fn = bool(__thiscall*)(void*);
		return (*static_cast<original_fn**>(networkable()))[9](networkable());
	}

	static __forceinline entity_t* get_entity_by_index(int index) {
		return static_cast<entity_t*>(interfaces::ent_list->get_client_entity(index));
	}

	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long)
	NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float)
	NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t)
	NETVAR("DT_BaseEntity", "m_iTeamNum", team, int)
	NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool)
	NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int)
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float)
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
	NETVAR("DT_PlantedC4", "m_flC4Blow", c4_blow_time, float)
	NETVAR("DT_PlantedC4", "m_flDefuseCountDown", c4_defuse_countdown, float)
	NETVAR("DT_PlantedC4", "m_hBombDefuser", c4_gets_defused, unsigned long)
	NETVAR("DT_PlantedC4", "m_bBombTicking", c4_is_ticking, bool)
	NETVAR("DT_PlantedC4", "m_flTimerLength", c4_timer_length, float)
	NETVAR("DT_PlantedC4", "m_nBombSite", c4_bomb_site, int)
	NETVAR("DT_PlantedC4", "m_flDefuseLength", c4_defuse_length, float)
	NETVAR("DT_PlantedC4", "m_bBombDefused", c4_is_defused, bool)

	OFFSET(std::uint32_t, idex, 0x64);
};

class precipitation_t {
public:
	NETVAR("DT_Precipitation", "m_nPrecipType", precip_type, precipitation_type_t)
	NETVAR("DT_BaseEntity", "m_nModelIndex", get_model_index, int)
	OFFSET(unsigned int, get_index, 0x64)

	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}

	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}


	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void pre_data_change(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[4](networkable(), update_type);
	}

	void on_data_changed(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[5](networkable(), update_type);
	}

	void post_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[7](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}


};

class weapon_t : public entity_t {
public:
	    NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int)
		NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float)
		NETVAR("DT_WeaponCSBase", "m_iIronSightMode", m_iIronSightMode, float)
		NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float)
		NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
		NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int)

	float inaccuracy() {
	    using original_fn = float(__thiscall*)(void*);
	    return (*(original_fn**)this)[482](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[452](this);
	}

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}

	bool is_reloading() {
		static auto inReload = *(uint32_t*)(find_pattern("client.dll", "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90") + 2);
		return *(bool*)((uintptr_t)this + inReload);
	}

	std::string weapon_name_definition() {
		if (!this)
			return ("");
		int id = this->item_definition_index();
		switch (id) {
		case WEAPON_DEAGLE:
			return ("desert eagle");
		case WEAPON_AUG:
			return ("aug");
		case WEAPON_G3SG1:
			return ("g3sg1");
		case WEAPON_MAC10:
			return ("mac10");
		case WEAPON_P90:
			return ("p90");
		case WEAPON_SSG08:
			return ("ssg08");
		case WEAPON_SCAR20:
			return ("scar20");
		case WEAPON_UMP45:
			return ("ump45");
		case WEAPON_ELITE:
			return ("elites");
		case WEAPON_FAMAS:
			return ("famas");
		case WEAPON_FIVESEVEN:
			return ("five seven");
		case WEAPON_GALILAR:
			return ("galil-ar");
		case WEAPON_M4A1_SILENCER:
			return ("m4a1-s");
		case WEAPON_M4A1:
			return ("m4a1");
		case WEAPON_P250:
			return ("p250");
		case WEAPON_M249:
			return ("m249");
		case WEAPON_XM1014:
			return ("xm1014");
		case WEAPON_GLOCK:
			return ("glock 18");
		case WEAPON_USP_SILENCER:
			return ("usp-s");
		case WEAPON_HKP2000:
			return ("p2000");
		case WEAPON_AK47:
			return ("ak-47");
		case WEAPON_AWP:
			return ("awp");
		case WEAPON_BIZON:
			return ("bizon");
		case WEAPON_MAG7:
			return ("mag-7");
		case WEAPON_NEGEV:
			return ("negev");
		case WEAPON_SAWEDOFF:
			return ("sawed off");
		case WEAPON_TEC9:
			return ("tec-9");
		case WEAPON_TASER:
			return ("taser");
		case WEAPON_NOVA:
			return ("nova");
		case WEAPON_CZ75A:
			return ("cz75");
		case WEAPON_SG556:
			return ("sg556");
		case WEAPON_REVOLVER:
			return ("revolver");
		case WEAPON_MP7:
			return ("mp7");
		case WEAPON_MP9:
			return ("mp9");
		case WEAPON_MP5SD:
			return ("mp5sd");
		case WEAPON_C4:
			return ("c4");
		case WEAPON_FRAG_GRENADE:
			return ("he grenade");
		case WEAPON_SMOKEGRENADE:
			return ("smoke grenade");
		case WEAPON_MOLOTOV:
			return ("molotov");
		case WEAPON_INCGRENADE:
			return ("molly");
		case WEAPON_FLASHBANG:
			return ("flashbang");
		case WEAPON_DECOY:
			return ("decoy");
		case WEAPON_KNIFE_T:
			return ("knife");
		case WEAPON_KNIFE:
			return ("knife");
		case WEAPON_TAGRENADE:
			return ("tagrenade");
		case WEAPON_FISTS:
			return ("fist");
		case WEAPON_BREACHCHARGE:
			return ("breach_charge");
		case WEAPON_TABLET:
			return ("tablet");
		case WEAPON_MELEE:
			return ("melee");
		case WEAPON_AXE:
			return ("axe");
		case WEAPON_HAMMER:
			return ("hammer");
		case WEAPON_SPANNER:
			return ("spanner");
		case WEAPON_KNIFE_GHOST:
			return ("knife");
		case WEAPON_FIREBOMB:
			return ("molotov");
		case WEAPON_DIVERSION:
			return ("diversion");
		case WEAPON_SNOWBALL:
			return ("snowball");
		case WEAPON_BUMPMINE:
			return ("bumpmine");
		case WEAPON_BAYONET:
			return ("knife");
		case WEAPON_KNIFE_FLIP:
			return ("knife");
		case WEAPON_KNIFEGG:
			return ("knife");
		case WEAPON_KNIFE_GUT:
			return ("knife");
		case WEAPON_KNIFE_KARAMBIT:
			return ("knife");
		case WEAPON_KNIFE_M9_BAYONET:
			return ("knife");
		case WEAPON_KNIFE_TACTICAL:
			return ("knife");
		case WEAPON_KNIFE_FALCHION:
			return ("knife");
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return ("knife");
		case WEAPON_KNIFE_BUTTERFLY:
			return ("knife");
		case WEAPON_KNIFE_PUSH:
			return ("knife");
		case WEAPON_KNIFE_URSUS:
			return ("knife");
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
			return ("knife");
		case WEAPON_KNIFE_STILETTO:
			return ("knife");
		case WEAPON_KNIFE_WIDOWMAKER:
			return ("knife");
		case WEAPON_HEALTHSHOT:
			return ("healthshot");
		case GLOVE_STUDDED_BLOODHOUND:
			return ("gloves");
		case GLOVE_T_SIDE:
			return ("gloves");
		case GLOVE_CT_SIDE:
			return ("gloves");
		case GLOVE_SPORTY:
			return ("gloves");
		case GLOVE_SLICK:
			return ("gloves");
		case GLOVE_LEATHER_WRAP:
			return ("gloves");
		case GLOVE_MOTORCYCLE:
			return ("gloves");
		case GLOVE_SPECIALIST:
			return ("gloves");
		case GLOVE_HYDRA:
			return ("gloves");
		default:
			return ("");
		}
		return ("");
	}

	std::u8string get_wpn_icon(int index = -1) {
		switch (auto idx = index != -1 ? index : this->item_definition_index(); idx) {
		case WEAPON_DEAGLE:					return u8"\uE001";
		case WEAPON_ELITE:					return u8"\uE002";
		case WEAPON_FIVESEVEN:				return u8"\uE003";
		case WEAPON_GLOCK:					return u8"\uE004";
		case WEAPON_AK47:					return u8"\uE007";
		case WEAPON_AUG:					return u8"\uE008";
		case WEAPON_AWP:					return u8"\uE009";
		case WEAPON_FAMAS:					return u8"\uE00A";
		case WEAPON_G3SG1:					return u8"\uE00B";
		case WEAPON_GALILAR:				return u8"\uE00D";
		case WEAPON_M249:					return u8"\uE00E";
		case WEAPON_M4A1:					return u8"\uE010";
		case WEAPON_MAC10:					return u8"\uE011";
		case WEAPON_P90:					return u8"\uE013";
		case WEAPON_MP5SD:					return u8"\uE017";
		case WEAPON_UMP45:					return u8"\uE018";
		case WEAPON_XM1014:					return u8"\uE019";
		case WEAPON_BIZON:					return u8"\uE01A";
		case WEAPON_MAG7:					return u8"\uE01B";
		case WEAPON_NEGEV:					return u8"\uE01C";
		case WEAPON_SAWEDOFF:				return u8"\uE01D";
		case WEAPON_TEC9:					return u8"\uE01E";
		case WEAPON_TASER:					return u8"\uE01F";
		case WEAPON_HKP2000:				return u8"\uE020";
		case WEAPON_MP7:					return u8"\uE021";
		case WEAPON_MP9:					return u8"\uE022";
		case WEAPON_NOVA:					return u8"\uE023";
		case WEAPON_P250:					return u8"\uE024";
		case WEAPON_SHIELD:					return u8"\uE025";
		case WEAPON_SCAR20:					return u8"\uE026";
		case WEAPON_SSG08:					return u8"\uE028";
		case WEAPON_KNIFEGG:				return u8"\uE029";
		case WEAPON_KNIFE:					return u8"\uE02A";
		case WEAPON_FLASHBANG:				return u8"\uE02B";
		case WEAPON_HEGRENADE:				return u8"\uE02C";
		case WEAPON_SMOKEGRENADE:			return u8"\uE02D";
		case WEAPON_MOLOTOV:				return u8"\uE02E";
		case WEAPON_DECOY:					return u8"\uE02F";
		case WEAPON_INCGRENADE:				return u8"\uE030";
		case WEAPON_C4:						return u8"\uE031";
		case WEAPON_HEALTHSHOT:				return u8"\uE039";
		case WEAPON_KNIFE_T:				return u8"\uE03B";
		case WEAPON_M4A1_SILENCER:			return u8"\uE03C";
		case WEAPON_USP_SILENCER:			return u8"\uE03D";
		case WEAPON_CZ75A:					return u8"\uE03F";
		case WEAPON_REVOLVER:				return u8"\uE040";
		case WEAPON_TAGRENADE:				return u8"\uE044";
		case WEAPON_FISTS:					return u8"\uE045";
		case WEAPON_BREACHCHARGE:			return u8"\uE046";
		case WEAPON_TABLET:					return u8"\uE048";
		case WEAPON_MELEE:					return u8"\uE04A";
		case WEAPON_AXE:					return u8"\uE04B";
		case WEAPON_HAMMER:					return u8"\uE04C";
		case WEAPON_SPANNER:				return u8"\uE04E";
		case WEAPON_KNIFE_GHOST:			return u8"\uE050";
		case WEAPON_FIREBOMB:				return u8"\uE051";
		case WEAPON_DIVERSION:				return u8"\uE052";
		case WEAPON_FRAG_GRENADE:			return u8"\uE053";
		case WEAPON_SNOWBALL:				return u8"\uE054";
		case WEAPON_BUMPMINE:				return u8"\uE055";
		case WEAPON_KNIFE_FLIP:				return u8"\uE1F9";
		case WEAPON_KNIFE_GUT:				return u8"\uE1FA";
		case WEAPON_KNIFE_KARAMBIT:			return u8"\uE1FB";
		case WEAPON_KNIFE_M9_BAYONET:		return u8"\uE1FC";
		case WEAPON_KNIFE_TACTICAL:			return u8"\uE1FD";
		case WEAPON_KNIFE_FALCHION:			return u8"\uE200";
		case WEAPON_KNIFE_SURVIVAL_BOWIE:	return u8"\uE202";
		case WEAPON_KNIFE_BUTTERFLY:		return u8"\uE203";
		case WEAPON_KNIFE_PUSH:				return u8"\uE204";
		case WEAPON_KNIFE_URSUS:			return u8"\uE207";
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:	return u8"\uE208";
		case WEAPON_KNIFE_STILETTO:			return u8"\uE20A";
		case WEAPON_KNIFE_WIDOWMAKER:		return u8"\uE20B";
		default:
			return u8"\u0000";
		}
	}

	bool is_grenade() {
		switch (this->item_definition_index()) {
		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_MOLOTOV:
		case WEAPON_INCGRENADE:
		case WEAPON_DECOY:
			return true;
		default:
			return false;
		}
	}

	bool is_knife()
	{
		if (this->item_definition_index() == WEAPON_TASER) return false;
		return this->weapon_name_definition() == "knife";
	}
};

class player_t : public entity_t {
private:
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	static __forceinline player_t* get_local_player(const bool set = false) {
		static player_t* local = nullptr;

		if (!local || set)
			local = get_player_by_index(interfaces::engine->get_local_player());

		return local;
	}

	static __forceinline player_t* get_player_by_index(int index) {
		return static_cast<player_t*>(get_entity_by_index(index));
	}

	NETVAR("DT_CSPlayer", "m_hRagdoll", ragdoll, int)
	NETVAR("DT_CSPlayer", "m_iHideHUD", m_iHideHUD, int)
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
	NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
	NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
	NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t)
	NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
	NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
	NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
	NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
	NETVAR("DT_CSPlayer", "m_bWaitForNoAttack", wait_for_no_attack, bool)
	NETVAR("DT_CSPlayer", "m_iAccount", money, int)
	NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
	NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
	NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
	NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
	NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
	NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
	NETVAR("DT_CSPlayer", "m_iHealth", health, int)
	NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
	NETVAR("DT_BasePlayer", "m_nNextThinkTick", next_think_tick, int)
	NETVAR("DT_BasePlayer", "m_flFallVelocity", fall_velocity, float)
	NETVAR("DT_BaseAnimating", "m_nSequence", get_sequence, int)
	NETVAR("DT_BaseAnimating", "m_nBody", body, int)
	NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecMins", m_vecMins, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecMaxs", m_vecMaxs, vec3_t)
	NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t)
	NETVAR("DT_BasePlayer", "m_hGroundEntity", g_entity, entity_handle_t)
	NETVAR("DT_CSPlayer", "m_flStamina", stamina, float);
	NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
	NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
	NETVAR("DT_CSPlayer", "m_iFOV", fov, int)
	NETVAR("DT_CSPlayer", "m_iFOVStart", fov_start, int)
	NETVAR("DT_BaseViewModel", "m_hOwner", owner, int)
	NETVAR("DT_BaseViewModel", "m_hWeapon", weapon, int)
	NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
	NETVAR("DT_BasePlayer", "m_iObserverMode", observer_mode, int)
	NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
	NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
	NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
	NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
	NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)
	NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", m_flHealthShotBoostExpirationTime, float)
	NETVAR("CBaseEntity", "m_nModelIndex", model_index, unsigned)
	NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
	NETVAR("DT_PlantedC4", "m_flC4Blow", c4_blow_time, float)
	NETVAR("DT_PlantedC4", "m_flDefuseCountDown", c4_defuse_countdown, float)
	NETVAR("DT_PlantedC4", "m_hBombDefuser", c4_gets_defused, unsigned long)
	NETVAR("DT_PlantedC4", "m_bBombTicking", c4_is_ticking, bool)
	NETVAR("DT_PlantedC4", "m_flTimerLength", c4_timer_length, float)
	NETVAR("DT_PlantedC4", "m_nBombSite", c4_bomb_site, int)
	NETVAR("DT_PlantedC4", "m_flDefuseLength", c4_defuse_length, float)
	NETVAR("DT_PlantedC4", "m_bBombDefused", c4_is_defused, bool)

	OFFSET(bool, should_use_new_anim_state, 0x9B14);
	OFFSET(bool, get_jiggle_bones, 0x2930);
	OFFSET(uint32_t, get_effects, 0xF0);
	OFFSET(CUtlVector<matrix_t>, get_bone_cache, 0x2914);

	datamap_t* pred_datamap() {
		using original_fn = datamap_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[17](this);
	}

	FINDDATAMAP(pred_datamap(), "m_hGroundEntity", ground_entity, int)
	FINDDATAMAP(pred_datamap(), "m_nButtons", buttons, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonLast", button_last, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonPressed", button_pressed, int)
	FINDDATAMAP(pred_datamap(), "m_afButtonReleased", button_released, int)
	FINDDATAMAP(pred_datamap(), "m_nImpulse", impulse, byte)

	weapon_t* active_weapon() {
		auto active_weapon = read<uintptr_t>(netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::ent_list->get_client_entity(active_weapon));
	}

	vec3_t& get_velocity() {
		return *(vec3_t*)((DWORD)this + 0x114);
	}

	player_t* local_or_spectated() {
		auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
		if (!local_player) return nullptr;
		return (local_player->is_alive()) ? local_player : reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity_handle(local_player->observer_target()));
	}

	UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}

	UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}

	vec3_t get_eye_pos() {
		return origin() + view_offset();
	}

	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.initialize(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}


	vec3_t get_hitbox_position(int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					auto min = vec3_t{}, max = vec3_t{};

					math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return (min + max) / 2.0f;
				}
			}
		}
		return vec3_t{};
	}

	int GetBoneByHash(const FNV1A_t uBoneHash) {
		auto studio_model = interfaces::model_info->get_studio_model(model());


		for (int i = 0; i < studio_model->bones_count; i++)
		{
			if (const auto pBone = studio_model->bone(i); pBone != nullptr && FNV1A::Hash(pBone->name()) == uBoneHash)
				return i;
		}

		return -1;
	}

	vec3_t get_hitbox_position(int hitbox_id, matrix_t* matrix) {
		auto studio_model = interfaces::model_info->get_studio_model(model());

		if (studio_model) {
			auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

			if (hitbox) {
				vec3_t min{}, max{};

				math::transform_vector(hitbox->mins, matrix[hitbox->bone], min);
				math::transform_vector(hitbox->maxs, matrix[hitbox->bone], max);

				return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}

		}
		return vec3_t{};
	}

	bool isvisiblebone(int bone) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
		ray_t ray;
		trace_t tr;
		ray.initialize(local_player->get_eye_pos(), this->get_bone_position(bone));

		trace_filter filter;
		filter.skip = local_player;

		interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

		if (tr.entity == this)
			return true;

		return false;
	}

	bool visible() {
		for (int i = 0; i < 8; ++i) {
			if (this->isvisiblebone(i))
				return true;
		}
		return false;
	}

	bool is_alive() {
		if (!this) return false;
		return this->health() > 0;
	}

	bool in_dangerzone() {
		static auto game_type = interfaces::console->get_convar("game_type");
		return game_type->get_int() == 6;
	}

	bool is_enemy() {
		const static auto mp_teammates_are_enemies = interfaces::console->get_convar("mp_teammates_are_enemies");

		if (mp_teammates_are_enemies->get_int())
			return this != get_local_player(true);

		if (in_dangerzone()) {
			return this->survival_team() != get_local_player(true)->survival_team() || get_local_player(true)->survival_team() == -1;
		}
		else {
			return this->team() != get_local_player(true)->team();
		}
	}

	void select_item(const char* str, int sub_type) {
		using original_fn = void(__thiscall*)(void*, const char*, int);
		(*(original_fn**)this)[330](this, str, sub_type);
	}

	const matrix_t& coordinate_frame() {
		static auto coord_frame = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup")) - 0x30;
		return *(matrix_t*)((uintptr_t)this + coord_frame);
	}

	bool is_moving() {
		if (!this) return false;
		return this->velocity().length() > 0.1f;
	}

	bool is_in_air() {
		if (!this) return false;
		return !(this->flags() & fl_onground);
	}


	bool is_not_target() {
		if (!this || this == get_local_player(true))
			return true;

		if (health() <= 0)
			return true;

		if (has_gun_game_immunity())
			return true;

		if (flags() & fl_frozen)
			return true;

		int entIndex = index();
		return entIndex > interfaces::globals->max_clients;
	}

	bool is_flashed() {
		if (!this) return false;
		return this->flash_duration() > 0.0f;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
	}

	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}

	void set_abs_origin(vec3_t origin) {
		using fn = void(__thiscall*)(void*, const vec3_t& origin);
		static fn abs_origin = (fn)find_pattern("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");

		abs_origin(this, origin);
	}

	bool lookup_bone(const char* pos) {
		static const auto lookup_bone_fn = reinterpret_cast<int(__thiscall*)(void*, const char*)>(find_pattern("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"));
		return lookup_bone_fn(this, pos);
	}

	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}

	int move_type() {
		static int type = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_nRenderMode")) + 1;
		return read<int>(type);
	}

	void set_sequence(int sequence) {
		using original_fn = void(__thiscall*)(void*, int);
		(*(original_fn**)this)[219](this, sequence);
	}

	void update_collision_bounds() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[340](this);
	}

	void studio_frame_advance() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[220](this);
	}

	void post_think_v_physics() {
		using original_fn = void(__thiscall*)(void*);
		static auto post_think_v_physics_fn = reinterpret_cast<original_fn>(find_pattern("client.dll", "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB"));
		post_think_v_physics_fn(this);
	}

	void simulate_player_simulated_entities() {
		using original_fn = void(__thiscall*)(void*);
		static auto simulate_player_simulated_entities_fn = reinterpret_cast<original_fn>(find_pattern("client.dll", "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74"));
		simulate_player_simulated_entities_fn(this);
	}

	bool physics_run_think(int think) {
		using original_fn = bool(__thiscall*)(void*, int);
		static auto physics_run_think_fn = reinterpret_cast<original_fn>(find_pattern("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87"));
		return physics_run_think_fn(this, think);
	}

	void think() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[139](this);
	}

	int ButtonDisabled()
	{
		return *(int*)((std::uintptr_t)this + 0x3330);
	}

	int ButtonForced()
	{
		return *(int*)((std::uintptr_t)this + 0x3334);
	}

	c_usercmd** current_command() {
		auto offset = netvar_manager::get_net_var(fnv::hash("CBasePlayer"), fnv::hash("m_hViewEntity")) - 0x4;
		return reinterpret_cast<c_usercmd**>(uintptr_t(this) + offset);
	}

	c_usercmd& last_command() {
		static const std::uintptr_t last_command_fn =
			*reinterpret_cast<std::uintptr_t*>(find_pattern("client.dll", "8D 8E ? ? ? ? 89 5C 24 3C") + 2);
		return *reinterpret_cast<c_usercmd*>(reinterpret_cast<std::uintptr_t>(this) + last_command_fn);
	}

	void set_next_think(int think) {
		using original_fn = void(__thiscall*)(void*, int);
		static auto set_next_think_fn = reinterpret_cast<original_fn>(find_pattern("client.dll", "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B"));
		set_next_think_fn(this, think);
	}

	void pre_think() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[318](this);
	}

	int* weapons() {
		return reinterpret_cast<int*> (uintptr_t(this) + 0x2E08);
	}

	void inv_bone_cache() {
		static DWORD addr = (DWORD)find_pattern("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

		*(int*)((uintptr_t)this + 0xA30) = interfaces::globals->frame_count; //we'll skip occlusion checks now
		*(int*)((uintptr_t)this + 0xA28) = 0;//clear occlusion flags

		unsigned long model_bone_counter = **(unsigned long**)(addr + 10);
		*(unsigned int*)((DWORD)this + 0x2924) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
		*(unsigned int*)((DWORD)this + 0x2690) = (model_bone_counter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
	}

	c_anim_state* get_anim_state() {
		return *static_cast<c_anim_state**>(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(this) + 0x9960));
	}
};

class base_view_model : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int);
	NETVAR("DT_BaseViewModel", "m_hWeapon", m_hweapon, int);
	NETVAR("DT_BaseViewModel", "m_hOwner", m_howner, int);
	NETVAR("DT_BaseViewModel", "m_nBody", m_nBody, int);
};

class attributable_item_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", original_owner_xuid_low, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", original_owner_xuid_high, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fallback_stattrak, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fallback_paint_kit, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fallback_seed, int);
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, int);
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fallback_wear, float);
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", world_model_handle, unsigned long);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short);
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int);
	NETVAR("DT_BaseAttributableItem", "m_iAccountID", acc_id, int);

	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	base_view_model* get_view_model() {
		return (base_view_model*)(DWORD)this;
	}

};

namespace g {
	inline player_t* local = nullptr;
	inline c_usercmd* cmd = nullptr;
}