#include "discord_rpc.h"
#include "../../menu/config/config.hpp"
#include "../../hooks/hooks.hpp"

void c_discord::initialize() {
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));

    Discord_Initialize("885283795044016130", &Handle, 1, NULL);
}

void c_discord::update() {
    static bool init = false;
    if (c::misc::discord_rpc) {
    if (!init){
        initialize();
    init = true;
}    
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    static auto elapsed = std::time ( nullptr );

    std::string current_status = { "In main menu" };
    if (interfaces::engine->is_connected()) {
		// get hostname from server
        current_status = "In game - ";
		current_status += interfaces::engine->get_level_name();
    }

    discordPresence.largeImageText = "777";
    discordPresence.state = current_status.c_str();
    discordPresence.largeImageKey = "https://raw.githubusercontent.com/ANANAIN3666/JPELIROJOXD/main/Jpelirrojo-396x595.jpg";
    discordPresence.startTimestamp = elapsed;
    Discord_UpdatePresence(&discordPresence);
    } else {
        shutdown();
        init = false;
    }
}

void c_discord::shutdown() {
    Discord_ClearPresence();
    Discord_Shutdown();
}
