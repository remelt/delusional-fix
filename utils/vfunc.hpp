#pragma once
#include <cstdint>
#include <cstddef>

template <typename T = void*>
constexpr T get_vfunc( void* thisptr, std::size_t idx ) {
	return ( *static_cast< T** >( thisptr ) )[ idx ];
}

template <typename T, typename ... Args_t>
constexpr T call_vfunc(void* this_ptr, std::size_t index, Args_t... arg_list) {
	using virtual_fn = T(__thiscall*)(void*, decltype(arg_list)...);
	return (*static_cast<virtual_fn**>(this_ptr))[index](this_ptr, arg_list...);
}
