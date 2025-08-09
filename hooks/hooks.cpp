#include "hooks.hpp"
#include "../features/skins/skins.hpp"
//#include "../features/events/events.hpp"
#include "../menu/config/config.hpp"

void sdk::hooks::init( ) {
	if (MH_Initialize( ) != MH_OK)
		printf( ( "failed to initialize hooking system.\n" ) );

	if (MH_CreateHook(get_vfunc<void*>( interfaces::device, 17 ), &present::present, ( void** ) &present::ofunc ))
		printf(( "present hook failed.\n" ));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::client_mode, 24 ), &create_move::create_move, ( void** ) &create_move::ofunc ))
		printf(( "create move hook failed.\n" ));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::client_mode, 27), &should_draw_viewmodel::should_draw_viewmodel, (void**)&should_draw_viewmodel::ofunc))
		printf(("should draw viewmodel hook failed.\n"));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::engine, 101 ), &get_screen_aspect_ratio::get_screen_aspect_ratio, ( void** ) &get_screen_aspect_ratio::ofunc ))
		printf(( "get screen aspect ratio hook failed.\n" ));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::panel, 41), &paint_traverse::paint_traverse, (void**)&paint_traverse::ofunc))
		printf(("override view hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::surface, 116), &on_screen_size_changed::on_screen_size_changed, (void**)&on_screen_size_changed::ofunc))
		printf(("on_screen_size_changed  hook failed.\n"));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::client_mode, 18 ), &override_view::override_view, ( void** ) &override_view::ofunc ))
		printf(( "override view hook failed.\n" ));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::client_mode, 23 ), &override_mouse_input::override_mouse_input, ( void** ) &override_mouse_input::ofunc ))
		printf(("override mouse input hook failed.\n" ));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::client, 37 ), &frame_stage_notify::frame_stage_notify, ( void** ) &frame_stage_notify::ofunc ))
		printf(("frame stage notify hook failed.\n" ));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::client_mode, 44), &do_post_screen_effects::do_post_screen_effects, (void**)&do_post_screen_effects::ofunc))
		printf(("override view hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::model_render, 21), &draw_model_execute::draw_model_execute, (void**)&draw_model_execute::draw_model_execute_original))
		printf(("draw model execute  hook failed.\n"));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::engine_sound, 5 ), &emit_sound::emit_sound, ( void** ) &emit_sound::ofunc ))
		printf(("emit sound hook failed.\n" ));

	if (MH_CreateHook( get_vfunc<void*>( interfaces::surface, 67 ), &lock_cursor::lock_cursor, ( void** ) &lock_cursor::ofunc ))
		printf( ("lock cursor hook failed."));

	//if (MH_CreateHook(get_vfunc<void*>(interfaces::surface, 15), &draw_set_color::draw_set_color, (void**)&draw_set_color::ofunc))
		//printf(("draw set color  hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::client, 7), &level_shutdown::level_shutdown, (void**)&level_shutdown::ofunc))
		printf(("level shutdown hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::game_movement, 32), &on_jump::on_jump, (void**)&on_jump::ofunc))
		printf(("on jump hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::client, 5), &level_init_pre_entity::level_init_pre_entity, (void**)&level_init_pre_entity::ofunc))
		printf(("level init pre entity hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::key_values_system, 2), &alloc_key_values_memory::alloc_key_values_memory, (void**)&alloc_key_values_memory::ofunc))
		printf(("key values system  hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::filesystem, 128), &loose_files_allowed::loose_files_allowed, (void**)&loose_files_allowed::ofunc))
		printf(("loose files allowed hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::engine->get_bsp_query(), 6), &list_leaves_in_box::list_leaves_in_box, (void**)&list_leaves_in_box::ofunc))
		printf(("list leaves in box hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::engine, 27), &is_connected::is_connected, (void**)&is_connected::ofunc))
		printf(("is connected hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60")), &modify_eye_position::modify_eye_position, (void**)&modify_eye_position::ofunc))
		printf(("modify eye position hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("55 8B EC 83 EC 14 53 56 57 FF 75 18")), &calcultate_view::calcultate_view, (void**)&calcultate_view::ofunc))
		printf(("calculate view hook failed.\n"));

	if (MH_CreateHook(find_pattern(("engine.dll"), ("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 89 7C 24 14")), &send_datagram::send_datagram, (void**)&send_datagram::ofunc))
		printf(("send datagram hook failed.\n"));

	if (MH_CreateHook(find_pattern(("engine.dll"), ("8B 0D ? ? ? ? 56 83 B9 ? ? ? ? ? 7E 6E")), &check_for_pure_server_white_list::check_for_pure_server_white_list, (void**)&check_for_pure_server_white_list::ofunc))
		printf(("check for pure server white list hook failed.\n"));

	if (MH_CreateHook(find_pattern(("materialsystem.dll"), ("55 8B EC 83 EC ? 56 8B F1 8A 46")), &get_color_modulation::get_color_modulation, (void**)&get_color_modulation::ofunc))
		printf(("get color modulation hook failed.\n"));

	if (MH_CreateHook(find_pattern(("materialsystem.dll"), ("56 8B F1 8A 46 20 C0 E8 02 A8 01 75 0B 6A 00 6A 00 6A 00 E8 ? ? ? ? 80 7E 22 05 76 0E")), &get_alpha_modulation::get_alpha_modulation, (void**)&get_alpha_modulation::ofunc))
		printf(("get alpha modulation hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("55 8B EC 51 56 8B 75 0C 8D 45 14 57 8B 7D 08 8B D6 50 51 FF 75 10 8B CF E8 ? ? ? ? 83 C4 0C 85 C0 78 08 85 F6 7E 0C 3B C6 7C 08 8D 46 FF")), &vsnprintf::vsnprintf, (void**)&vsnprintf::ofunc))
		printf(("vsnprintf hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("8B 0D ? ? ? ? 56 8B 01 FF 50 ? 8B F0 85 F6 75 ?")), &is_depth_of_field_enabled::is_depth_of_field_enabled, (void**)&is_depth_of_field_enabled::ofunc))
		printf(("is depth of field enabled hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8B 53 5C")), &set_visuals_data::set_visuals_data, (void**)&set_visuals_data::ofunc))
		printf(("set visuals data hook failed.\n"));

	if (MH_CreateHook(find_pattern(("client.dll"), ("55 8B EC 83 E4 F8 B8 ? ? ? ? E8 ? ? ? ? 53 8B D9 8B 0D")), &push_notice::push_notice, (void**)&push_notice::ofunc))
		printf(("push notice hook failed.\n"));

	if (MH_CreateHook(find_pattern(("shaderapidx9.dll"), ("A1 ? ? ? ? A8 ? 75 ? 83 C8 ? B9 ? ? ? ? 68 ? ? ? ? A3")), &supports_full_depth::supports_full_depth, (void**)&supports_full_depth::ofunc))
		printf(("supports full depth hook failed.\n"));

	if (MH_CreateHook(get_vfunc<void*>(interfaces::event_manager, 9), &fire_event::fire_event, (void**)&fire_event::ofunc))
		printf(("is connected hook failed.\n"));

	if (MH_EnableHook( MH_ALL_HOOKS ) != MH_OK)
		printf(( "failed to initialize hooks.\n" ));

	window = FindWindowW( ( L"Valve001" ), NULL );

	features::skins::init_parser( );
	features::skins::init_knife_proxy( );
	panorama::scaleform_init();
	//events.setup();

	ctx.c.cl_hud_color = interfaces::console->get_convar("cl_hud_color");
	ctx.c.cl_hud_background_alpha = interfaces::console->get_convar("cl_hud_background_alpha");
	ctx.c.cl_hud_healthammo_style = interfaces::console->get_convar("cl_hud_healthammo_style");

	if (!window)
		printf( ( "delusional | failed to get game window\n" ) );
	else
		printf( ( "delusional | found game window\n" ) );

	wndproc_original = reinterpret_cast< WNDPROC >( SetWindowLongW( window, GWL_WNDPROC, reinterpret_cast< LONG >( wndproc ) ) );
}

void sdk::hooks::unload() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
