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

    std::string current_status = { "js chillin" };
    if (interfaces::engine->is_connected()) {
		// get hostname from server
        if (interfaces::engine->get_level_name() && interfaces::engine->get_level_name() != " ") {
            current_status = "playing on: ";
            current_status += interfaces::engine->get_level_name();
        }
    }

    discordPresence.largeImageText = "T _T";
    discordPresence.state = current_status.c_str();
    //https://github.com/remelt/delusional-fix/blob/main/includes/discord/discord_rpc_image.png?raw=true
    discordPresence.largeImageKey = "https://images2.imgbox.com/57/b1/7hsWTykc_o.png";
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

c_discord g_Discord;
