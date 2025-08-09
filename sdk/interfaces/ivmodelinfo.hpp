#pragma once
#include "../structs/models.hpp"
#include "../classes/studio.hpp"

class iv_model_info {
public:
	model_t* get_model( int index ) {
		using fn = model_t * ( __thiscall* )( iv_model_info*, int );
		return ( *( fn** ) this )[ 1 ]( this, index );
	}
	int get_model_index( const char* filename ) {
		using fn = int( __thiscall* )( iv_model_info*, const char* );
		return ( *( fn** ) this )[ 2 ]( this, filename );
	}
	const char* get_model_name( const model_t* model ) {
		using fn = const char* ( __thiscall* )( iv_model_info*, const model_t* );
		return ( *( fn** ) this )[ 3 ]( this, model );
	}
	studio_hdr_t* get_studio_model( const model_t* model ) {
		using fn = studio_hdr_t * ( __thiscall* )( iv_model_info*, const model_t* );
		return ( *( fn** ) this )[ 32 ]( this, model );
	}
};