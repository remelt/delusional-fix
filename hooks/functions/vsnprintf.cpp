#define _CRT_SECURE_NO_WARNINGS
#include "../hooks.hpp"

int __cdecl sdk::hooks::vsnprintf::vsnprintf( char* dest, int text_len, const char* fmt, ... ) {   
    int v4;

    va_list args;
    va_start( args, fmt );
    if (strstr( fmt, "fps:" )) {
        fmt = "[delusional] fps: %5i / var: %4.1f ms / ping: %i ms";
        v4 = _vsnprintf( dest, text_len, fmt, args );
    }
    else if (strstr( fmt, "loss:" )) {
        fmt = "loss: %3i%% / choke: %2i%%";
        v4 = _vsnprintf( dest, text_len, fmt, args );
    }
    else if (strstr( fmt, "tick:")) {
        fmt = "tr: %.f / ";
        v4 = _vsnprintf( dest, text_len, fmt, args );
    }
    else if (strstr( fmt, "sv:" )) {
        fmt = "sv: %5.1f / %s%4.1f ms";
        v4 = _vsnprintf( dest, text_len, fmt, args );
    }
    else
        v4 = _vsnprintf( dest, text_len, fmt, args );
    va_end( args );

    if (( v4 < 0 ) || ( text_len > 0 && v4 >= text_len )) {
        v4 = text_len - 1;
        dest[ text_len - 1 ] = '\0';
    }

    return v4;
}