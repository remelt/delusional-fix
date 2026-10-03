#pragma once
#include "../../sdk/sdk.hpp"

namespace models {
	static constexpr const char* knives[] = {
		"none",
		"models/weapons/v_knife_default_ct.mdl",
		"models/weapons/v_knife_default_t.mdl",
		"models/weapons/v_knife_bayonet.mdl",
		"models/weapons/v_knife_m9_bay.mdl",
		"models/weapons/v_knife_karam.mdl",
		"models/weapons/v_knife_survival_bowie.mdl",
		"models/weapons/v_knife_butterfly.mdl",
		"models/weapons/v_knife_falchion_advanced.mdl",
		"models/weapons/v_knife_flip.mdl",
		"models/weapons/v_knife_gut.mdl",
		"models/weapons/v_knife_tactical.mdl",
		"models/weapons/v_knife_push.mdl",
		"models/weapons/v_knife_gypsy_jackknife.mdl",
		"models/weapons/v_knife_stiletto.mdl",
		"models/weapons/v_knife_widowmaker.mdl",
		"models/weapons/v_knife_ursus.mdl",
		"models/weapons/v_knife_gg.mdl",
		"models/weapons/v_knife_css.mdl",
		"models/weapons/v_knife_outdoor.mdl",
		"models/weapons/v_knife_canis.mdl",
		"models/weapons/v_knife_cord.mdl",
		"models/weapons/v_knife_skeleton.mdl"
	};
    static constexpr const char* gloves[] = {
		"none",
        "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_brokenfang.mdl",
        "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl",
        "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl",
        "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl",
        "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl",
        "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl",
        "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl",
        "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"
    };
	static constexpr const char* agents[] = {
		"none",
		"models/player/custom_player/legacy/ctm_diver_varianta.mdl", // Cmdr. Davida 'Goggles' Fernandez | SEAL Frogman
		"models/player/custom_player/legacy/ctm_diver_variantb.mdl", // Cmdr. Frank 'Wet Sox' Baroud | SEAL Frogman
		"models/player/custom_player/legacy/ctm_diver_variantc.mdl", // Lieutenant Rex Krikey | SEAL Frogman
		"models/player/custom_player/legacy/ctm_fbi_varianth.mdl", // Michael Syfers | FBI Sniper
		"models/player/custom_player/legacy/ctm_fbi_variantf.mdl", // Operator | FBI SWAT
		"models/player/custom_player/legacy/ctm_fbi_variantb.mdl", // Special Agent Ava | FBI
		"models/player/custom_player/legacy/ctm_fbi_variantg.mdl", // Markus Delrow | FBI HRT
		"models/player/custom_player/legacy/ctm_gendarmerie_varianta.mdl", // Sous-Lieutenant Medic | Gendarmerie Nationale
		"models/player/custom_player/legacy/ctm_gendarmerie_variantb.mdl", // Chem-Haz Capitaine | Gendarmerie Nationale
		"models/player/custom_player/legacy/ctm_gendarmerie_variantc.mdl", // Chef d'Escadron Rouchard | Gendarmerie Nationale
		"models/player/custom_player/legacy/ctm_gendarmerie_variantd.mdl", // Aspirant | Gendarmerie Nationale
		"models/player/custom_player/legacy/ctm_gendarmerie_variante.mdl", // Officer Jacques Beltram | Gendarmerie Nationale
		"models/player/custom_player/legacy/ctm_sas_variantg.mdl", // D Squadron Officer | NZSAS
		"models/player/custom_player/legacy/ctm_sas_variantf.mdl", // B Squadron Officer | SAS
		"models/player/custom_player/legacy/ctm_st6_variante.mdl", // Seal Team 6 Soldier | NSWC SEAL
		"models/player/custom_player/legacy/ctm_st6_variantg.mdl", // Buckshot | NSWC SEAL
		"models/player/custom_player/legacy/ctm_st6_varianti.mdl", // Lt. Commander Ricksaw | NSWC SEAL
		"models/player/custom_player/legacy/ctm_st6_variantj.mdl", // 'Blueberries' Buckshot | NSWC SEAL
		"models/player/custom_player/legacy/ctm_st6_variantk.mdl", // 3rd Commando Company | KSK
		"models/player/custom_player/legacy/ctm_st6_variantl.mdl", // 'Two Times' McCoy | TACP Cavalry
		"models/player/custom_player/legacy/ctm_st6_variantm.mdl", // 'Two Times' McCoy | USAF TACP
		"models/player/custom_player/legacy/ctm_st6_variantn.mdl", // Primeiro Tenente | Brazilian 1st Battalion
		"models/player/custom_player/legacy/ctm_swat_variante.mdl", // Cmdr. Mae 'Dead Cold' Jamison | SWAT
		"models/player/custom_player/legacy/ctm_swat_variantf.mdl", // 1st Lieutenant Farlow | SWAT
		"models/player/custom_player/legacy/ctm_swat_variantg.mdl", // John 'Van Healen' Kask | SWAT
		"models/player/custom_player/legacy/ctm_swat_varianth.mdl", // Bio-Haz Specialist | SWAT
		"models/player/custom_player/legacy/ctm_swat_varianti.mdl", // Sergeant Bombson | SWAT
		"models/player/custom_player/legacy/ctm_swat_variantj.mdl", // Chem-Haz Specialist | SWAT
		"models/player/custom_player/legacy/ctm_swat_variantk.mdl", // Lieutenant 'Tree Hugger' Farlow | SWAT
		"models/player/custom_player/legacy/tm_professional_varj.mdl", // Getaway Sally | The Professionals
		"models/player/custom_player/legacy/tm_professional_vari.mdl", // Number K | The Professionals
		"models/player/custom_player/legacy/tm_professional_varh.mdl", // Little Kev | The Professionals
		"models/player/custom_player/legacy/tm_professional_varg.mdl", // Safecracker Voltzmann | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf5.mdl", // Bloody Darryl The Strapped | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf4.mdl", // Sir Bloody Loudmouth Darryl | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf3.mdl", // Sir Bloody Darryl Royale | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf2.mdl", // Sir Bloody Skullhead Darryl | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf1.mdl", // Sir Bloody Silent Darryl | The Professionals
		"models/player/custom_player/legacy/tm_professional_varf.mdl", // Sir Bloody Miami Darryl | The Professionals
		"models/player/custom_player/legacy/tm_phoenix_varianti.mdl", // Street Soldier | Phoenix
		"models/player/custom_player/legacy/tm_phoenix_varianth.mdl", // Soldier | Phoenix
		"models/player/custom_player/legacy/tm_phoenix_variantg.mdl", // Slingshot | Phoenix
		"models/player/custom_player/legacy/tm_phoenix_variantf.mdl", // Enforcer | Phoenix
		"models/player/custom_player/legacy/tm_leet_variantj.mdl", // Mr. Muhlik | Elite Crew
		"models/player/custom_player/legacy/tm_leet_varianti.mdl", // Prof. Shahmat | Elite Crew
		"models/player/custom_player/legacy/tm_leet_varianth.mdl", // Osiris | Elite Crew
		"models/player/custom_player/legacy/tm_leet_variantg.mdl", // Ground Rebel | Elite Crew
		"models/player/custom_player/legacy/tm_leet_variantf.mdl", // The Elite Mr. Muhlik | Elite Crew
		"models/player/custom_player/legacy/tm_jungle_raider_variantf2.mdl", // Trapper | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variantf.mdl", // Trapper Aggressor | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variante.mdl", // Vypa Sista of the Revolution | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variantd.mdl", // Col. Mangos Dabisi | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variant?.mdl", // Arno The Overgrown | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variantb2.mdl", // 'Medium Rare' Crasswater | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_variantb.mdl", // Crasswater The Forgotten | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_jungle_raider_varianta.mdl", // Elite Trapper Solman | Guerrilla Warfare
		"models/player/custom_player/legacy/tm_balkan_varianth.mdl", // 'The Doctor' Romanov | Sabre
		"models/player/custom_player/legacy/tm_balkan_variantj.mdl", // Blackwolf | Sabre
		"models/player/custom_player/legacy/tm_balkan_varianti.mdl", // Maximus | Sabre
		"models/player/custom_player/legacy/tm_balkan_variantf.mdl", // Dragomir | Sabre
		"models/player/custom_player/legacy/tm_balkan_variantg.mdl", // Rezan The Ready | Sabre
		"models/player/custom_player/legacy/tm_balkan_variantk.mdl", // Rezan the Redshirt | Sabre
		"models/player/custom_player/legacy/tm_balkan_variantl.mdl", // Dragomir | Sabre Footsoldier
	};
};

