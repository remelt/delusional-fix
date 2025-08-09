#include "../hooks.hpp"

bool __stdcall sdk::hooks::supports_full_depth::supports_full_depth() {
	if (!interfaces::material_system)
		return ofunc();

	constexpr auto nvidia = 0x10DEu;
	constexpr auto amd = 0x1002u;
	constexpr auto intel = 0x8086u;

	material_adapter_info_t info;
	auto adapter = interfaces::material_system->get_current_adapter();
	interfaces::material_system->get_display_adapter_info(adapter, &info);

	if (info.vendor_id == amd)
		return false;

	return ofunc();
}