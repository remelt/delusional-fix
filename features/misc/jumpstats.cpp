#include "../../features/movement/movement.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../misc/misc.hpp"
#include <iostream>
#include <cmath>

class jumpstats_calculations {
private:
    static const auto white = '\x08';
    static const auto violet = '\x0B';
    static const auto green = '\x06';
    static const auto red = '\x07';
    static const auto golden = '\x10';
public:
    void resetStats() {
        units = 0.0f;
        strafes = 0;
        pre = 0.0f;
        max_vel = 0.0f;
        max_height = 0.0f;
        jumps = 0;
        bhops = 0;
        sync = 0.0f;
        start_pos = vec3_t{};
        landing_pos = vec3_t{};
    }

    bool show() {
        if (!onground || jumping || jumpbugged)
            return false;

        if (!should_show)
            return false;

        units = (start_pos - landing_pos).length_2d() + (is_ladderjump ? 0.0f : 32.0f);

        const float z = fabsf(start_pos.z - landing_pos.z) - (is_jb ? 9.0f : 0.0f);
        const bool fail = z >= (is_ladderjump ? 32.0f : (jumps > 0 ? (jumps > 1 ? 46.0f : 2.0f) : 46.0f));
        const bool simplifyNames = true;

        std::string jump = "null";

        auto color = white;
        switch (jumps) {
        case 1:
            if (!is_jb) {
                jump = "lj";
                if (units >= 245.f)
                    color = '\x03';
                else if (units >= 243.f)
                    color = '\x10';
                else if (units >= 240.f)
                    color = '\x02';
                else if (units >= 235.f)
                    color = '\x06';
                else if (units >= 230.f)
                    color = '\x0B';
                else
                    color = '\x08';
            }
            else {
                jump = "jb";
                if (units < 250.0f)
                    color = white;
                else if (units >= 250.0f && units < 260.0f)
                    color = violet;
                else if (units >= 260.0f && units < 265.0f)
                    color = green;
                else if (units >= 265.0f && units < 270.0f)
                    color = red;
                else if (units >= 270.0f)
                    color = golden;
            }
            break;
        case 2:
            jump = "bh";
            if (units < 230.0f)
                color = white;
            else if (units >= 230.0f && units < 233.0f)
                color = violet;
            else if (units >= 233.0f && units < 235.0f)
                color = green;
            else if (units >= 235.0f && units < 240.0f)
                color = red;
            else if (units >= 240.0f)
                color = golden;
            break;
        default:
            if (jumps >= 3) {
                jump = "mbh";
                if (units < 230.0f)
                    color = white;
                else if (units >= 230.0f && units < 233.0f)
                    color = violet;
                else if (units >= 233.0f && units < 235.0f)
                    color = green;
                else if (units >= 235.0f && units < 240.0f)
                    color = red;
                else if (units >= 240.0f)
                    color = golden;
            }
            break;
        }

        if (is_ladderjump) {
            jump = "laj";
            if (units < 80.0f)
                color = white;
            else if (units >= 80.0f && units < 90.0f)
                color = violet;
            else if (units >= 90.0f && units < 105.0f)
                color = green;
            else if (units >= 105.0f && units < 109.0f)
                color = red;
            else if (units >= 109.0f)
                color = golden;
        }

        if (!c::misc::jumpstats_show_clr_fail && fail)
            color = white;

        if (fail)
            jump += "-f";

        //TODO: FIX CHATPRINT
        
        const bool show = (is_ladderjump ? units >= 50.0f : units >= 186.0f) && (!(!c::misc::jumpstats_show_fail && fail) || (c::misc::jumpstats_show_fail));
        if (show && c::misc::jumpstats) {
            if (jumps > 2) {
                /*interfaces::chat_element->chatprintf(" \x08 %c%s: %.2f \x08units | \x08 %c%d\x08 strafes | %c%.0f\x08 pre | %c%.0f\x08 max | %c%.1f\x08 height | %c%d\x08 bhops | %c%.0f\x08 sync",
                    color, jump.c_str(),
                    jump_stats_cals.units, color, jump_stats_cals.strafes, color, jump_stats_cals.pre, color, jump_stats_cals.max_vel, color, jump_stats_cals.max_height, color, jump_stats_cals.jumps, color, jump_stats_cals.sync);*/

                interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[delusional] "));
                interfaces::console->console_printf("%s: %.2f units | %d strafes | %.0f pre | %.0f max | %.1f height | %d bhops | %.0f sync \n", 
                    jump.c_str(), jump_stats_cals.units, jump_stats_cals.strafes, jump_stats_cals.pre, jump_stats_cals.max_vel, jump_stats_cals.max_height, jump_stats_cals.jumps, jump_stats_cals.sync);

            }                
            else {
                /*interfaces::chat_element->chatprintf(
                    " \x08 %c%s: %.2f \x08units | \x08 %c%d\x08 strafes | %c%.0f\x08 pre | %c%.0f\x08 max | %c%.1f\x08 height | %c%.0f\x08 sync",
                    color, jump.c_str(),
                    jump_stats_cals.units, color, jump_stats_cals.strafes, color, jump_stats_cals.pre, color, jump_stats_cals.max_vel, color, jump_stats_cals.max_height, color, jump_stats_cals.sync);*/

                interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[delusional] "));
                interfaces::console->console_printf("%s: %.2f units | %d strafes | %.0f pre | %.0f max | %.1f height | %.0f sync \n", 
                    jump.c_str(), jump_stats_cals.units, jump_stats_cals.strafes, jump_stats_cals.pre, jump_stats_cals.max_vel, jump_stats_cals.max_height, jump_stats_cals.sync);
            }
                
        }

        should_show = false;
        return true;
    }

