#pragma once
#include <dmnt/dmntcht.hpp>
#include <switch.h>
#include <vector>

namespace Memory {
    Result attach();
    u64 getTitleId();
    u64 getBaseAddress();
    u64 getHeapAddress();
    Result readMemory(u64 offset, void *buffer, size_t size);
    Result writeMemory(u64 offset, void *buffer, size_t size);
    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
};