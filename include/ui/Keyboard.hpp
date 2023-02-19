#pragma once

#include <tesla.hpp>

class Keyboard : public tsl::elm::List
{
public:
    Keyboard(std::string *outText, int maxSize, std::function<void()> onOk);
    virtual void layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight) override;
    virtual Element *requestFocus(Element *oldFocus, tsl::FocusDirection direction) override;

    std::string *getOutText()
    {
        return this->outText;
    }
    int getMaxSize() { return this->maxSize; }

private:
    std::string *outText;
    int maxSize;
    std::function<void()> onOk;
};

class KeyboardItem : public tsl::elm::ListItem
{
public:
    KeyboardItem(std::string text);
    virtual void draw(tsl::gfx::Renderer *renderer) override;
};