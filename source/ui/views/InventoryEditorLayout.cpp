#include <ui/MainApplication.hpp>
#include <ui/views/InventoryEditorLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <sstream>

struct Item {
    u16 id;
    int count;
};

class ItemEditorLayout : public tsl::Gui {
public:
    ItemEditorLayout(Item item, int index) : item(item), index(index) {

    }

    tsl::elm::Element *createUI() {
        auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
        auto list = new tsl::elm::List();

        auto itemidBtn = new tsl::elm::ListItem("Item ID: ", Utils::Hex2String(item.id));
        itemidBtn->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                
                return true;
            }

            return false;
        });

        list->addItem(itemidBtn);

        auto itemCountBtn = new tsl::elm::ListItem("Item Count: ", std::to_string(item.count));
        itemCountBtn->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                
                return true;
            }

            return false;
        });

        list->addItem(itemCountBtn);

        auto itemCount = new tsl::elm::StepTrackBar("\uE13C", 32);
        itemCount->setProgress(item.count);
        itemCount->setValueChangedListener([this](u8 value) {
            Memory::writeMemory(Game::Inventory + (index * 0x8) + 0x4, &value, 0x4);
        });

        list->addItem(itemCount);

        rootFrame->setContent(list);
        return rootFrame;
    }
private:
    Item item;
    int index;
};

tsl::elm::Element *InventoryEditorLayout::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
    auto list = new tsl::elm::List();

    Item items[20];
    Memory::readMemory(Game::Inventory, &items, (20 * 0x8));

    for(int i = 0; i < 20; i++) {
        auto itemEditorBtn = new tsl::elm::ListItem(std::to_string(i));
        itemEditorBtn->setClickListener([items, i](u64 keys) {
            if (keys & HidNpadButton_A) {
                tsl::changeTo<ItemEditorLayout>(items[i], i);
                return true;
            }

            return false;
        });

        list->addItem(itemEditorBtn);
    }
    
    rootFrame->setContent(list);
    return rootFrame;
}