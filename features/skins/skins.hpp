#pragma once
#include "../../sdk/sdk.hpp"
#include <stdio.h>

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct node_t {
	int previous_id;
	int next_id;
	void* _unknown_ptr;
	int _unknown;
	Key key;
	Value value;
};

template <typename key, typename value>
struct head_t {
	node_t<key, value>* memory;
	int allocation_count;
	int grow_size;
	int start_element;
	int next_available;
	int _unknown;
	int last_element;
};

struct string_t {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

struct paint_kit_t {
	int id;						//0x0000
	string_t name;				//0x0004
	string_t description;		//0x0014
	string_t item_name;			//0x0024
	string_t material_name;		//0x0034
	string_t image_inventory;	//0x0044
	string_t pad1;
	string_t sLogoMaterial;
	int bBaseDiffuseOverride;
	int rarity;
	int nStyle;
	int color1;
	int color2;
	int color3;
	int color4;
	int logoColor1;
	int logoColor2;
	int logoColor3;
	int logoColor4;
	float flWearDefault;
	float flWearRemapMin;
	float flWearRemapMax;
	char nFixedSeed;
	char uchPhongExponent;
	char uchPhongAlbedoBoost;
	char uchPhongIntensity;
	float flPatternScale;
	float flPatternOffsetXStart;
	float flPatternOffsetXEnd;
	float flPatternOffsetYStart;
	float flPatternOffsetYEnd;
	float flPatternRotateStart;
	float flPatternRotateEnd;
	float flLogoScale;
	float flLogoOffsetX;
	float flLogoOffsetY;
	float flLogoRotation;
	int bIgnoreWeaponSizeScale;
	int nViewModelExponentOverrideSize;
	int bOnlyFirstMaterial;
	float pearlescent;
	int sVmtPath[4];
	int kvVmtOverrides;
};

class wpn_visual_data {
public:
	char padding1[0x77C];
	char weapon_path[260];
	char padding2[0x10C];

	float color1_r;
	float color1_g;
	float color1_b;
	float color2_r;
	float color2_g;
	float color2_b;
	float color3_r;
	float color3_g;
	float color3_b;
	float color4_r;
	float color4_g;
	float color4_b;

	int nPhongAlbedoBoost;
	int nPhongExponent;
	int nPhongIntensity;

	float flPhongAlbedoFactor;

	float flWearProgress;

	float flPatternScale;
	float flPatternOffsetX;
	float flPatternOffsetY;
	float flPatternRot;

	float flWearScale;
	float flWearOffsetX;
	float flWearOffsetY;
	float flWearRot;

	float flGrungeScale;
	float flGrungeOffsetX;
	float flGrungeOffsetY;
	float flGrungeRot;
};

struct CStickerKit
{
	int idd;
	int item_rarity;
	string_t name;
	string_t description;
	string_t item_name;
	string_t material_name;
	string_t image_inventory;
	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;
	float rotate_end;
	float rotate_start;
	float scale_min;
	float scale_max;
	float wear_min;	float wear_max;
	string_t image_inventory2;
	string_t image_inventory_large;
	std::uint32_t pad0[4];
};


namespace features::skins {
	struct hud_weapons_t {
		std::int32_t* wpn_count( ) {
			return reinterpret_cast< std::int32_t* >( std::uintptr_t( this ) + 0x80 );
		}
	};

	void agent_changer( );
	void gloves_changer();
	void knife_changer( );
	void init_knife_proxy( );
	void init_parser( );
	void full_update();
	inline bool forcing_update = false;

	struct paint_kit {
		paint_kit(int i, std::string n, paint_kit_t* l) {
			id = i;
			name = n;
			link = l;
		}
		int id;
		std::string name;
		paint_kit_t* link;
		bool operator < ( const paint_kit& other ) {
			return name < other.name;
		}
	};

	inline std::vector<paint_kit> parser_skins;
	inline std::vector<paint_kit> parser_gloves;

	inline bool hud_update_req =  false;
	inline int(__thiscall* clear_wpn_hud)(int*, int);
}
