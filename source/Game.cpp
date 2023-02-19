#include <Game.hpp>
#include <Memory.hpp>
#include <stdlib.h>

uintptr_t Game::Offsets::MainSave = 0x0;
uintptr_t Game::Offsets::PersonalSave = 0x0;
uintptr_t Game::Offsets::Inventory = 0x0;
uintptr_t Game::Offsets::PlayerPosition = 0x0;
uintptr_t Game::Offsets::CameraPosition = 0x0;
uintptr_t Game::Offsets::ChatBuffer = 0x0;
uintptr_t Game::Offsets::CurrentTime = 0x0;

Memory::Patch *Game::Patches::eatEverythings = 0x0;
Memory::Patch *Game::Patches::freezeTime = 0x0;

HiddbgHdlsHandle handle;
HiddbgHdlsSessionId session_id;

void Game::Initialize()
{
#ifdef LIBRED
    Game::Private::Initialize();
#endif

    Offsets::MainSave = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C1AD20, {0x10, 0x0, 0x10, 0x0});
    Offsets::PersonalSave = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C1AD20, {0x10, 0x0, 0x28, 0x0});
    Offsets::PlayerPosition = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x50CFDE8, {0x20, 0x08, 0x20, 0x10});
    Offsets::Inventory = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C1AD20, {0x1A0, 0x08, 0xC8});
    Offsets::CameraPosition = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C71578, {0xF0, 0x70});
    Offsets::ChatBuffer = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4AA9CD8, {0x40});
    Offsets::CurrentTime = Memory::getBaseAddress() + 0xBD3A18C;

    Patches::eatEverythings = new Memory::Patch(Memory::getBaseAddress() + 0x1F08308, Memory::Instructions::nop(), 0x4);
    Patches::freezeTime = new Memory::Patch(Memory::getBaseAddress() + 0x328BD0, Memory::Instructions::nop(), 0x4);

    // bgm volume is at 0x1A02148

    /* TIME

    if (keys & KEY_DRIGHT)
    {
      Wrapper::Write32(0, 0x328BD0, 0xD503201F); //Freeze time
      Wrapper::AddToOffset(1, 0xBD3A18C, 0x01);
      return true;
    }
    else if (keys & KEY_DLEFT)
    {
       Wrapper::Write32(0, 0x328BD0, 0xD503201F); //Freeze time
       Wrapper::SubFromOffset(1, 0xBD3A18C, 0x01);
       return true;
    }

    Shops open 24 hours - 1B06370 52800020
    */

    HiddbgHdlsDeviceInfo device = {0};

    device.deviceType = HidDeviceType_FullKey15;
    device.singleColorBody = RGBA8_MAXALPHA(255, 255, 255);
    device.singleColorButtons = RGBA8_MAXALPHA(0, 0, 0);

    hiddbgAttachHdlsWorkBuffer(&session_id);
    hiddbgAttachHdlsVirtualDevice(&handle, &device);
}

void Game::Exit()
{
    hiddbgDetachHdlsVirtualDevice(handle);
    hiddbgReleaseHdlsWorkBuffer(session_id);

    Patches::eatEverythings->setEnabled(false);

#ifdef LIBRED
    Game::Private::Exit();
#endif
}

std::map<char *, Game::Building *> Game::getBuildings()
{
    std::map<char *, Game::Building *> buildings;

    for (int i = 0; i < Game::Constants::Map::BuildingCount; i++)
    {
        char *buffer = new char[sizeof(Game::Building)];
        Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::MainFieldStructure + (i * sizeof(Game::Building)), buffer, sizeof(Game::Building));
        Game::Building *building = reinterpret_cast<Game::Building *>(buffer);
        buildings[(char *)buildingTypeToString(building->buildingType)] = building;
    }

    return buildings;
}

Game::Building *Game::getBuilding(int index)
{
    char *buffer = new char[sizeof(Game::Building)];
    Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::MainFieldStructure + (index * sizeof(Game::Building)), buffer, sizeof(Game::Building));
    return reinterpret_cast<Game::Building *>(buffer);
}

void Game::setBuilding(Game::Building *building, int index)
{
    Memory::writeMemory(Game::Offsets::MainSave + Game::Save::Main::MainFieldStructure + (index * sizeof(Game::Building)), building, sizeof(Game::Building));
}

