#include <ui/MainApplication.hpp>
#include <ui/views/SaveOptionsLayout.hpp>
#include <ui/views/BuildingEditorLayout.hpp>
#include <Memory.hpp>
#include <algorithm>
#include "Utils.hpp"

tsl::elm::Element *SaveOptionsLayout::createUI()
{
  auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
  auto list = new tsl::elm::List();

  list->addItem(new tsl::elm::CategoryHeader("Miscs Options", true));
  list->addItem(new tsl::elm::ListItem("Island Hemisphere", "North"));
  list->addItem(new tsl::elm::ListItem("Change Name"));

  auto test = new tsl::elm::ListItem("Test");
  test->setClickListener([](u64 keys)
                         {
    if (keys & HidNpadButton_A) {
      std::string str = "A";
      str.resize(0x1AA, 'A');
      for(int i = 0; i < str.size(); i++)
      {
        u16 utf16Char = str[i];
        Memory::writeMemory(Game::Offsets::ChatBuffer + (i * 2), &utf16Char, sizeof(utf16Char));
      }

    return true;
    }

    return false; });

  list->addItem(test);

  list->addItem(new tsl::elm::CategoryHeader("Buildings Editor", true));

  std::map<char *, Game::Building *> buildings = Game::getBuildings();
  auto buildingBtn = new tsl::elm::ListItem("Add Building", "...");
  buildingBtn->setClickListener([this, buildings](u64 keys)
                                {
    if (keys & HidNpadButton_A)
    {
      auto building = std::find_if(buildings.begin(), buildings.end(), [](std::pair<char *, Game::Building *> building) { return building.second->buildingType == Game::BuildingType::None; });

      if (building == buildings.end())
        return false;

      tsl::changeTo<BuildingEditorLayout>(building->second, building->first, std::distance(buildings.begin(), building));
      return true;
    }

    return false; });

  list->addItem(buildingBtn);

  for (std::map<char *, Game::Building *>::iterator it = buildings.begin(); it != buildings.end(); ++it)
  {
    Game::Building *building = it->second;
    if (building->buildingType == Game::BuildingType::None)
      continue;

    auto buildingBtn = new tsl::elm::ListItem(it->first, ("X: " + std::to_string(building->x) + " Y: " + std::to_string(building->y)).c_str());
    buildingBtn->setClickListener([this, building, it](u64 keys)
                                  {
      if (keys & HidNpadButton_A) {
        tsl::changeTo<BuildingEditorLayout>(building, it->first, 0);
        return true;
      }

      return false; });

    list->addItem(buildingBtn);
  }

  rootFrame->setContent(list);

  return rootFrame;
}