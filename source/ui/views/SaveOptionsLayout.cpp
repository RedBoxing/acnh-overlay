#include <ui/MainApplication.hpp>
#include <ui/views/SaveOptionsLayout.hpp>
#include <ui/views/BuildingEditorLayout.hpp>
#include <Memory.hpp>
#include <algorithm>

tsl::elm::Element *SaveOptionsLayout::createUI()
{
  auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
  auto list = new tsl::elm::List();

  list->addItem(new tsl::elm::CategoryHeader("Dumping Options", true));
  list->addItem(new tsl::elm::ListItem("Dump Town main.dat", "..."));

  list->addItem(new tsl::elm::CategoryHeader("Restore Options", true));
  list->addItem(new tsl::elm::ListItem("Restore Town main.dat", "..."));

  list->addItem(new tsl::elm::CategoryHeader("Miscs Options", true));
  list->addItem(new tsl::elm::ListItem("Island Hemisphere", "North"));
  list->addItem(new tsl::elm::ListItem("Change Name"));

  list->addItem(new tsl::elm::CategoryHeader("Buildings Editor", true));

  std::vector<Game::Building> buildings = Game::getBuildings();
  auto buildingBtn = new tsl::elm::ListItem("Add Building", "...");
  buildingBtn->setClickListener([this, buildings](u64 keys)
                                {
    if (keys & HidNpadButton_A)
    {
      // find a free building slot in the buildings vector
      int index = std::distance(buildings.begin(), std::find_if(buildings.begin(), buildings.end(), [](Game::Building building)
                                                                { return building.buildingType == Game::BuildingType::None; }));

      if (index == buildings.size())
        return false;

      tsl::changeTo<BuildingEditorLayout>(const_cast<Game::Building*>(&buildings.at(index)), index);
      return true;
    }

    return false; });

  list->addItem(buildingBtn);

  for (int i = 0; i < buildings.size(); i++)
  {
    Game::Building building = buildings[i];
    if (building.buildingType == Game::BuildingType::None)
      continue;

    Game::BuildingType type = building.buildingType;
    std::string name = Game::buildingTypeToString(type);
    auto buildingBtn = new tsl::elm::ListItem(name.c_str(), ("X: " + std::to_string(building.x) + " Y: " + std::to_string(building.y)).c_str());
    buildingBtn->setClickListener([this, building, i](u64 keys)
                                  {
      if (keys & HidNpadButton_A) {
        tsl::changeTo<BuildingEditorLayout>(const_cast<Game::Building*>(&building), i);
        return true;
      }

      return false; });

    list->addItem(buildingBtn);
  }

  rootFrame->setContent(list);

  return rootFrame;
}