const char *Game::buildingTypeToString(Game::BuildingType type)
{
    switch (type)
    {
    case Game::BuildingType::None:
        return "None";
    case Game::BuildingType::PlayerHouse1:
        return "Player House 1";
    case Game::BuildingType::PlayerHouse2:
        return "Player House 2";
    case Game::BuildingType::PlayerHouse3:
        return "Player House 3";
    case Game::BuildingType::PlayerHouse4:
        return "Player House 4";
    case Game::BuildingType::PlayerHouse5:
        return "Player House 5";
    case Game::BuildingType::PlayerHouse6:
        return "Player House 6";
    case Game::BuildingType::PlayerHouse7:
        return "Player House 7";
    case Game::BuildingType::PlayerHouse8:
        return "Player House 8";
    case Game::BuildingType::Villager1:
        return "Villager 1";
    case Game::BuildingType::Villager2:
        return "Villager 2";
    case Game::BuildingType::Villager3:
        return "Villager 3";
    case Game::BuildingType::Villager4:
        return "Villager 4";
    case Game::BuildingType::Villager5:
        return "Villager 5";
    case Game::BuildingType::Villager6:
        return "Villager 6";
    case Game::BuildingType::Villager7:
        return "Villager 7";
    case Game::BuildingType::Villager8:
        return "Villager 8";
    case Game::BuildingType::Villager9:
        return "Villager 9";
    case Game::BuildingType::Villager10:
        return "Villager 10";
    case Game::BuildingType::NooksCranny:
        return "Nook's Cranny";
    case Game::BuildingType::ResidentCenterStructure:
        return "Resident center structure";
    case Game::BuildingType::Museum:
        return "Museum";
    case Game::BuildingType::Airport:
        return "Airport";
    case Game::BuildingType::ResidentCenterTent:
        return "Resident center tent";
    case Game::BuildingType::AblesSisters:
        return "Ables' Sisters";
    case Game::BuildingType::Campsite:
        return "Campsite";
    case Game::BuildingType::Bridge:
        return "Bridge";
    case Game::BuildingType::Incline:
        return "Incline";
    case Game::BuildingType::ReddsTreasureTrawler:
        return "Redd's Treasure Trawler";
    case Game::BuildingType::Studio:
        return "Studio";
    default:
        return "Unknown";
    }

    return "";
}

Vector2 *Game::getPlayerPosition()
{
    Vector2 *position = (Vector2 *)malloc(sizeof(Vector2));

    Memory::readMemory(Game::Offsets::PlayerPosition, &position->x, 4);
    Memory::readMemory(Game::Offsets::PlayerPosition + 4, &position->y, 4);

    return position;
}

Game::CameraOffsets *Game::getCameraPosition()
{
    CameraOffsets *position = (CameraOffsets *)malloc(sizeof(CameraOffsets));
    Memory::readMemory(Game::Offsets::CameraPosition, position, sizeof(CameraOffsets));
    return position;
}

int Game::Map::getTileIndex(int x, int y)
{
    return (Constants::Map::FieldItemHeight * x) + y;
}

Vector2 Game::Map::getTilePosition(int index)
{
    Vector2 position;
    position.x = index / Constants::Map::FieldItemHeight;
    position.y = index % Constants::Map::FieldItemHeight;
    return position;
}

Game::Item Game::Map::getTile(Item *items, int x, int y)
{
    return items[getTileIndex(x, y)];
}

Game::Item *Game::Map::getFieldItems()
{
    Item *items = new Item[Game::Constants::Map::MapTileCount];
    Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::FieldItem, items, Game::Constants::Map::FieldItemLayerSize);
    return items;
}

void Game::Map::writeTile(int index, Item item)
{
    Memory::writeMemory(Game::Offsets::MainSave + Game::Save::Main::FieldItem + (index * sizeof(Item)), &item, sizeof(Item));
}

Vector2 *Game::Map::getPlazaCordinates()
{
    Vector2 *position = (Vector2 *)malloc(sizeof(Vector2));
    // Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 4, &position->x, 4);
    // Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 8, &position->y, 4);
    Memory::readMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 4, position, 8);
    return position;
}

void Game::Map::setPlazaCordinates(Vector2 *position)
{
    // Memory::writeMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 4, (void *)position->x, 4);
    // Memory::writeMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 8, (void *)position->y, 4);
    Memory::writeMemory(Game::Offsets::MainSave + Game::Save::Main::OutsideField + Game::Constants::Map::AcreSizeAll + 4, position, 8);
}

Game::Item *Game::Inventory::getItems()
{
    Item *items = new Item[Game::Constants::Inventory::InventorySize];
    Memory::readMemory(Game::Offsets::Inventory, items, Game::Constants::Inventory::InventorySize * sizeof(Item));
    return items;
}

Game::Item Game::Inventory::getItem(int index)
{
    Game::Item *item = new Game::Item();
    Memory::readMemory(Game::Offsets::Inventory + (index * sizeof(Game::Item)), item, sizeof(Game::Item));
    return *item;
}

void Game::Inventory::setItem(Game::Item item, int index)
{
    Memory::writeMemory(Game::Offsets::Inventory + (index * sizeof(Game::Item)), &item, sizeof(Game::Item));
}

int Game::Inventory::findSlot(u16 id)
{
    for (int i = 0; i < Game::Constants::Inventory::InventorySize; i++)
    {
        Game::Item item = Game::Inventory::getItem(i);
        if (item.id == id)
        {
            return i;
        }
    }

    return -1;
}

u8 Game::Time::getCurrentTime()
{
    u8 time;
    Memory::readMemory(Game::Offsets::CurrentTime, &time, 1);
    return time;
}

void Game::Time::incrementTime(u8 time)
{
    u8 value = getCurrentTime() + time;
    Memory::writeMemory(Game::Offsets::CurrentTime, &value, 1);
}

void Game::Time::decrementTime(u8 time)
{
    u8 value = getCurrentTime() - time;
    Memory::writeMemory(Game::Offsets::CurrentTime, &value, 1);
}

void Game::pressKey(u64 key)
{
    HiddbgHdlsState state = {0};
    state.buttons = key;
    hiddbgSetHdlsState(handle, &state);

    // wait 0.1 seconds
    svcSleepThread(100000000);

    state.buttons = 0;
    hiddbgSetHdlsState(handle, &state);
}