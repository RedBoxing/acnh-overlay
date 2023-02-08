#include <ui/MainApplication.hpp>
#include <ui/views/MiscsOptionsLayout.hpp>
#include "Game.hpp"

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
#endif

    rootFrame->setContent(list);

    return rootFrame;
}