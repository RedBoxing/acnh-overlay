#pragma once
#include <dmnt/dmntcht.hpp>
#include <switch.h>
#include <vector>
#include <map>

namespace Memory
{
    Result attach();
    u64 getTitleId();
    u64 getProcessID();
    uintptr_t getBaseAddress();
    uintptr_t getBaseSize();
    uintptr_t getHeapAddress();
    uintptr_t getHeapSize();
    Result readMemory(u64 offset, void *buffer, size_t size);
    Result writeMemory(u64 offset, void *buffer, size_t size);
    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
    uintptr_t FindUnusedMemory(uintptr_t start, uintptr_t end, size_t size);

    u32 makeMov(u8 _register, u16 value);

    class Patch
    {
    public:
        Patch(std::map<uintptr_t, std::pair<u32, size_t>> patches);
        Patch(uintptr_t address, u32 value, size_t size) : Patch({{address, {value, size}}}) {}
        ~Patch();

        void apply();
        void revert();
        bool isEnabled();
        void setEnabled(bool enabled);
        void toogle();

    private:
        std::map<uintptr_t, std::pair<u32, size_t>> patches;
        std::map<uintptr_t, std::pair<u32, size_t>> original;
        bool enabled = false;
    };
};