#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <chrono>
#include <thread>

#include "sdk/sdk.hpp"
//#include "utils/xor.hpp"
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

#ifdef _DEBUG
bool unload( HMODULE module ) {
    while (!GetAsyncKeyState( VK_END )) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 20 ) );
    }

    if (menu::open) 
        menu::open = false;

    if (c::misc::view_model) 
        c::misc::view_model = false;
   
    c::skins::knife_changer_model = 0;

    features::skins::full_update();

    printf( ( "delusional | unloading hooks\n" ) );
    sdk::hooks::unload( );
    printf( ( "delusional | unloaded hooks\n" ) );

    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    printf( ( "delusional | unloading\n" ) );

    std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );
    fclose( stdin );
    fclose( stdout );
    FreeConsole( );

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    FreeLibraryAndExitThread( module, 1 );
    return false;
}
#endif

DWORD WINAPI on_attach(void* instance) {
	[&]() {
        interfaces::initialize();
        sdk::hooks::init();
        surface::initialize();
        c::create_directory();
        cvar::init();
        route = std::make_unique<savingroute>("records");

        mplayer.sessionManager.reset();
        mplayer.session.reset();
        mplayer.thumbnail.reset();

        interfaces::engine->execute_cmd("clear");
        interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[delusional] "));
        interfaces::console->console_printf(std::string("successfully injected").append(" \n").c_str());

        if (interfaces::engine->is_in_game()) {
            interfaces::chat_element->chatprintf("#delusional#_print_injected");
        }
	}();

    while (!GetAsyncKeyState(VK_F1) || !GetAsyncKeyState(VK_F2))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

	[&]() {
        if (menu::open)
            menu::open = false;

        if (c::misc::view_model)
            c::misc::view_model = false;

        c::skins::knife_changer_model = 0;

        features::skins::full_update();

        events.unload();
        sdk::hooks::unload();
        im_render.unload();

	}();

    //works only w manual map for some reason
    FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);

	return 0;
}

//MUSIC PLAYER FROM LB

void GetNowPlayingInfoAndSaveAlbumArt(void* instance)
{
    while (true) {

        if (!c::misc::show_spotify_currently_playing) {
            Sleep(10000);
            continue;
        }
        mplayer.Update(interfaces::device);
        if (oldtitle != strtitle || oldartist != strartist || !albumArtTexture) {
            albumArtTexture = mplayer.thumb;
            oldtitle = strtitle;
            oldartist = strartist;
        }
        Sleep(1);
        strartist = mplayer.Artist;
        Sleep(1);
        strtitle = mplayer.Title;
        Sleep(1);
        albumArtTexture = mplayer.thumb;
        Sleep(1000);
    }
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD  call_reason, LPVOID reserved) 
{
    if (call_reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(instance);

        const HANDLE thread = CreateThread(nullptr, 0, on_attach, instance, 0, nullptr);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)GetNowPlayingInfoAndSaveAlbumArt, instance, 0, nullptr);
        if (thread) {

            CloseHandle(thread);
        }
    }
    else if (call_reason == DLL_PROCESS_DETACH) {
    }
    return TRUE;
}