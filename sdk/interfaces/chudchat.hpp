#pragma once
#include <cstdarg>
#include <xlocinfo>
#include "../../utils/vfunc.hpp"

#define buffer_size 1024
class c_hud_chat {
public:
	void chatprintf(const char* fmt, ...) {
		char msg[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(msg, fmt, args);
		get_vfunc< void(__cdecl*)(void*, int, int, const char*, ...) >(this, 27)(this, 0, 0, fmt);
		va_end(args);
	}
};