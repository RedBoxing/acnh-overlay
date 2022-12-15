#include <ui/MainApplication.hpp>
#include <ui/views/DebugToolsLayout.hpp>
#include <Game.hpp>
#include <Memory.hpp>
#include <Utils.hpp>
#include <sstream>
#include <iomanip>
#include <iostream>

std::string Hex2String2(u16 hex)
{
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << std::hex << hex;

    return ss.str();
}

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

tsl::elm::Element *DebugToolsLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame("ACNH Overlay", "v1.0.0 - PRIVATE BETA");

    u16 airportColor = 0;
    u16 itemId = 0x0;
    int itemCount = 0;
    void *buffer;
    void *buffer2;
    int nowpoint = 0;

    Memory::readMemory(Game::MainSave, &buffer, 16);
    Memory::readMemory(Game::MainSave + Game::Save::Main::AirportThemeColor, &airportColor, sizeof(airportColor));
    // Memory::readMemory(Game::Inventory, &itemId, sizeof(itemId));
    // Memory::readMemory(Game::Inventory + 0x4, &itemCount, sizeof(itemCount));
    Memory::readMemory(Game::PersonalSave, &buffer2, 16);
    Memory::readMemory(Game::PersonalSave + Game::Save::Personal::NowPoint, &nowpoint, sizeof(nowpoint));

    Vector2 *pos = Game::getPlayerPosition();

    rootFrame->setContent(new tsl::elm::CustomDrawer([itemId, airportColor, buffer, itemCount, nowpoint, buffer2, pos](tsl::gfx::Renderer *renderer, s32 x, s32 y, s32 w, s32 h)
                                                     {
        renderer->drawString(("TitleID: 0x" + Utils::Hex2String(Memory::getTitleId())).c_str(), false, x, y + 20, 20, renderer->a(0xFFFF));
        renderer->drawString(("Main Address: 0x" + Utils::Hex2String(Memory::getBaseAddress())).c_str(), false, x, y + 45, 20, renderer->a(0xFFFF));
        renderer->drawString(("Main Save Addr: 0x" + Utils::Hex2String(Game::MainSave)).c_str(), false, x, y + 70, 20, renderer->a(0xFFFF));
        renderer->drawString(("Personal Save Addr: 0x" + Utils::Hex2String(Game::PersonalSave)).c_str(), false, x, y + 95, 20, renderer->a(0xFFFF));
        renderer->drawString(("Inventory Addr: 0x" + Utils::Hex2String(Game::Inventory)).c_str(), false, x, y + 120, 20, renderer->a(0xFFFF));
        renderer->drawString(("item: 0x" + Hex2String2(itemId) + " Count: " + std::to_string(itemCount)).c_str(), false, x, y + 145, 20, renderer->a(0xFFFF));
        renderer->drawString(("NowPoint: " + std::to_string(nowpoint)).c_str(), false, x, y + 195, 20, renderer->a(0xFFFF));
        renderer->drawString(("Personal.dat: " + Utils::Hex2String(buffer2)).c_str(), false, x, y + 220, 20, renderer->a(0xFFFF)); 
        renderer->drawString(("X: " + std::to_string(pos->x)).c_str(), false, x, y + 245, 20, renderer->a(0xFFFF));
        renderer->drawString(("Y: " + std::to_string(pos->y)).c_str(), false, x, y + 270, 20, renderer->a(0xFFFF)); }));

    return rootFrame;
}