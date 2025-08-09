#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

void* __stdcall sdk::hooks::alloc_key_values_memory::alloc_key_values_memory(const std::int32_t size) {
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_engine) ||
		address == reinterpret_cast<std::uint32_t>(interfaces::key_values_client))
		return nullptr;

	return ofunc(interfaces::key_values_system, size);
}