namespace definition_index {
	static const int knives[] = {
		0,
		WEAPON_KNIFE,
		WEAPON_KNIFE_T,
		WEAPON_BAYONET,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_SURVIVAL_BOWIE,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_FALCHION,
		WEAPON_KNIFE_FLIP,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO,
		WEAPON_KNIFE_WIDOWMAKER,
		WEAPON_KNIFE_URSUS,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE_CSS,
		WEAPON_KNIFE_OUTDOOR,
		WEAPON_KNIFE_CANIS,
		WEAPON_KNIFE_CORD,
		WEAPON_KNIFE_SKELETON
	};
	static const int gloves[] = {
		0,
		GLOVE_STUDDED_BROKENFANG,
		GLOVE_STUDDED_BLOODHOUND,
		GLOVE_SPORTY, GLOVE_SLICK,
		GLOVE_LEATHER_WRAP,
		GLOVE_MOTORCYCLE,
		GLOVE_SPECIALIST,
		GLOVE_HYDRA
	};
	static const int weapons[] = {
		WEAPON_USP_SILENCER,
		WEAPON_HKP2000,
		WEAPON_GLOCK,
		WEAPON_P250,
		WEAPON_FIVESEVEN,
		WEAPON_TEC9,
		WEAPON_CZ75A,
		WEAPON_ELITE,
		WEAPON_DEAGLE,
		WEAPON_REVOLVER,
		WEAPON_FAMAS,
		WEAPON_GALILAR,
		WEAPON_M4A1,
		WEAPON_M4A1_SILENCER,
		WEAPON_AK47,
		WEAPON_SG556,
		WEAPON_AUG,
		WEAPON_SSG08,
		WEAPON_AWP,
		WEAPON_SCAR20,
		WEAPON_G3SG1,
		WEAPON_SAWEDOFF,
		WEAPON_M249,
		WEAPON_NEGEV,
		WEAPON_MAG7,
		WEAPON_XM1014,
		WEAPON_NOVA,
		WEAPON_BIZON,
		WEAPON_MP5SD,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_MAC10,
		WEAPON_P90,
		WEAPON_UMP45
	};
}