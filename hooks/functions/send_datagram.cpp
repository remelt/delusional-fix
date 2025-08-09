#define NOMINMAX
#include "../hooks.hpp"
#include "../../features/aimbot/aimbot.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/movement.hpp"

int __fastcall sdk::hooks::send_datagram::send_datagram(i_net_channel* thisptr, int edx, bf_write* datagram){

	i_net_channel_info* net_channel_info = interfaces::engine->get_net_channel_info();
	static convar* sv_maxunlag = interfaces::console->get_convar("sv_maxunlag");

	if (!interfaces::engine->is_in_game() || !c::backtrack::backtrack || !c::backtrack::fake_latency || datagram != nullptr || net_channel_info == nullptr || sv_maxunlag == nullptr)
		return ofunc(thisptr, edx, datagram);

	int state = thisptr->in_reliable_state;
	int sequence = thisptr->in_sequence_nr;

	// prob forgot to divide to get the right result
	const float max_latency = std::max(0.f, std::clamp(c::backtrack::fake / 1000.f, 0.f, sv_maxunlag->get_float()) - net_channel_info->get_latency(FLOW_OUTGOING));
	fakeping.add_latency(thisptr, max_latency);

	const int _return = ofunc(thisptr, edx, datagram);

	thisptr->in_reliable_state = state;
	thisptr->in_sequence_nr = sequence;

	return _return;
}