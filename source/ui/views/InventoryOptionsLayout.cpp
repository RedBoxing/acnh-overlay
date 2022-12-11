#include <ui/MainApplication.hpp>
#include <ui/views/InventoryOptionsLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <sstream>
#include <ui/views/InventoryEditorLayout.hpp>

bool test2itemEnabled = false;

u16 hexFromString(char* buffer) {
    std::stringstream ss;
    ss << buffer;
    u16 val;
    ss >> val;

    return val;
}

tsl::elm::Element *InventoryOptionsLayout::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
    auto list = new tsl::elm::List();

    auto text2item = new tsl::elm::ListItem("Inventory Editor");
    text2item->setClickListener([](u64 keys) {
        if(keys & HidNpadButton_A) {
            tsl::changeTo<InventoryEditorLayout>();
            return true;
        }

        return false;
    });

    list->addItem(text2item);

    auto test = new tsl::elm::ListItem("Give 2 Money Tree 1st slot");
    test->setClickListener([](u64 keys) {
        if (keys & HidNpadButton_A) {
            u16 item = 0x114A;
            u16 count = 2;
            Memory::writeMemory(Game::Inventory, &item, sizeof(item));
            Memory::writeMemory(Game::Inventory + 0x4, &count, sizeof(count));
            return true;
        }

        return false;
    });

    list->addItem(test);


    rootFrame->setContent(list);

    return rootFrame;
}
