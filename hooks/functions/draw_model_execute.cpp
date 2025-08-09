#include "../../features/movement/prediction/prediction.hpp"
#include "../../features/movement/movement.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../features/visuals/visuals.hpp"
#include "../../features/misc/misc.hpp"
#include "../../menu/config/config.hpp"
#include "../../sdk/math/math.hpp"
#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

void __fastcall sdk::hooks::draw_model_execute::draw_model_execute(void* _this, int edx, i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* matrix) {
	static auto chams_init = (features::chams::initialize(), false);

	if (interfaces::model_render->is_forced_material_override() && !strstr(info.model->name, "arms") && !strstr(info.model->name, "weapons/v_")) {
		return 	draw_model_execute_original(_this, edx, ctx, state, info, matrix);
	}

	features::chams::run_bt(ctx, state, info, matrix);
	features::chams::run(ctx, state, info, matrix);

	draw_model_execute_original(_this, edx, ctx, state, info, matrix);
	interfaces::model_render->override_material(nullptr);

}