    void run(c_usercmd* cmd)  {
        if (g::local->move_type() == movetype_noclip) {
            resetStats();
            should_show = false;
            return;
        }

        velocity = g::local->velocity().length_2d();
        origin = g::local->abs_origin();
        onground = g::local->flags() & 1;
        onladder = g::local->move_type() == movetype_ladder;
        jumping = cmd->buttons & in_jump && !(lastbuttons & in_jump) && onground;
        jumpbugged = !jumpped && has_jumped;;

        //we jumped so we should show this jump
        if (jumping || jumpbugged)
            should_show = true;

        if (onladder) {
            start_pos = origin;
            pre = velocity;
            startedOnLadder = true;
        }

        if (onground) {
            if (!onladder) {
                if (jumping) {
                    //we save pre velocity and the starting position
                    start_pos = origin;
                    pre = velocity;
                    jumps++;
                    startedOnLadder = false;
                    is_ladderjump = false;
                }
                else {
                    landing_pos = origin;
                    //we reset our jumps after logging them, and incase we do log our jumps and need to reset anyways we do this
                    if (!should_show)
                        jumps = 0;

                    if (startedOnLadder) {
                        is_ladderjump = true;
                        should_show = true;
                    }
                    startedOnLadder = false;
                }
            }

            //Calculate sync
            if (ticksInAir > 0 && !jumping)
                sync = (static_cast<float>(ticksSynced) / static_cast<float>(ticksInAir)) * 100.0f;

            //Reset both counters used for calculating sync
            ticksInAir = 0;
            ticksSynced = 0;
        }
        else if (!onground && !onladder) {
            if (jumpbugged) {
                if (old_origin.not_null())
                    start_pos = old_origin;
                pre = old_vel;
                jumps = 1;
                is_jb = true;
                jumpbugged = false;
            }
            //check for strafes
            if (cmd->mouse_dx != 0 && cmd->side_move != 0.0f) {
                if (cmd->mouse_dx > 0 && lastmouse_dx <= 0.0f && cmd->side_move > 0.0f) {
                    strafes++;
                }
                if (cmd->mouse_dx < 0 && lastmouse_dx >= 0.0f && cmd->side_move < 0.0f) {
                    strafes++;
                }
            }

            //if we gain velocity, we gain more sync
            if (old_vel != 0.0f) {
                float deltaSpeed = velocity - old_vel;
                bool gained = deltaSpeed > 0.000001f;
                bool lost = deltaSpeed < -0.000001f;
                if (gained)
                {
                    ticksSynced++;
                }
            }

            //get max height and max velocity
            max_height = max(fabsf(start_pos.z - origin.z), max_height);
            max_vel = max(velocity, max_vel);

            ticksInAir++; //we are in air
            sync = 0; //we dont calculate sync yet
        }

        lastmouse_dx = cmd->mouse_dx;
        lastonground = onground;
        lastbuttons = cmd->buttons;
        old_vel = velocity;
        old_origin = origin;
        jumpped = jumping;
        has_jumped = false;

        if (show())
            resetStats();

        if (onground && !onladder) {
            is_jb = false;
        }
        is_ladderjump = false;
    }

    //last values
    short lastmouse_dx = 0;
    bool lastonground = false;
    int lastbuttons = 0;
    float old_vel = 0.0f;
    bool jumpped = false ;
    vec3_t old_origin;
    vec3_t start_pos;

    //current values
    float velocity = 0.0f;
    bool onladder = false;
    bool onground = false;
    bool jumping = false;
    bool jumpbugged = false;
    bool is_jb = false;
    bool has_jumped = false;
    bool startedOnLadder = false;
    bool is_ladderjump = false;
    bool should_show = false;
    int jumps = 0;
    vec3_t origin;
    vec3_t landing_pos;
    int ticksInAir = 0;
    int ticksSynced = 0;

    //final values
    float units = 0.0f;
    int strafes = 0;
    float pre = 0.0f;
    float max_vel = 0.0f;
    float max_height = 0.0f;
    int bhops = 0;
    float sync = 0.0f;
} jump_stats_cals;

void features::misc::jumpstats::gotjump() {
    jump_stats_cals.has_jumped = true;
}

void features::misc::jumpstats::jumpstats(c_usercmd* cmd) {
    if (!g::local)
        return;

    if (!g::local->is_alive()) {
        jump_stats_cals = {};
        return;
    }

    static bool once = true;
    if (g::local->flags() & (fl_atcontrols)) {
        if (once) {
            jump_stats_cals = {};
            once = false;
        }
        return;
    }

    jump_stats_cals.run(cmd);

    once = true;
}

void features::misc::jumpstats::resetjumpstats() {
    jump_stats_cals = jumpstats_calculations{};
}