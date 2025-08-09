#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/visuals/visuals.hpp"

bool __stdcall sdk::hooks::should_draw_viewmodel::should_draw_viewmodel() {
    if (c::visuals::apply_zoom && g::local && g::local->fov() < 45 && g::local->fov_start() < 45)
        return false;

    return ofunc();
}