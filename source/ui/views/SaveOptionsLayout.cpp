#include <ui/MainApplication.hpp>
#include <ui/views/SaveOptionsLayout.hpp>
#include <ui/views/BuildingEditorLayout.hpp>
#include <Memory.hpp>
#include <algorithm>
#include "Utils.hpp"
#include <iterator>

tsl::elm::Element *SaveOptionsLayout::createUI()
{
  auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
  auto list = new tsl::elm::List();

  list->addItem(new tsl::elm::CategoryHeader("Miscs Options", true));
  list->addItem(new tsl::elm::ListItem("Island Hemisphere", "North"));
  list->addItem(new tsl::elm::ListItem("Change Name"));

  Vector2 *plazaPos = Game::Map::getPlazaCordinates();

  auto plazaXItem = new tsl::elm::ListItem("X: ", std::to_string(plazaPos->x));
  list->addItem(plazaXItem);

  auto plazaX = new tsl::elm::TrackBar("\uE13C");
  // percentage
  plazaX->setProgress(plazaPos->x * 0.5);
  plazaX->setValueChangedListener([this, plazaPos, plazaXItem](u8 value)
                                  { plazaPos->x = value / 0.5;
    plazaXItem->setValue(std::to_string(plazaPos->x)); });

  list->addItem(plazaX);

  auto plazaYItem = new tsl::elm::ListItem("Y: ", std::to_string(plazaPos->y));
  list->addItem(plazaYItem);

  auto plazaY = new tsl::elm::TrackBar("\uE13C");
  plazaY->setProgress(plazaPos->y * 0.5);
  plazaY->setValueChangedListener([this, plazaPos, plazaYItem](u8 value)
                                  { plazaPos->y = value / 0.5;
    plazaYItem->setValue(std::to_string(plazaPos->y)); });

  list->addItem(plazaY);

  auto saveBtn = new tsl::elm::ListItem("Save");
  saveBtn->setClickListener([this, plazaPos](u64 keys)
                            {
    if (keys & HidNpadButton_A) {
      Game::Map::setPlazaCordinates(plazaPos);
      return true;
    }

    return false; });

  list->addItem(saveBtn);

  auto test = new tsl::elm::ListItem("Remove All Weeds");
  test->setClickListener([](u64 keys)
                         {
    if (keys & HidNpadButton_A) {
      Game::Item* items = Game::Map::getFieldItems();
      for(int i = 0; i < Game::Constants::Map::MapTileCount; i++) {
        Game::Item item = items[i];
        if (item.id == 0xEB38 || item.id == 0xEB36 || item.id == 0xEB37 || item.id == 0xEB39 || item.id == 0xEB3A || item.id == 0xEB3B || item.id == 0xEB3C)
        {
          item.id = 0xFFFE;
          item.systemParam = 0x00;
          item.additionalParam = 0x00;
          item.freeParam = 0x00;
          Game::Map::writeTile(i, item);
        }
      }
      return true;
    }

    return false; });

  list->addItem(test);

  auto time = new tsl::elm::ListItem("Time", std::to_string(Game::Time::getCurrentTime()).c_str());
  list->addItem(time);

  auto freezeTime = new tsl::elm::ToggleListItem("Freeze Time", Game::Patches::freezeTime->isEnabled());
  freezeTime->setStateChangedListener([](bool enabled)
                                      { Game::Patches::freezeTime->setEnabled(enabled); });

  list->addItem(freezeTime);

  auto incrementTime = new tsl::elm::ListItem("Increment Time");
  incrementTime->setClickListener([](u64 keys)
                                  {
    if (keys & HidNpadButton_A) {
      Game::Time::incrementTime(1);
      return true;
    }

    return false; });

  list->addItem(incrementTime);

  auto decrementTime = new tsl::elm::ListItem("Decrement Time");
  decrementTime->setClickListener([](u64 keys)
                                  {
    if (keys & HidNpadButton_A) {
      Game::Time::decrementTime(1);
      return true;
    }

    return false; });

  list->addItem(decrementTime);

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
    buildingBtn->setClickListener([this, building, it, buildings](u64 keys)
                                  {
      if (keys & HidNpadButton_A) {
        tsl::changeTo<BuildingEditorLayout>(building, it->first, std::distance(buildings.begin(), buildings.find(it->first)));
        return true;
      }

      return false; });

    list->addItem(buildingBtn);
  }

  rootFrame->setContent(list);

  return rootFrame;
}