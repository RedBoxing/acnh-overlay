#include <ui/MainApplication.hpp>
#include <ui/views/MiscsOptionsLayout.hpp>
#include <ui/views/KeyboardLayout.hpp>
#include "Game.hpp"
#include "Utils.hpp"

#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>

// using random_bytes_engine = std::independent_bits_engine<
//     std::default_random_engine, CHAR_BIT, unsigned char>;

tsl::elm::Element *MiscsOptionsLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);
    auto list = new tsl::elm::List();

    auto *eatEverythings = new tsl::elm::ToggleListItem("Eat Everythings", Game::Patches::eatEverythings->isEnabled());
    eatEverythings->setStateChangedListener([](bool state)
                                            { Game::Patches::eatEverythings->setEnabled(state); });

    list->addItem(eatEverythings);

#ifdef LIBRED
    auto *onlineSeeder = new tsl::elm::ToggleListItem("Online Seeder", Game::Private::Patches::onlineSeeder->isEnabled());
    onlineSeeder->setStateChangedListener([](bool state)
                                          { Game::Private::Patches::onlineSeeder->setEnabled(state); });

    list->addItem(onlineSeeder);

    auto *chatCorruption = new tsl::elm::ToggleListItem("Chat Corruption", Game::Private::Patches::chatCorruption->isEnabled());
    chatCorruption->setStateChangedListener([](bool state)
                                            { Game::Private::Patches::chatCorruption->setEnabled(state); });

    list->addItem(chatCorruption);

    auto *keepTerraformingTools = new tsl::elm::ToggleListItem("Keep Terraforming Tools", Game::Private::Patches::keepTerraformingTools->isEnabled());
    keepTerraformingTools->setStateChangedListener([](bool state)
                                                   { Game::Private::Patches::keepTerraformingTools->setEnabled(state); });

    list->addItem(keepTerraformingTools);

    auto *giveTerrafomingTools = new tsl::elm::ListItem("Give Terraforming Tools");
    giveTerrafomingTools->setClickListener([](s64 keys)
                                           {
        if (keys & HidNpadButton_A)
        {
            Game::Private::Inventory::giveTerraformingTools();
        }
        return false; });

    list->addItem(giveTerrafomingTools);
#endif

    auto *test = new tsl::elm::ListItem("Test");
    test->setClickListener([](s64 keys)
                           {
        if (keys & HidNpadButton_A)
        {
          //  std::string data = "A";
           /* str.resize(0x1AA, 'A');
            for (int i = 0; i < str.size(); i++)
            {
                u16 utf16Char = str[i];
                Memory::writeMemory(Game::Offsets::ChatBuffer + (i * 2), &utf16Char, sizeof(utf16Char));
            }*/


          //  std::string data = "";
          //  data.resize(0x410, 'A');

          //  random_bytes_engine rbe;
            //std::vector<unsigned char> data(0x10);
           // std::generate(data.begin(), data.end(), std::ref(rbe));

          // data.resize(0x410, 0xFF);

           /*  for (int i = 0; i < data.size(); i++)
             {
                 u16 utf16Char = data[i];
                 Memory::writeMemory(Game::Offsets::ChatBuffer + (i * 2), &utf16Char, sizeof(utf16Char));
                 Memory::writeMemory(Game::Private::Offsets::sendBuffer + (i * 2), &utf16Char, sizeof(utf16Char));
             }*/

           // Memory::writeMemory(Game::Offsets::ChatBuffer, (void *)data.data(), data.size());
           // Memory::writeMemory(Game::Private::Offsets::sendBuffer, (void *)data.data(), data.size());
           Game::Map::writeTile(25464, Game::Item(0x08A4, 0x0, 0x0, 0x8000));
        }
        return false; });

    list->addItem(test);

    rootFrame->setContent(list);

    return rootFrame;
}