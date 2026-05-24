#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
#include <chrono>
#include <thread>

#include "sdk/sdk.hpp"
#include "menu/menu.hpp"
#include "features/events/events.hpp"
#include "hooks/hooks.hpp"
#include "menu/config/config.hpp"
#include "features/misc/misc.hpp"
#include "sdk/netvars/netvars.hpp"
#include "features/skins/skins.hpp"
#include "features/movement/movement.hpp"
#include "includes/discord/discord_rpc.h"
#include "features/mplayer/mplayer.h"
#include "features/visuals/visuals.hpp"

#define erase_end __asm _emit 0xEB __asm _emit 0x06 __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC __asm _emit 0xCC
HANDLE player_thread = NULL; // music player thread // saving to close handle l8r

DWORD WINAPI on_attach(void* instance) {
	[&]() {
#ifdef _DEBUG
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        SetConsoleTitleA("delusional fix");
        c::misc::unload_shit = true;
#endif

        interfaces::initialize();
        sdk::hooks::init();
        surface::initialize();
        c::create_directory();
        cvar::init();
        route = std::make_unique<savingroute>("records");

        interfaces::engine->execute_cmd("clear");
        interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[delusional] "));
        interfaces::console->console_printf(std::string("successfully injected").append(" \n").c_str());

        if (interfaces::engine->is_in_game()) {
            interfaces::chat_element->chatprintf("#delusional#_print_injected");
        }
	}();

    while (!(menu::iskeydown(VK_F1) && c::misc::unload_shit && menu::iskeydown(VK_F2)))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

	[&]() {
        if (menu::open)
            menu::open = false;

        if (c::misc::view_model)
            c::misc::view_model = false;

        // isnt right to do it like that
        c::skins::knife_changer_model = 0;
        c::skins::gloves_model = 0;
        c::misc::show_spotify_currently_playing = 0;
        //features::skins::forcing_update = true; // crashes idc
        menu::unhook = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        WaitForSingleObject(player_thread, INFINITE);
        CloseHandle(player_thread);

        events.unload();
        features::skins::animation_unhook();
        sdk::hooks::unload();
        im_render.unload(); // that was the problem with cheat unload // i was too dumb to understand it

        free(mplayer.Thumbnail_buffer);
        if (mplayer.thumb) {
            mplayer.thumb->Release();
            mplayer.thumb = nullptr;
        }

        g_Discord.shutdown();

#ifdef _DEBUG
        fclose(stderr);
        fclose(stdin);
        fclose(stdout);
        FreeConsole();
#endif
	}();

    FreeLibraryAndExitThread(static_cast<HMODULE>(instance), EXIT_SUCCESS); // bye bye T_T
    erase_end;

	return 0;
}

DWORD WINAPI now_playing(void* instance)
{
    while (!menu::unhook)
    {
        if (!c::misc::show_spotify_currently_playing) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        mplayer.Update(interfaces::device);
        Sleep(1000);
    }
    // if being unhooked
    ExitThread(EXIT_SUCCESS);
    printf(xs("exited mplayer thread"));
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD  call_reason, LPVOID reserved) 
{
    if (call_reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const HANDLE thread = CreateThread(nullptr, 0, on_attach, instance, 0, nullptr);
        if (thread) {
            CloseHandle(thread);
        }
        player_thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)now_playing, instance, 0, nullptr); // bad
    }
    else if (call_reason == DLL_PROCESS_DETACH) {
    }
    return TRUE;
}