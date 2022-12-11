#include <ui/MainApplication.hpp>
#include <ui/views/MainLayout.hpp>
#include <ui/views/SaveOptionsLayout.hpp>
#include <ui/views/DebugToolsLayout.hpp>
#include <ui/views/InventoryOptionsLayout.hpp>
#include <ui/views/MiscsOptionsLayout.hpp>
#include <dmnt/dmntcht.hpp>

template <typename G>
void addListItem(tsl::elm::List *list, const char *name)
{
    auto *elm = new tsl::elm::ListItem(name);
    elm->setClickListener([](u64 keys)
                          {
        if (keys & HidNpadButton_A) {
            tsl::changeTo<G>();
            return true;
        }

        return false; });
    list->addItem(elm);
}

tsl::elm::Element *MainLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
    auto list = new tsl::elm::List();

    addListItem<SaveOptionsLayout>(list, "Save Options");
    list->addItem(new tsl::elm::ListItem("Movement Options"));
    addListItem<InventoryOptionsLayout>(list, "Inventory Options");
    list->addItem(new tsl::elm::ListItem("Player Options"));
    addListItem<MiscsOptionsLayout>(list, "Miscs Options");
    addListItem<DebugToolsLayout>(list, "Debug Tools");

    auto unload = new tsl::elm::ListItem("Unload");
    unload->setClickListener([](u64 keys)
                             {
        if(keys & HidNpadButton_A) {
            tsl::Overlay::get()->close();
            return true;
        }

        return false; });

    list->addItem(unload);

    rootFrame->setContent(list);

    return rootFrame;
}