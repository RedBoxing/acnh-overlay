#include <ui/MainApplication.hpp>
#include <ui/views/InventoryOptionsLayout.hpp>
#include <Utils.hpp>
#include <Memory.hpp>
#include <Game.hpp>
#include <sstream>
#include <ui/views/InventoryEditorLayout.hpp>
#include <ui/views/KeyboardLayout.hpp>

bool test2itemEnabled = false;

u16 hexFromString(char *buffer)
{
    std::stringstream ss;
    ss << std::hex << buffer;
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

    auto *customItemID = new tsl::elm::ListItem("Give Item ");
    customItemID->setClickListener([customItemID](u64 keys)
                                   {
        if (keys & HidNpadButton_A)
        {
            std::string *hexInput = new std::string();
            tsl::changeTo<KeyboardLayout>(std::string("Enter Item ID"), std::string("Ex: 0x0833"), hexInput, 4, [hexInput]() {
                u16 id = hexFromString((char*)hexInput->c_str());
                int slot = Game::Inventory::findSlot(0xFFFE);
                if (slot != -1)
                {
                    Game::Item item2 = {id, 0x20, 0x0, 0x80000};
                    Game::Inventory::setItem(item2, slot);
                }
            });
            return true;
        }

        return false; });

    list->addItem(customItemID);

    list->addItem(new tsl::elm::CategoryHeader("Favorite Items", true));

    std::map<u16, char *> items = {{0x21d4, "Golden Fishing Rod"}, {0x217E, "Golden Shovel"}, {0x1FF3, "Golden Net"}, {0x2591, "Golden Axe"}, {0x2155, "Golden Watering Can"}, {0x0B91, "Blue Rose"}, {0x0B95, "Gold Rose"}, {0x114A, "Money Tree"}, {0x08A4, "99k Bells"}, {0x16DB, "Nook Miles Ticket"}, {0x09C9, "Gold Nugget"}, {0x09CF, "Iron Nugget"}, {0x09C6, "Stone"}, {0x0ACF, "Softwood"}, {0x0AD0, "Wood"}, {0x0AD1, "Hardwood"}};

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
