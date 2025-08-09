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
    //lua::unload();

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
       // lua::init_state();
        interfaces::initialize();
        sdk::hooks::init();
        surface::initialize();
        c::create_directory();
        //lua::init_command();
        cvar::init();
        route = std::make_unique<savingroute>("records");

        interfaces::engine->execute_cmd("clear");
        interfaces::console->console_color_printf({ 255, 0, 0, 255 }, ("[delusional] "));
        interfaces::console->console_printf(std::string("successfully injected").append(" \n").c_str());
	}();

    while (!GetAsyncKeyState(VK_END) && !GetAsyncKeyState(VK_DELETE))
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

	[&]() {

        events.unload();
        sdk::hooks::unload();
        im_render.unload();
       // lua::unload();

	}();

	(FreeLibraryAndExitThread)(static_cast<HMODULE>(instance), 0);

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
    }
    return TRUE;
}