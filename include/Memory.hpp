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

    namespace Instructions
    {
        u32 mov(u8 reg, u16 value, bool is64Bit);
        u32 movR(u8 dstReg, u8 srcReg, bool is64Bit);
        u32 movk(u8 reg, u16 imm, u8 shift, bool is64Bit);
        u32 movz(u8 reg, u16 imm, u8 shift, bool is64Bit);
        u32 branch(u32 offset, bool link = false);
        u32 branchR(u8 reg, bool link = false);
        u32 ldr(u8 srcReg, u8 dstReg, u16 offset, bool is64Bit);
        u32 addSp(u8 dst, u16 offset);
        u32 add(u8 dst, u8 src, u16 offset, bool is64Bit);
        u32 nop();
        u32 ret();
    };

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