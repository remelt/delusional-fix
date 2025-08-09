#pragma once
#include <Windows.h>
#include <vector>

#define in_range(x,a,b)   (x >= a && x <= b)
#define get_bits( x )    (in_range((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (in_range(x,'0','9') ? x - '0' : 0))
#define get_byte( x )    (get_bits(x[0]) << 4 | get_bits(x[1]))

template <typename T>
static auto real2abs(const T& address) {
    return address + 4 + reinterpret_cast<int>(address);
}

inline uint8_t* find_pattern(const char* mod_name, const char* sig) {
    static int errCodeCount = 0;
    errCodeCount++;
    const auto module_handle = GetModuleHandle(mod_name);

    if (!module_handle)
        return nullptr;

    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + std::strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;

                if (*current == '?')
                    ++current;

                bytes.push_back(-1);
            }
            else {
                bytes.push_back(std::strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
    auto nt_headers =
        reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

    auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
    auto pattern_bytes = pattern_to_byte(sig);
    auto scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

    auto s = pattern_bytes.size();
    auto d = pattern_bytes.data();

    for (auto i = 0ul; i < size_of_image - s; ++i) {
        bool found = true;

        for (auto j = 0ul; j < s; ++j) {
            if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found)
            return &scan_bytes[i];
    }

    printf(("delusional | sig %s not found in %s\n", sig, mod_name));
    return nullptr;
}

inline uintptr_t find_sig_ida(const char* module1, const char* signature) {
    const char* pat = signature;
    DWORD first_match = 0;
    DWORD range_start = (DWORD)GetModuleHandleA(module1);
    MODULEINFO module_info;
    GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &module_info, sizeof(MODULEINFO));
    DWORD range_end = range_start + module_info.SizeOfImage;
    for (DWORD pCur = range_start; pCur < range_end; pCur++) {
        if (!*pat)
            return first_match;

        if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == get_byte(pat)) {
            if (!first_match)
                first_match = pCur;

            if (!pat[2])
                return first_match;

            if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
                pat += 3;

            else
                pat += 2;
        }
        else {
            pat = signature;
            first_match = 0;
        }
    }
    return NULL;
}

inline std::uintptr_t* find_hud_element(const char* name) {

    static auto hud = *reinterpret_cast<void**>(find_pattern("client.dll", "B9 ? ? ? ? 68 ? ? ? ? E8 ? ? ? ? 89") + 0x1); 

    using find_hud_element_fn = std::uintptr_t* (__thiscall*)(void*, const char*);
    static auto o_find_hud_element = reinterpret_cast<find_hud_element_fn>(find_pattern("client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28")); 
    assert(o_find_hud_element != nullptr);

    return o_find_hud_element(hud, name);
}

inline vec2_t get_mouse_position() {
    POINT mousePosition;
    GetCursorPos(&mousePosition);
    ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
    return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
}

inline bool mouse_in_region(int x, int y, int x2, int y2) {
    if (get_mouse_position().x > x && get_mouse_position().y > y && get_mouse_position().x < x2 + x && get_mouse_position().y < y2 + y)
        return true;
    return false;
}

inline void copy_convert(const uint8_t* rgba, uint8_t* out, const size_t size) {
    auto in = reinterpret_cast<const uint32_t*>(rgba);
    auto buf = reinterpret_cast<uint32_t*>(out);
    for (auto i = 0u; i < (size / 4); ++i) {
        const auto pixel = *in++;
        *buf++ = (pixel & 0xFF00FF00) | ((pixel & 0xFF0000) >> 16) | ((pixel & 0xFF) << 16);
    }
}
