#pragma once

class i_panel {
public:
	void set_keyboard_input_enabled( unsigned int panel_id, bool state ) {
		using fn = void( __thiscall* )( i_panel*, unsigned int, bool );
		return ( *( fn** ) this )[ 31 ]( this, panel_id, state );
	}

	void set_mouse_input_enabled( unsigned int panel_id, bool state ) {
		using fn = void( __thiscall* )( i_panel*, unsigned int, bool );
		return ( *( fn** ) this )[ 32 ]( this, panel_id, state );
	}

	const char* get_panel_name( unsigned int panel_id ) {
		using fn = const char* ( __thiscall* )( i_panel*, unsigned int );
		return ( *( fn** ) this )[ 36 ]( this, panel_id );
	}
};