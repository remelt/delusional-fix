#include "knife_proxy.hpp"
#include "skins.hpp"

#include "../../sdk/sdk.hpp"
#include "../../menu/config/config.hpp"

void features::skins::hooked_recvproxy_viewmodel(c_recv_proxy_data* p_data, void* p_struct, void* p_out) {

	if (!c::skins::knife_changer_model || !c::skins::knife_changer_enable)
		return recv_model_index(p_data, p_struct, p_out);

	for (int i = 0; i < IM_ARRAYSIZE(models::knives); i++) {
		if (i == 0) { continue; }
		if (p_data->value.m_int == interfaces::model_info->get_model_index(models::knives[i])) {
			p_data->value.m_int = interfaces::model_info->get_model_index(models::knives[c::skins::knife_changer_model]);
			break;
		}
	}

	recv_model_index(p_data, p_struct, p_out);
}

void features::skins::set_view_model_sequence(const c_recv_proxy_data* pDataConst, void* p_struct, void* p_out) {
	c_recv_proxy_data* p_data = const_cast<c_recv_proxy_data*>(pDataConst);
	base_view_model* player_view_model = static_cast<base_view_model*>(p_struct);

	if (!c::skins::knife_changer_model || !c::skins::knife_changer_enable)
		return sequence_proxy_fn(p_data, p_struct, p_out);

	if (player_view_model) {
		auto local_player = reinterpret_cast<player_t*>(interfaces::ent_list->get_client_entity(interfaces::engine->get_local_player()));
		player_t* p_owner = static_cast<player_t*>(interfaces::ent_list->get_client_entity(player_view_model->m_howner() & 0xFFF));
		if (p_owner == local_player) {
			std::string sz_model = interfaces::model_info->get_model_name(interfaces::model_info->get_model(player_view_model->model_index()));
			int m_nSequence = p_data->value.m_int;
			if (sz_model == "models/weapons/v_knife_butterfly.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (sz_model == "models/weapons/v_knife_falchion_advanced.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_push.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (sz_model == "models/weapons/v_knife_survival_bowie.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_ursus.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_stiletto.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_widowmaker.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_css.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_css.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = 15;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_cord.mdl" || sz_model == "models/weapons/v_knife_canis.mdl" || sz_model == "models/weapons/v_knife_outdoor.mdl" || sz_model == "models/weapons/v_knife_skeleton.mdl") {
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
				}
			}
			p_data->value.m_int = m_nSequence;
		}
	}
	sequence_proxy_fn(p_data, p_struct, p_out);
}

void features::skins::animation_hook() {
	c_client_class* pClass = interfaces::client->get_all_classes();
	while (pClass)
	{
		const char* pszName = pClass->recvtable_ptr->table_name;
		if (!strcmp(pszName, ("DT_BaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClass->recvtable_ptr->props_count; nIndex++) {
				recv_prop* pProp = &(pClass->recvtable_ptr->props[nIndex]);

				if (!pProp || strcmp(pProp->prop_name, ("m_nSequence"))) continue;

				sequence_proxy_fn = (recv_var_proxy_fn)pProp->proxy_fn;

				pProp->proxy_fn = (recv_var_proxy_fn)set_view_model_sequence;
			}
		}

		if (!strcmp(pszName, ("DT_BaseViewModel")))
		{
			for (int i = 0; i < pClass->recvtable_ptr->props_count; i++)
			{
				recv_prop* pProp = &(pClass->recvtable_ptr->props[i]);
				const char* name = pProp->prop_name;

				if (!strcmp(name, ("m_nModelIndex")))
				{
					recv_model_index = (recv_var_proxy_fn)pProp->proxy_fn;
					pProp->proxy_fn = (recv_var_proxy_fn)hooked_recvproxy_viewmodel;
				}
			}
		}
		pClass = pClass->next_ptr;
	}
	printf(("delusional | knife animations initialized\n"));
}

void features::skins::animation_unhook() {
	for (c_client_class* pClass = interfaces::client->get_all_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, ("CBaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, ("m_nSequence"))) continue;

				pProp->proxy_fn = sequence_proxy_fn;

				break;
			}
			break;
		}
	}

	for (c_client_class* pClass = interfaces::client->get_all_classes(); pClass; pClass = pClass->next_ptr) {
		if (!strcmp(pClass->network_name, ("CBaseViewModel"))) {
			recv_table* pClassTable = pClass->recvtable_ptr;

			for (int nIndex = 0; nIndex < pClassTable->props_count; nIndex++) {
				recv_prop* pProp = &pClassTable->props[nIndex];

				if (!pProp || strcmp(pProp->prop_name, ("m_nModelIndex"))) continue;

				pProp->proxy_fn = recv_model_index;

				break;
			}
			break;
		}
	}
}