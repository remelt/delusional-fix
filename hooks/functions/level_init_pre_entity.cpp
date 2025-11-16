#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/panorama/scaleform.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/movement/prediction/prediction.hpp"

void __fastcall sdk::hooks::level_init_pre_entity::level_init_pre_entity(void* self, void* edx, const char* map) {
	ofunc(self, edx, map);
	return panorama::scaleform_modify_all();
}

void __fastcall sdk::hooks::level_shutdown::level_shutdown(void* self, void* edx) {
	ofunc(self, edx);
	return panorama::scaleform_init();
    features::weather::reset_weather(false);
}

// false - vsvg
// true - svg
#define VSVG_OR_SVG true
#define VSVG_EXT ((VSVG_OR_SVG) ? ".svg" : ".vsvg")
static const char* get_extension(const char* filename) {
    if (ctx.f.compare_extension(filename, ".png"))
        return ".png";
    else if (ctx.f.compare_extension(filename, ".svg"))
        return ".svg";
    else if (ctx.f.compare_extension(filename, ".vsvg"))
        return VSVG_EXT;
    return nullptr; // we don't care abt. the rest
}

#define WIN32_LINUX_LITERAL(x, y) x
#define DUMP_ICONS 0
#define DUMP_FILENAMES 0
bool __fastcall sdk::hooks::set_image_data_r8g8b8a8::set_image_data_r8g8b8a8(const uint8_t* data, uint32_t len, const char* filename, int w, int h, void* arg1, int arg2) {
	if (!c::sfui::sfui_on || !filename)
		return ofunc(data, len, filename, w, h, arg1, arg2);

#if (DUMP_FILENAMES == 1)

#endif

    const char* extension = get_extension(filename);
    // Data is actually the file itself, it's not first turned into RGBA
    // data.
    if (!data || !len || !extension)
        return ofunc(data, len, filename, w, h, arg1, arg2);

    //DEBUG("extension: %s arg1: %d arg2: %d\n", extension, arg1, arg2);
    constexpr const char eq_prefix[] = WIN32_LINUX_LITERAL("materials\\panorama\\images\\icons\\equipment\\", "materials/panorama/images/icons/equipment/");
    bool equipment = (strstr(filename, eq_prefix) == filename) && !strcmp(extension, VSVG_EXT);

    if (equipment)
    {
        // gonna be written to by reference
        const uint8_t* replacement_data;
        size_t replacement_size;
        int replacement_w, replacement_h;

        // name
        char copy[256];
        const char* start_ptr = &filename[sizeof(eq_prefix) - 1];
        const char* end_ptr = strstr(start_ptr, ".vsvg");
        size_t size = (size_t)end_ptr - (size_t)start_ptr;
        strncpy(copy, start_ptr, size);
        copy[size] = 0;

#if (DUMP_ICONS == 1)
        scaleform_dump_icons(copy, data, len, extension);
#endif

        if (panorama::scaleform_get_replacement_icon(copy, replacement_data, replacement_size, replacement_w, replacement_h))
        {
            arg2 = 6;
            return ofunc(replacement_data, replacement_size, filename, replacement_w, replacement_h, arg1, arg2);
        }
        else {

        }
    }

    return (data, len, filename, w, h, arg1, arg2);
}