#include "../hooks.hpp"
#include "../../menu/config/config.hpp"

void __fastcall sdk::hooks::modify_eye_position::modify_eye_position(void* thisPointer, void* edx, vec3_t& input_eye_pos) {
	const auto anim_state = static_cast<c_anim_state*>(thisPointer);

	if (!anim_state)
		return ofunc(thisPointer, edx, input_eye_pos);

	if (!anim_state->m_hit_ground && anim_state->m_duck_amount == 0.0f) {
		anim_state->m_smooth_height_valid = false;
		anim_state->m_camera_smooth_height = 0x7F7FFFFF;
		return;
	}

	const auto head_bone = anim_state->m_player->lookup_bone("head_0");
	if (head_bone == -1)
		return;

	auto head_pos = anim_state->m_player->get_hitbox_position(head_bone);
	head_pos.z += 1.7f;

	if (input_eye_pos.z <= head_pos.z)
		return;

	const auto lol = abs(input_eye_pos.z - head_pos.z);
	float value = (lol - 4.0) * 0.16666667;
	value = std::clamp(value, 0.0f, 1.0f);

	const auto value_squared = value * value;
	input_eye_pos.z = (head_pos.z - input_eye_pos.z) * (value_squared * 3.0 - value_squared * 2.0 * value) + input_eye_pos.z;
}
