#include <ui/MainApplication.hpp>
#include <ui/views/DebugToolsLayout.hpp>
#include <Game.hpp>
#include <Memory.hpp>
#include <Utils.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

#include <PrivateUtils.hpp>

std::string Hex2String2(u16 hex)
{
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << std::hex << hex;

    return ss.str();
}

std::pair<Game::Item, int> findItem(u16 id)
{
    Game::Item *items = Game::Map::getFieldItems();

    for (int i = 0; i < Game::Constants::Map::MapTileCount; i++)
    {
        if (items[i].id == id)
        {
            return std::make_pair(items[i], i);
        }
    }

    return std::make_pair(Game::Item(), -1);
}

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

tsl::elm::Element *DebugToolsLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", VERSION);

    Game::Item items = Game::Map::getFieldItems()[25464];
    std::pair<Game::Item, int> shovelItem = std::make_pair(items, 25464);

    uintptr_t selectedIndexPtr = Memory::FindDMAAddy(Memory::getBaseAddress() + 0x510F718, {0x78, 0x190, 0x398, 0x90, 0xD0, 0x238, 0x4D});
    u8 selectedIndex = 0;
    Memory::readMemory(selectedIndexPtr, &selectedIndex, 0x1);

    Vector2 *pos = Game::getPlayerPosition();

    rootFrame->setContent(new tsl::elm::CustomDrawer([shovelItem, selectedIndex, selectedIndexPtr, pos](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h)
                                                     {
        renderer->drawString(("TitleID: 0x" + Utils::Hex2String(Memory::getTitleId())).c_str(), false, x, y + 20, 20, renderer->a(0xFFFF));
        renderer->drawString(("Main Address: 0x" + Utils::Hex2String(Memory::getBaseAddress())).c_str(), false, x, y + 45, 20, renderer->a(0xFFFF));
        renderer->drawString(("Main Save Addr: 0x" + Utils::Hex2String(Game::Offsets::MainSave)).c_str(), false, x, y + 70, 20, renderer->a(0xFFFF));
        renderer->drawString(("Personal Save Addr: 0x" + Utils::Hex2String(Game::Offsets::PersonalSave)).c_str(), false, x, y + 95, 20, renderer->a(0xFFFF));
        renderer->drawString(("Inventory Addr: 0x" + Utils::Hex2String(Game::Offsets::Inventory)).c_str(), false, x, y + 120, 20, renderer->a(0xFFFF));
        renderer->drawString(("ChatBuffer Addr: 0x" + Utils::Hex2String(Game::Offsets::ChatBuffer)).c_str(), false, x, y + 145, 20, renderer->a(0xFFFF));

        renderer->drawString(("FieldItem ID: 0x" + Hex2String2(shovelItem.first.id)).c_str(), false, x, y + 195, 20, renderer->a(0xFFFF));
        renderer->drawString(("FieldItem Addr: 0x" + Utils::Hex2String(Game::Offsets::MainSave + Game::Save::Main::FieldItem)).c_str(), false, x, y + 220, 20, renderer->a(0xFFFF));
        renderer->drawString(("FieldItem Index: " + std::to_string(shovelItem.second)).c_str(), false, x, y + 245, 20, renderer->a(0xFFFF));
        renderer->drawString(("systemParam: 0x" + Utils::Hex2String(shovelItem.first.systemParam)).c_str(), false, x, y + 270, 20, renderer->a(0xFFFF));
        renderer->drawString(("additionalParam: 0x" + Utils::Hex2String(shovelItem.first.additionalParam)).c_str(), false, x, y + 295, 20, renderer->a(0xFFFF));
        renderer->drawString(("freeParam: 0x" + Utils::Hex2String(shovelItem.first.freeParam)).c_str(), false, x, y + 320, 20, renderer->a(0xFFFF));
        renderer->drawString(("selectedIndex: " + std::to_string(selectedIndex)).c_str(), false, x, y + 345, 20, renderer->a(0xFFFF));
        renderer->drawString(("selectedIndex Addr: 0x" + Utils::Hex2String(selectedIndexPtr)).c_str(), false, x, y + 370, 20, renderer->a(0xFFFF));
        renderer->drawString(("Tile Index: " + std::to_string(Game::Map::getTileIndex(pos->x, pos->y))).c_str(), false, x, y + 395, 20, renderer->a(0xFFFF));
        renderer->drawString(("Player X: " + std::to_string(pos->x)).c_str(), false, x, y + 420, 20, renderer->a(0xFFFF));
        renderer->drawString(("Player Y: " + std::to_string(pos->y)).c_str(), false, x, y + 445, 20, renderer->a(0xFFFF)); }));

    return rootFrame;
}