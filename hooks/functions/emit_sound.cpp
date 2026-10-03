#include "../hooks.hpp"
#include "../../menu/config/config.hpp"
#include "../../features/movement/prediction/prediction.hpp"

void __fastcall sdk::hooks::emit_sound::emit_sound(registers, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* fds) {

	if (!prediction::in_prediction)
		ofunc( ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, fds );
}