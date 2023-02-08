#include <ui/MainApplication.hpp>
#include <ui/views/BuildingEditorLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <sstream>

tsl::elm::ListItem *buildingIdBtn;

class BuildingTypeLayout : public tsl::Gui
{
public:
    BuildingTypeLayout(Game::Building *building) : building(building)
    {
    }

    tsl::elm::Element *createUI()
    {
        auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
        auto list = new tsl::elm::List();

        for (u16 i = 0; i < 30; i++)
        {
            std::string name = Game::buildingTypeToString((Game::BuildingType)i);
            auto item = new tsl::elm::ListItem(name.c_str());
            item->setClickListener([this, i](u64 keys)
                                   {
                if (keys & HidNpadButton_A) {
                    building->buildingType = (Game::BuildingType)i;
                    buildingIdBtn->setValue(std::string(Game::buildingTypeToString(building->buildingType)));
                    tsl::goBack();
                    return true;
                }
            
                return false; });

            list->addItem(item);
        }

        rootFrame->setContent(list);

        return rootFrame;
    }

private:
    Game::Building *building;
};

tsl::elm::Element *BuildingEditorLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
    auto list = new tsl::elm::List();

    list->addItem(new tsl::elm::CategoryHeader("Building", true));
    buildingIdBtn = new tsl::elm::ListItem("Building Type: ", name);
    buildingIdBtn->setClickListener([this](u64 keys)
                                    {
            if (keys & HidNpadButton_A) {
                tsl::changeTo<BuildingTypeLayout>(building);
                return true;
            }

            return false; });

    list->addItem(buildingIdBtn);

    list->addItem(new tsl::elm::CategoryHeader("Position", true));

    auto buildingXItem = new tsl::elm::ListItem("X: ", std::to_string(building->x));
    list->addItem(buildingXItem);

    auto buildingX = new tsl::elm::TrackBar("\uE13C");
    // percentage
    buildingX->setProgress(building->x * 0.1);
    buildingX->setValueChangedListener([this, buildingXItem](u8 value)
                                       { building->x = value / 0.1;
                                            buildingXItem->setValue(std::to_string(building->x)); });

    list->addItem(buildingX);

    auto buildingYItem = new tsl::elm::ListItem("Y: ", std::to_string(building->y));
    list->addItem(buildingYItem);

    auto buildingY = new tsl::elm::TrackBar("\uE13C");
    buildingY->setProgress(building->y * 0.1);
    buildingY->setValueChangedListener([this, buildingYItem](u8 value)
                                       { building->y = value / 0.1;
                                            buildingYItem->setValue(std::to_string(building->y)); });

    list->addItem(buildingY);

    auto setToPlayerBtn = new tsl::elm::ListItem("Set to Player");
    setToPlayerBtn->setClickListener([this, buildingX, buildingY, buildingXItem, buildingYItem](u64 keys)
                                     {
            if (keys & HidNpadButton_A) {
                Vector2* playerPos = Game::getPlayerPosition();
                building->x = playerPos->x;
                building->y = playerPos->y;
                buildingX->setProgress(building->x * 0.1);
                buildingY->setProgress(building->y * 0.1);
                buildingXItem->setValue(std::to_string(building->x));
                buildingYItem->setValue(std::to_string(building->y));
                return true;
            }

            return false; });

    list->addItem(setToPlayerBtn);

    list->addItem(new tsl::elm::CategoryHeader("Rotation", true));

    auto buildingRotation = new tsl::elm::StepTrackBar("\uE13C", 255);
    buildingRotation->setProgress(building->angle);
    buildingRotation->setValueChangedListener([this](u8 value)
                                              { building->angle = value; });

    list->addItem(buildingRotation);

    auto saveBtn = new tsl::elm::ListItem("Save");
    saveBtn->setClickListener([this](u64 keys)
                              {
            if (keys & HidNpadButton_A) {
                Game::setBuilding(building, index);
                return true;
            }

            return false; });

    list->addItem(saveBtn);

    auto deleteBtn = new tsl::elm::ListItem("Delete");
    deleteBtn->setClickListener([this](u64 keys)
                                {
            if (keys & HidNpadButton_A) {
                building->buildingType = Game::BuildingType::None;
                building->x = 0;
                building->y = 0;
                building->angle = 0;
                Game::setBuilding(building, index);
                tsl::goBack();
                return true;
            }

            return false; });

    list->addItem(deleteBtn);

    rootFrame->setContent(list);
    return rootFrame;
}
