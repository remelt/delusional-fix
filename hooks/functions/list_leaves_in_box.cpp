#include "../../features/aimbot/aimbot.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/math/math.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
int __fastcall sdk::hooks::list_leaves_in_box::list_leaves_in_box(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max) {
	static auto list_leaves = find_pattern("client.dll", "56 52 FF 50 18") + 5;
	auto info = *reinterpret_cast<renderable_info_t**>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if (!c::chams::invisible_chams || (_ReturnAddress()) != list_leaves)
		return ofunc(bsp, mins, maxs, list, list_max);

	if (!info || !info->renderable)
		return ofunc(bsp, mins, maxs, list, list_max);

	auto entity = call_vfunc<entity_t*>(info->renderable - 4, 7);

	if (!entity || entity->client_class()->class_id != class_ids::ccsplayer)
		return ofunc(bsp, mins, maxs, list, list_max);

	info->flags &= ~0x100;
	info->flags2 |= 0x40;

	static vec3_t map_min = vec3_t(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static vec3_t map_max = vec3_t(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
	return ofunc(bsp, map_min, map_max, list, list_max);
}