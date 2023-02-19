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
    // std::pair<Game::Item, int> shovelItem = findItem(0x0833);

    Vector2 itemPos = Game::Map::getTilePosition(shovelItem.second);
    Vector2 *playerPos = Game::getPlayerPosition();

    char *buf = new char[0x10];
    Memory::readMemory(Game::Private::Offsets::sendBuffer, buf, 0x10);
    std::string str = Utils::utf16ToUtf8((u16 *)buf);

    u32 val = 0;
    Memory::readMemory(Memory::getBaseAddress() + 0x4539888, &val, 4);
    Memory::readMemory(val, &val, 4);

    rootFrame->setContent(new tsl::elm::CustomDrawer([shovelItem, itemPos, playerPos, str, val](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h)
                                                     {
                                                         renderer->drawString(("TitleID: 0x" + Utils::Hex2String(Memory::getTitleId())).c_str(), false, x, y + 20, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Main Address: 0x" + Utils::Hex2String(Memory::getBaseAddress())).c_str(), false, x, y + 45, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Main Save Addr: 0x" + Utils::Hex2String(Game::Offsets::MainSave)).c_str(), false, x, y + 70, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Personal Save Addr: 0x" + Utils::Hex2String(Game::Offsets::PersonalSave)).c_str(), false, x, y + 95, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Inventory Addr: 0x" + Utils::Hex2String(Game::Offsets::Inventory)).c_str(), false, x, y + 120, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Pos addr: 0x" + Utils::Hex2String(Game::Offsets::CameraPosition)).c_str(), false, x, y + 145, 20, renderer->a(0xFFFF));

                                                         renderer->drawString(("FieldItem ID: 0x" + Hex2String2(shovelItem.first.id)).c_str(), false, x, y + 195, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("FieldItem Addr: 0x" + Utils::Hex2String(Game::Offsets::MainSave + Game::Save::Main::FieldItem + (25464 * sizeof(Game::Item)))).c_str(), false, x, y + 220, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("FieldItem Index: " + std::to_string(shovelItem.second)).c_str(), false, x, y + 245, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("systemParam: 0x" + Utils::Hex2String(shovelItem.first.systemParam)).c_str(), false, x, y + 270, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("additionalParam: 0x" + Utils::Hex2String(shovelItem.first.additionalParam)).c_str(), false, x, y + 295, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("freeParam: 0x" + Utils::Hex2String(shovelItem.first.freeParam)).c_str(), false, x, y + 320, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Item X: " + std::to_string(itemPos.x)).c_str(), false, x, y + 345, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Item Z: " + std::to_string(itemPos.y)).c_str(), false, x, y + 370, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Player X: " + std::to_string(playerPos->x)).c_str(), false, x, y + 395, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Player Z: " + std::to_string(playerPos->y)).c_str(), false, x, y + 420, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Buffer: " + str).c_str(), false, x, y + 445, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Buffer Addr: 0x" + Utils::Hex2String(Game::Private::Offsets::sendBuffer)).c_str(), false, x, y + 470, 20, renderer->a(0xFFFF));
                                                         renderer->drawString(("Value: 0x" + Utils::Hex2String(val)).c_str(), false, x, y + 495, 20, renderer->a(0xFFFF)); }));

    return rootFrame;
}