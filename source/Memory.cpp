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

    u32 makeMov(u8 _register, u16 value)
    {
        return 0xD2800000 | (_register) | (value << 5);
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