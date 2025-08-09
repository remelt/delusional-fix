#include "../../sdk/sdk.hpp"
#include "../hooks.hpp"

bool __stdcall sdk::hooks::loose_files_allowed::loose_files_allowed() {
	return true;	// allowing the files:V
}

void __fastcall sdk::hooks::check_for_pure_server_white_list::check_for_pure_server_white_list(void* edx, void* ecx) {
	return;			// stop the game from checking the whitelist
}