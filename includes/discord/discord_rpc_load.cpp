#include "discord_rpc.h"
#include "../../menu/config/config.hpp"
#include "../../hooks/hooks.hpp"

void c_discord::initialize() {
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));

    Discord_Initialize("1418951638756098050", &Handle, 1, NULL);
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
    //discordPresence.largeImageKey = "";
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
