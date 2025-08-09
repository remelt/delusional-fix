#include "../hooks.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::calcultate_view::calcultate_view(player_t* player, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov) {
	if (!player || !g::local || player != g::local)
		return ofunc(player, edx, eye_origin, eye_angles, z_near, z_far, fov);

	const auto backup_use_new_anim_state = player->should_use_new_anim_state();

	player->should_use_new_anim_state() = false; {
		ofunc(player, edx, eye_origin, eye_angles, z_near, z_far, fov);
	}
	player->should_use_new_anim_state() = backup_use_new_anim_state;

}
