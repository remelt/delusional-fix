#pragma once
#include <cstdint>
#include "utilities.hpp"

class c_key_values {
public:
	void init(const char* key_name) {
		using initkv_t = void(__thiscall*)(void*, const char*, int, int);
		static auto initkv_fn = *reinterpret_cast<initkv_t>(find_pattern(("client.dll"), "55 8B EC 51 33 C0 C7 45"));
		initkv_fn(this, key_name, 0, 0);
	}

	void load_from_buffer(const char* resource, const char* buf, void* file = nullptr, const char* id = nullptr, void* eval_proc = nullptr, void* unk = nullptr) {
		using loadfrombuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		static auto loadfrombuffer_fn = *reinterpret_cast<loadfrombuffer_t>(find_pattern("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
		loadfrombuffer_fn(this, resource, buf, file, id, eval_proc, unk);
	}
};