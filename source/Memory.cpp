#include <Memory.hpp>
#include <Utils.hpp>

namespace Memory
{
    static dmntcht::CheatProcessMetadata m_metadata;

    Result attach() { return dmntcht::getCheatProcessMetadata(&m_metadata); }
    u64 getTitleId() { return m_metadata.title_id; }
    u64 getBaseAddress() { return m_metadata.main_nso_extents.base; }
    u64 getHeapAddress() { return m_metadata.heap_extents.base; }
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
}