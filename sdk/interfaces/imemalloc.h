#pragma once

// @source: master/public/tier0/memalloc.h
using MemAllocFailHandler_t = std::size_t(__cdecl*)(std::size_t);

class IMemAlloc
{
public:
	void* Allocate(std::size_t nSize)
	{
		using original_fn = void*(__thiscall*)(void*, std::size_t);
		return (*(original_fn**)this)[1](this, nSize);
	}

	void Freee(void* pMemory)
	{
		using original_fn = void(__thiscall*)(void*, void*);
		return (*(original_fn**)this)[5](this, pMemory);
	}
};
