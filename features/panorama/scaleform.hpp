#pragma once
#include "../../sdk/sdk.hpp"
#include "icons.hpp"
#include <cstdint>

#define JAVASCRIPT constexpr const char *
#define HEX_COLOR constexpr const char *
#define CSGO_HUD_SCHEMA "panorama/layout/hud/hud.xml"
#define SCALEFORM_JS_STATE const char *
#define UPDATING_VAR(old, goal_name, goal, on_update) \
{                                                 \
decltype(old) goal_name = goal;               \
if (old != goal_name)                         \
{                                             \
on_update;                                \
old = goal_name;                          \
}                                             \
}         

 struct c_ui_panel;
 struct player_t;
 struct i_game_event;
 struct convar;
struct scaleform_t
{
    c_ui_panel* root;
    c_ui_panel* weap_sel;
    c_ui_panel* weap_pan_bg;
    bool inited;
    int old_color;
    float old_alpha;
    int old_healthammo_style;
    int old_in_buyzone;
    bool pending_mvp;   
    int old_weap_rows_count;    

} inline sfui;

namespace panorama {
    void scaleform_init();
    void scaleform_modify_all();
    void scaleform_tick(player_t*);
    void scaleform_on_event(i_game_event*);
    void scaleform_after_event(const char*);
    bool scaleform_get_replacement_icon(const char*, const uint8_t*&, size_t&, int&, int&);

    // special event
    void scaleform_on_death();
    void scaleform_on_weapon_event();
    void scaleform_dump_icons(const uint8_t*, size_t, const char*);
}

struct context_t {

    struct functions_t
    {
        char* (CDECL* compare_extension)(const char* lhs, const char* rhs);
    } f;
    struct cvars_t
    {
        convar* cl_hud_color;
        convar* cl_hud_background_alpha;
        convar* cl_hud_healthammo_style;
    } c;
 
} inline ctx;