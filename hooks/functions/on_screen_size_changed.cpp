#include "../hooks.hpp"
#include "../../utils/render/render.hpp"

void __stdcall sdk::hooks::on_screen_size_changed::on_screen_size_changed(int old_width, int old_height) {
	ofunc(interfaces::surface, old_width, old_height);

	surface::initialize();
}