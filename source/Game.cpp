#include <Game.hpp>
#include <Memory.hpp>
#include <stdlib.h>

uintptr_t Game::MainSave = 0x0;
uintptr_t Game::PersonalSave = 0x0;
uintptr_t Game::Inventory = 0x0;
uintptr_t Game::EatEverythings = 0x0;
uintptr_t Game::PlayerPosition = 0x0;

void Game::Initialize()
{
    //[[[[main+4C1AD20]+10]]+10]
    MainSave = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C1AD20, {0x10, 0x0, 0x10, 0x0});
    PersonalSave = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x4C1AD20, {0x10, 0x0, 0x28, 0x0});

    PlayerPosition = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x50CFDE8, {0x20, 0x08, 0x20, 0x10});

    //[[[main+3D96720]+190]+08]+C8
    // Inventory = Memory::getHeapAddress() + 0xAE19C778; //Memory::FindDMAAddy(Memory::getBaseAddress() + 0x3D96720, { 0x190, 0x08, 0xC8 });

    //[[[[[[main+39A0DE0]+38]+90]+08]+2D8]+61C]+9FC
    // EatEverythings = Memory::getHeapAddress() + 0x387D5528; //Memory::FindDMAAddy(Memory::getBaseAddress() + 0x39A0DE0, { 0x38, 0x90, 0x08, 0x2D8, 0x61C, 0x9FC });
}

std::vector<Game::Building> Game::getBuildings()
{
    std::vector<Game::Building> buildings;

    for (int i = 0; i < Game::Save::Main::BuildingCount; i++)
    {
        char *buffer = new char[sizeof(Game::Building)];
        Memory::readMemory(Game::MainSave + Game::Save::Main::MainFieldStructure + (i * sizeof(Game::Building)), buffer, sizeof(Game::Building));
        buildings.push_back(*reinterpret_cast<Game::Building *>(buffer));
    }

    return buildings;
}

Game::Building Game::getBuilding(int index)
{
    char *buffer = new char[sizeof(Game::Building)];
    Memory::readMemory(Game::MainSave + Game::Save::Main::MainFieldStructure + (index * sizeof(Game::Building)), buffer, sizeof(Game::Building));
    return *reinterpret_cast<Game::Building *>(buffer);
}

void Game::setBuilding(Game::Building building, int index)
{
    Memory::writeMemory(Game::MainSave + Game::Save::Main::MainFieldStructure + (index * sizeof(Game::Building)), &building, sizeof(Game::Building));
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
}

Vector2 *Game::getPlayerPosition()
{
    Vector2 *position = (Vector2 *)malloc(sizeof(Vector2));
    // Memory::readMemory(Game::PlayerPosition, position, sizeof(Vector2));

    Memory::readMemory(Game::PlayerPosition, &position->x, 4);
    Memory::readMemory(Game::PlayerPosition + 4, &position->y, 4);

    return position;
}