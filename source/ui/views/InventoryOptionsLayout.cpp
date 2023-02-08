#include <ui/MainApplication.hpp>
#include <ui/views/InventoryOptionsLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <sstream>
#include <ui/views/InventoryEditorLayout.hpp>

bool test2itemEnabled = false;

u16 hexFromString(char *buffer)
{
    std::stringstream ss;
    ss << buffer;
    u16 val;
    ss >> val;

    return val;
}

tsl::elm::Element *InventoryOptionsLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
    auto list = new tsl::elm::List();

    auto text2item = new tsl::elm::ListItem("Inventory Editor");
    text2item->setClickListener([](u64 keys)
                                {
        if(keys & HidNpadButton_A) {
            tsl::changeTo<InventoryEditorLayout>();
            return true;
        }

        return false; });

    list->addItem(text2item);

    std::map<u16, char *> items = {{0x08A4, "99k Bells"}, {0x16DB, "Nook Miles Ticket"}, {0x09C9, "Gold Nugget"}, {0x09CF, "Iron Nugget"}, {0x09C6, "Stone"}, {0x0ACF, "Softwood"}, {0x0AD0, "Wood"}, {0x0AD1, "Hardwood"}};

    for (auto const &item : items)
    {
        auto test = new tsl::elm::ListItem(item.second);
        test->setClickListener([item](u64 keys)
                               {
            if (keys & HidNpadButton_A)
            {
                int slot = Game::Inventory::findSlot(0xFFFE);
                if (slot != -1)
                {
                    Game::Item item2 = {item.first, 0x20, 0x0, 0x80000};
                    Game::Inventory::setItem(item2, slot);
                }
                return true;
            }

            return false; });

        list->addItem(test);
    }

    rootFrame->setContent(list);

    return rootFrame;
}
