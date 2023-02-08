#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <switch.h>
#include <map>

#include "Memory.hpp"

#ifdef LIBRED
#include "PrivateGame.hpp"
#endif

struct Vector2
{
    int x;
    int y;
};

namespace Game
{
    namespace Offsets
    {
        extern uintptr_t MainSave;
        extern uintptr_t PersonalSave;
        extern uintptr_t Inventory;
        extern uintptr_t PlayerPosition;
        extern uintptr_t CameraPosition;
        extern uintptr_t ChatBuffer;
    }

    namespace Patches
    {
        extern Memory::Patch *eatEverythings;
    }

    struct Item
    {
        u16 id;
        u8 systemParam;
        u8 additionalParam;
        u32 freeParam;
    };

    namespace Save
    {
        namespace Main
        {
            const int GSaveLandStart = 0x110;
            const int GSaveMainFieldStart = GSaveLandStart + 0x22f3f0;
            const int FieldItem = GSaveMainFieldStart + 0x0000;
            const int LandMakingMap = GSaveMainFieldStart + 0xAAA00;
            const int MainFieldStructure = GSaveMainFieldStart + 0xCF600;
            const int OutsideField = GSaveMainFieldStart + 0xCF998;

            const int GSaveWeather = GSaveLandStart + 0x1e35f0;
            const int WeatherArea = GSaveWeather + 0x14; // Hemisphere
            const int AirportThemeColor = GSaveLandStart + 0x5437c8;
        }

        namespace Personal
        {
            const int GSavePlayerStart = 0x110;
            const int PersonalId = GSavePlayerStart + 0x1e3c00;
            const int GSaveLifeSupport = GSavePlayerStart + 0xBFE0;
            const int NowPoint = GSaveLifeSupport + 0x5498;
        }
    }

    namespace Constants
    {
        namespace Map
        {
            const int AcreWidth = 7;
            const int AcreHeight = 6;
            const int AcreCount = AcreWidth * AcreHeight;
            const int TilesPerAcreDim = 32;
            const int FieldItemWidth = TilesPerAcreDim * AcreWidth;
            const int FieldItemHeight = TilesPerAcreDim * AcreHeight;

            const int MapTileCount = 32 * 32 * AcreCount;
            const int FieldItemLayerSize = MapTileCount * 0x8;

            const int BuildingCount = 48;
        }

        namespace Inventory
        {
            const int InventorySize = 23;
        }
    }

    namespace Map
    {
        int getTileIndex(int x, int y);
        Item getTile(Item *items, int x, int y);
        Item *getFieldItems();
        void writeTile(int index, Item item);
    }

    namespace Inventory
    {
        Item *getItems();
        Item getItem(int index);
        void setItem(Item item, int index);
        int findSlot(u16 id);
    }

    enum class BuildingType : u16
    {
        None = 0,
        PlayerHouse1 = 1,
        PlayerHouse2 = 2,
        PlayerHouse3 = 3,
        PlayerHouse4 = 4,
        PlayerHouse5 = 5,
        PlayerHouse6 = 6,
        PlayerHouse7 = 7,
        PlayerHouse8 = 8,
        Villager1 = 9,
        Villager2 = 10,
        Villager3 = 11,
        Villager4 = 12,
        Villager5 = 13,
        Villager6 = 14,
        Villager7 = 15,
        Villager8 = 16,
        Villager9 = 17,
        Villager10 = 18,
        NooksCranny = 19,
        ResidentCenterStructure = 20,
        Museum = 21,
        Airport = 22,
        ResidentCenterTent = 23,
        AblesSisters = 24,
        Campsite = 25,
        Bridge = 26,
        Incline = 27,
        ReddsTreasureTrawler = 28,
        Studio = 29,
    };

    struct Building
    {
        BuildingType buildingType;
        u16 x;
        u16 y;
        u8 angle;
        s8 bit;
        u16 type;
        u8 typeArg;
        u16 uniqueId;
        u32 unused;
    };

    struct CameraOffsets
    {
        u16 x;
        u16 y;
        u16 yaw;
        u16 pitch;
    };

    void Initialize();
    void Exit();

    std::map<char *, Building *> getBuildings();
    Game::Building *getBuilding(int index);
    void setBuilding(Game::Building *building, int index);

    const char *buildingTypeToString(Game::BuildingType type);

    Vector2 *getPlayerPosition();
    CameraOffsets *getCameraPosition();
}

#endif