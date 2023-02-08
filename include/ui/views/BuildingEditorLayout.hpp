#pragma once

#include <tesla.hpp>
#include <Game.hpp>

class BuildingEditorLayout : public tsl::Gui
{
public:
    BuildingEditorLayout(Game::Building *building, char *name, int index) : building(building), name(name), index(index)
    {
    }

    virtual tsl::elm::Element *createUI();

private:
    Game::Building *building;
    char *name;
    int index;
};