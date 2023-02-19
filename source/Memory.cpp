#include <Memory.hpp>
#include <Utils.hpp>

namespace Memory
{
    static dmntcht::CheatProcessMetadata m_metadata;

    Result attach() { return dmntcht::getCheatProcessMetadata(&m_metadata); }
    u64 getProcessID() { return m_metadata.process_id; }
    u64 getTitleId() { return m_metadata.title_id; }
    uintptr_t getBaseAddress() { return m_metadata.main_nso_extents.base; }
    uintptr_t getBaseSize() { return m_metadata.main_nso_extents.size; }
    uintptr_t getHeapAddress() { return m_metadata.heap_extents.base; }
    uintptr_t getHeapSize() { return m_metadata.heap_extents.size; }
    Result readMemory(u64 address, void *buffer, size_t size) { return dmntcht::readCheatProcessMemory(address, buffer, size); }
    Result writeMemory(u64 address, void *buffer, size_t size) { return dmntcht::writeCheatProcessMemory(address, buffer, size); }

    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
    {
        uintptr_t addr = ptr;
        for (unsigned int i = 0; i < offsets.size(); ++i)
        {
            readMemory(addr, &addr, sizeof(addr));
            addr += offsets[i];
        }
        return addr;
    }

    uintptr_t FindUnusedMemory(uintptr_t start, uintptr_t end, size_t size)
    {
        // scan the memory for size bytes of 0x00
        uintptr_t addr = start;
        while (addr < end)
        {
            bool found = true;
            for (size_t i = 0; i < size; i++)
            {
                uint8_t byte = 0;
                readMemory(addr + i, &byte, sizeof(byte));
                if (byte != 0)
                {
                    found = false;
                    break;
                }
            }
            if (found)
                return addr;
            addr += 0x1000;
        }
    }

    namespace Instructions
    {
        u32 mov(u8 reg, u16 value, bool is64Bit)
        {
            // thx to Bidziila for this

            uint32_t M0VV = 0x52800000 + reg;
            uint32_t M0VVPlus = 0x52900000 + reg;
            uint32_t M0VV64 = 0xD2800000 + reg;
            uint32_t M0VVPlus64 = 0xD2900000 + reg;

            if (is64Bit)
            {
                return ((value <= 0x7FFF) ? M0VV64 : M0VVPlus64) + ((((value * 2) & 0xFFFF) << 4) & 0xFFFFF);
            }
            else
            {
                return ((value <= 0x7FFF) ? M0VV : M0VVPlus) + (((((value * 2) & 0xFFFF) << 4) & 0xFFFFF));
            }
        }

        u32 movR(u8 dstReg, u8 srcReg, bool is64Bit)
        {
            return (is64Bit ? 0xAA0003E0 : 0x2A0003E0) + (dstReg) + (srcReg << 16);
        }

        u32 movk(u8 reg, u16 imm, u8 shift, bool is64Bit)
        {
            return (is64Bit ? 0xF2800000 : 0x72800000) + (reg) + (imm << 5) + (shift << 17);
        }

        u32 movz(u8 reg, u16 imm, u8 shift, bool is64Bit)
        {
            return (is64Bit ? 0xD2800000 : 0x52800000) + (reg) + (imm << 5) + (shift << 17);
        }

        u32 branch(u32 offset, bool link)
        {
            return (link ? 0x94000000 : 0x14000000) + (offset >> 2);
        }

        u32 branchR(u8 reg, bool link)
        {
            return (link ? 0xD63F0000 : 0xD61F0000) + (reg << 5);
        }

        u32 ldr(u8 srcReg, u8 dstReg, u16 offset, bool is64Bit)
        {
            return (is64Bit ? 0xF9400000 : 0xB9400000) + (srcReg) + (dstReg << 5) + (offset << 8);
        }

        u32 addSp(u8 dst, u16 offset)
        {
            return 0x910003E0 + dst + (offset << 10);
        }

        u32 add(u8 dst, u8 src, u16 offset, bool is64Bit)
        {
            return (is64Bit ? 0x91000000 : 0x11000000) + (dst) + +(src << 5) + (offset << 10);
        }

        u32 nop()
        {
            return 0xD503201F;
        }

        u32 ret()
        {
            return 0xD65F03C0;
        }
    }

    Patch::Patch(std::map<uintptr_t, std::pair<u32, size_t>> patches)
    {
        this->patches = patches;
        for (auto patch : this->patches)
        {
            u32 original = 0;
            readMemory(patch.first, &original, patch.second.second);
            this->original[patch.first] = std::make_pair(original, patch.second.second);
        }
    }

    Patch::~Patch()
    {
        if (this->enabled)
            this->revert();
    }

    void Patch::apply()
    {
        for (auto patch : this->patches)
        {
            writeMemory(patch.first, &patch.second.first, patch.second.second);
        }
    }

    void Patch::revert()
    {
        for (auto patch : this->original)
        {
            writeMemory(patch.first, &patch.second.first, patch.second.second);
        }
    }

    bool Patch::isEnabled()
    {
        return this->enabled;
    }

    void Patch::setEnabled(bool enabled)
    {
        if (this->enabled == enabled)
            return;
        if (enabled)
            this->apply();
        else
            this->revert();
        this->enabled = enabled;
    }

    void Patch::toogle()
    {
        this->setEnabled(!this->enabled);
    }
}