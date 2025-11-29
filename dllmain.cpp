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

#pragma comment(lib, "dbghelp.lib")

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

//understand how2 do to correctly
LONG WINAPI cheat_crashed_lol(EXCEPTION_POINTERS* pExceptionInfo) {

    //if we close csgo it wont work
    if (pExceptionInfo->ExceptionRecord->ExceptionCode == 0xc0000005) {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    //creating crashes folder dir
    std::string dir = "C:/delusional/crashes";
    if (!std::filesystem::exists(dir))
        std::filesystem::create_directory(dir);

    //gettin time
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* timeinfo = std::localtime(&time_t_now);

    char filename[256];
    strftime(filename, sizeof(filename), "%Y-%m-%d_%H-%M-%S", timeinfo);
    std::string dump_path = dir + "/" + std::string(filename) + ".dmp";
    std::stringstream info;

    STACKFRAME64 frame;
    ZeroMemory(&frame, sizeof(frame));
    CONTEXT* context = pExceptionInfo->ContextRecord;

    //gettin offsets
    frame.AddrPC.Offset = context->Eip;
    frame.AddrFrame.Offset = context->Ebp;
    frame.AddrStack.Offset = context->Esp;

    //https://learn.microsoft.com/ru-ru/windows/win32/api/dbghelp/ns-dbghelp-address
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;

    //https://learn.microsoft.com/ru-ru/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64
    if (StackWalk64(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &frame, context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
    {
        //gettin cpp line info
        IMAGEHLP_LINE64 line;
        DWORD displacement = 0;
        ZeroMemory(&line, sizeof(line));
        line.SizeOfStruct = sizeof(line);

        //final output
        if (SymGetLineFromAddr64(GetCurrentProcess(), frame.AddrPC.Offset, &displacement, &line)) {
            info << line.FileName << ":" << line.LineNumber << std::endl;
        }
    }
    std::string str = info.str();

    //dump
    HANDLE hFile = CreateFileA(dump_path.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pExceptionInfo;
        mdei.ClientPointers = FALSE;

        MINIDUMP_USER_STREAM user_stream;
        user_stream.Type = CommentStreamW;
        user_stream.BufferSize = str.size();
        user_stream.Buffer = (void*)str.c_str();

        MINIDUMP_USER_STREAM_INFORMATION user_stream_info;
        user_stream_info.UserStreamCount = 1;
        user_stream_info.UserStreamArray = &user_stream;

        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithIndirectlyReferencedMemory, &mdei, &user_stream_info, NULL);
        CloseHandle(hFile);
    }

    //msg
    std::string finalmsggg;
    if (str.empty()) {
        finalmsggg = dump_path + "\n" + "send dump here -> dc @opportun1ty.";
    }
    else {
        finalmsggg = str + "\n" + dump_path + "\n" + "send dump here -> dc @opportun1ty";
    }
    int but = MessageBoxA(NULL, finalmsggg.c_str(), "delusional fix", MB_ICONERROR | MB_YESNO);
    switch (but) {
    case 6:
        system("start https://discordapp.com/users/683305078815588372");
        break;
    }

    //fuck u
    return EXCEPTION_EXECUTE_HANDLER;
}

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

    while (!(menu::iskeydown(VK_F1) && c::misc::unload_shit && menu::iskeydown(VK_F2)))
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

        g_Discord.shutdown();
	}();

    //works only w manual map for some reason
    FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 1);

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
        //debug purposes only
        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
        SetUnhandledExceptionFilter(cheat_crashed_lol);

        DisableThreadLibraryCalls(instance);

        const HANDLE thread = CreateThread(nullptr, 0, on_attach, instance, 0, nullptr);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)GetNowPlayingInfoAndSaveAlbumArt, instance, 0, nullptr);
        if (thread) {

            CloseHandle(thread);
        }
    }
    else if (call_reason == DLL_PROCESS_DETACH) {
        g_Discord.shutdown();
    }
    return TRUE;
}