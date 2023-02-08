#include <ui/MainApplication.hpp>
#include <ui/views/InventoryEditorLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <sstream>

class ItemEditorLayout : public tsl::Gui
{
public:
    ItemEditorLayout(Game::Item item, int index) : item(item), index(index)
    {
    }

    tsl::elm::Element *createUI()
    {
        auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
        auto list = new tsl::elm::List();

        auto itemidBtn = new tsl::elm::ListItem("Item ID: ", Utils::Hex2String(item.id));
        /* itemidBtn->setClickListener([this](u64 keys)
                                     {
             if (keys & HidNpadButton_A) {
                 char* hexID = new char[0x6];
                 Utils::ReadUserKeyboard(hexID, 0x6, "Enter Item ID", "Enter the item ID in hex", "Example: 0x08FF");
                 item.id = std::stoul(hexID, nullptr, 16);
                 Game::Inventory::setItem(item, index);

                 return true;
             }

             return false; });*/

        list->addItem(itemidBtn);

        auto itemCountBtn = new tsl::elm::ListItem("Item Count: ", std::to_string(item.freeParam + 1));
        /*  itemCountBtn->setClickListener([this](u64 keys)
                                         {
              if (keys & HidNpadButton_A) {
                  char* buf;
                  Utils::ReadUserKeyboard(buf, 0x6, "Enter Item Count", "Enter the item count ", "Example: 10", SwkbdType_NumPad);
                  item.freeParam = std::stoi(buf);
                  Game::Inventory::setItem(item, index);
                  return true;
              }

              return false; });*/

        list->addItem(itemCountBtn);

        auto itemCount = new tsl::elm::StepTrackBar("\uE13C", 32);
        itemCount->setProgress(item.freeParam);
        itemCount->setValueChangedListener([this, itemCountBtn](u8 value)
                                           {
                                               item.freeParam = value;
                                                  Game::Inventory::setItem(item, index);
                                                itemCountBtn->setValue(std::to_string(value + 1)); });

        list->addItem(itemCount);

        rootFrame->setContent(list);
        return rootFrame;
    }

private:
    Game::Item item;
    int index;
};

tsl::elm::Element *InventoryEditorLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
    auto list = new tsl::elm::List();

    Game::Item *items = Game::Inventory::getItems();

    for (int i = 0; i < Game::Constants::Inventory::InventorySize; i++)
    {
        auto itemEditorBtn = new tsl::elm::ListItem(std::to_string(i));
        itemEditorBtn->setClickListener([items, i](u64 keys)
                                        {
            if (keys & HidNpadButton_A) {
                tsl::changeTo<ItemEditorLayout>(items[i], i);
                return true;
            }

            return false; });

        list->addItem(itemEditorBtn);
    }

    rootFrame->setContent(list);
    return rootFrame;
}