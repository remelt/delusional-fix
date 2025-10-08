#include "../sdk/sdk.hpp"

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
	ent_list = get_interface<iv_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	event_manager = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	input_system = get_interface<i_input_system, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	trace_ray = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	game_movement = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");
	prediction = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");
	engine_sound = get_interface<void*, interface_type::index>("engine.dll", "IEngineSoundClient003");
	surface_props = get_interface<i_physics_surface_props, interface_type::index>("vphysics.dll", "VPhysicsSurfaceProps001");
	mdl_cache = get_interface<i_mdl_cache, interface_type::index>("datacache.dll", "MDLCache004");
	panorama = get_interface<i_panoramaui_engine, interface_type::index>("panorama.dll", "PanoramaUIEngine001");
	effects = get_interface<iv_effects, interface_type::index>("engine.dll", "VEngineEffects001");
	filesystem = get_interface<i_filesystem, interface_type::index>("filesystem_stdio.dll", "VFileSystem017");
	physics_collision = get_interface<c_physics_collison, interface_type::index>("vphysics.dll", "VPhysicsCollision007");

	/*custom interfaces*/
	input = *reinterpret_cast<i_input**>(find_pattern("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
	client_state = **reinterpret_cast<i_client_state***>(find_pattern("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
	client_mode = **reinterpret_cast<i_client_mode***>((*reinterpret_cast<uintptr_t**>(client))[10] + 5);
	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
	device = **reinterpret_cast<IDirect3DDevice9***>(find_pattern(("shaderapidx9.dll"), ("A1 ? ? ? ? 50 8B 08 FF 51 0C")) + 1);
	move_helper = **reinterpret_cast<player_move_helper***>(find_pattern(("client.dll"), ("8B 0D ? ? ? ? 8B 46 08 68")) + 2);
	render_beams = *reinterpret_cast<i_view_render_beams**>((find_pattern("client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1));
	weapon_system = *reinterpret_cast<i_weapon_system**>(find_pattern("client.dll", "8B 35 ? ? ? ? FF 10 0F B7 C0") + 2);
	glow_manager = reinterpret_cast<glow_manager_t*>(*reinterpret_cast<uintptr_t*>(find_pattern("client.dll", "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3));
	chat_element = *reinterpret_cast<c_hud_chat**>(reinterpret_cast<uintptr_t>(client_mode) + *reinterpret_cast<uint8_t*>(find_pattern(("client.dll"), ("E8 ? ? ? ? 8B 4F ? 85 C9 74 06 51")) + 7));

	//keyvalues
	if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))												
		key_values_system = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();	

	key_values_engine = find_pattern("engine.dll", "85 C0 74 ? 51 6A ? 56 8B C8 E8 ? ? ? ? 8B F0");
	key_values_client = find_pattern("client.dll", "85 C0 74 ? 6A ? 6A ? 56 8B C8 E8 ? ? ? ? 8B F0");

	// steam api
	const auto steam_api_ctx = engine->get_steam_api_contex();
	if (!steam_api_ctx)
		return false;

	steam_user = steam_api_ctx->steam_user;
	steam_friend = steam_api_ctx->steam_friends;
	steam_utils = steam_api_ctx->steam_utils;

	if (!steam_user || !steam_friend || !steam_utils)
		return false;

	D3DDEVICE_CREATION_PARAMETERS params;
	device->GetCreationParameters(&params);
	game_hwnd = params.hFocusWindow;

	return true;
}

entity_t* entity_handle_t::get() const {
	if (this->handle == 0xFFFFFFFF)
		return nullptr;

	return interfaces::ent_list->get_client_entity_handle(this->handle);
}

bool entity_handle_t::operator==(const entity_handle_t& other) const {
	return handle == other.handle;
}

bool entity_handle_t::operator==(entity_t* entity) const {
	return get() == entity;
}

bool entity_handle_t::operator!=(const entity_handle_t& other) const {
	return handle != other.handle;
}

bool entity_handle_t::operator!=(entity_t* entity) const {
	return get() != entity;
}