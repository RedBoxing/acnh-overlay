#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <switch.h>
#include <vector>

#define ACNH_TITLE_ID 0x01006F8002326000

#define VERSION_2_0_5 0x15765149DF53BA41

struct Vector2
{
    int x;
    int y;
};

namespace Game
{
    extern uintptr_t MainSave;
    extern uintptr_t PersonalSave;
    extern uintptr_t Inventory;
    extern uintptr_t EatEverythings;
    extern uintptr_t PlayerPosition;

    namespace Save
    {
        namespace Main
        {
            const int GSaveLandStart = 0x110;
            const int GSaveMainFieldStart = GSaveLandStart + 0x22f3f0;
            const int MainFieldStructure = GSaveMainFieldStart + 0xCF600;

            const int GSaveWeather = GSaveLandStart + 0x1e35f0;
            const int WeatherArea = GSaveWeather + 0x14; // Hemisphere
            const int AirportThemeColor = GSaveLandStart + 0x5437c8;

            const int BuildingCount = 48;
        }

        namespace Personal
        {
            const int GSavePlayerStart = 0x110;
            const int PersonalId = GSavePlayerStart + 0x1e3c00;
            const int GSaveLifeSupport = GSavePlayerStart + 0xBFE0;
            const int NowPoint = GSaveLifeSupport + 0x5498;
        }
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

    void Initialize();

    std::vector<Building> getBuildings();
    Game::Building getBuilding(int index);
    void setBuilding(Game::Building building, int index);

    const char *buildingTypeToString(Game::BuildingType type);

    Vector2 *getPlayerPosition();
}

#endif