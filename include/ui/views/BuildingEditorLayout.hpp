#pragma once

#include <tesla.hpp>
#include <Game.hpp>

class BuildingEditorLayout : public tsl::Gui
{
public:
    BuildingEditorLayout(Game::Building *building, int index) : building(building), index(index)
    {
    }

    virtual tsl::elm::Element *createUI();

private:
    Game::Building *building;
    int index;
};