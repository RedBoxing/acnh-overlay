#include <ui/MainApplication.hpp>
#include <ui/views/MiscsOptionsLayout.hpp>
#include <ui/views/EatItemEditor.hpp>

tsl::elm::Element *MiscsOptionsLayout::createUI() {
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
    auto list = new tsl::elm::List();

    auto* eatEverythings = new tsl::elm::ToggleListItem("Eat Everythings", MainApplication::isEatEvertingsEnabled());
    eatEverythings->setStateChangedListener([](bool state) {
        MainApplication::toggleEatEverythings();
    });

    list->addItem(eatEverythings);

    auto* elm = new tsl::elm::ListItem("Select Eat Item", "...");
    elm->setClickListener([elm](u64 keys) {
        if (keys & HidNpadButton_A) {
            tsl::changeTo<EatItemEditor>();
            return true;
        }

        return false;
    });
    list->addItem(elm);

    rootFrame->setContent(list);

    return rootFrame;
}