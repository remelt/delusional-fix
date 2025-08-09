#pragma once
#include "../../utils/padding.hpp"
#include "../../utils/vfunc.hpp"


class player_t;
class iv_client_entity_list {
public:
	void* get_client_entity( int index ) {
		using fn = void* ( __thiscall* )( iv_client_entity_list*, int );
		return ( *( fn** ) this )[ 3 ]( this, index );
	}
	player_t* get_client_entity_handle( int handle ) {
		using fn = player_t * ( __thiscall* )( iv_client_entity_list*, int );
		return ( *( fn** ) this )[ 4 ]( this, handle );
	}
	void* get_client_entity_handle_alt( std::uint32_t handle_index ) {
		using fn = void* ( __thiscall* )( iv_client_entity_list*, std::uint32_t );
		return ( *( fn** ) this )[ 4 ]( this, handle_index );
	}
	int get_highest_index( ) {
		using fn = int( __thiscall* )( iv_client_entity_list* );
		return ( *( fn** ) this )[ 6 ]( this );
	}

	template< class T >
	T* get( int index )
	{
		return reinterpret_cast< T* >( get_client_entity( index ) );
	}
};