#include <ui/views/KeyboardLayout.hpp>
#include "ui/Keyboard.hpp"

KeyboardLayout::KeyboardLayout() : Gui(), title(""), subTitle(""), outText(new std::string()), maxSize(4), onOk(nullptr)
{
}

KeyboardLayout::KeyboardLayout(std::string title, std::string subTitle, std::string *outText, int maxSize, std::function<void()> onOk) : Gui(), title(title), subTitle(subTitle), outText(outText), maxSize(maxSize), onOk(onOk)
{
}

tsl::elm::Element *KeyboardLayout::createUI()
{
    auto rootFrame = new tsl::elm::OverlayFrame(this->title, this->subTitle);
    auto list = new Keyboard(this->outText, this->maxSize, this->onOk);

    rootFrame->setContent(list);
    return rootFrame;
}