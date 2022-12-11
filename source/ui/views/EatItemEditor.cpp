#include <ui/views/EatItemEditor.hpp>
#include <ui/MainApplication.hpp>
#include <vector>
#include <Utils.hpp>
#include <sstream>

EatItemEditor::EatItemEditor()
{
}

u16 str2u16(const char *str)
{
    u16 x;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;

    return x;
}

tsl::elm::Element *EatItemEditor::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");
    auto list = new tsl::elm::List();

    std::vector<ItemField> items;

    items.push_back({"", "Money Tree", str2u16("114A")});
    items.push_back({"", "Star Fragment", str2u16("175F")});
    items.push_back({"", "Large Star Fragment", str2u16("1760")});
    items.push_back({"", "Gold Roses", str2u16("0B93")});
    items.push_back({"", "Garden Rock", str2u16("02D9")});
    items.push_back({"", "99k Bells", str2u16("08A4")});
    items.push_back({"", "Present", str2u16("1E13")});

    for (auto &item : items)
    {
        auto *elm = new tsl::elm::ListItem(item.name);
        elm->setClickListener([item](u64 keys)
                              {
            if(keys & HidNpadButton_A) {
                MainApplication::setItemToEat(item.id);
                return true;
            }
            return false; });

        list->addItem(elm);
    }

    rootFrame->setContent(list);
    return rootFrame;
}
