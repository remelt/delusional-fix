#pragma once

#include "../classes/studio.hpp"
#include "../interfaces/iappsystem.hpp"

struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

enum MDLCacheDataType_t
{
	// Callbacks to Get called when data is loaded or unloaded for these:
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	// Callbacks NOT called when data is loaded or unloaded for these:
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK
};

enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

class IMDLCacheNotify
{
public:
	virtual void OnDataLoaded( MDLCacheDataType_t type, MDLHandle_t handle ) = 0;
	virtual void OnDataUnloaded( MDLCacheDataType_t type, MDLHandle_t handle ) = 0;
};

class i_mdl_cache
{
public:
	void begin_lock() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[33](this);
	}

	void end_lock() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[34](this);
	}
};
