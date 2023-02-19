#pragma once

#include <tesla.hpp>

class KeyboardLayout : public tsl::Gui
{
public:
    KeyboardLayout();
    KeyboardLayout(std::string title, std::string subTitle, std::string *outText, int maxSize, std::function<void()> onOk);
    virtual tsl::elm::Element *createUI();

private:
    std::string title;
    std::string subTitle;
    std::string *outText;
    int maxSize;
    std::function<void()> onOk;
};