#include "ui/Keyboard.hpp"

void addKeyboardItem(Keyboard *list, tsl::elm::ListItem *listItem, std::string text)
{
    auto *keyItem = new KeyboardItem(text);
    keyItem->setClickListener([list, text, listItem](u64 keys)
                              { if ((keys & HidNpadButton_A) && list->getOutText()->length() < list->getMaxSize()) {
                                        std::string *out = list->getOutText();
                                        *out += text;
                                        listItem->setText(*out);
                                        return true;
                                    }

                                    return false; });

    list->addItem(keyItem);
}

Keyboard::Keyboard(std::string *outText, int maxSize, std::function<void()> onOk) : List(), outText(outText), maxSize(maxSize), onOk(onOk)
{
    auto *text = new tsl::elm::ListItem(*outText);
    this->addItem(text);

    const char keys[17] = "AD789BE456CF1230";
    for (int i = 0; i < 16; i++)
    {
        addKeyboardItem(this, text, std::string(1, keys[i]));
    }

    auto *keyItem = new KeyboardItem("<-");
    keyItem->setClickListener([this, text](u64 keys)
                              {
            if (keys & HidNpadButton_A)
            {
                std::string *out = this->getOutText();
                if (out->length() > 0)
                {
                    out->pop_back();
                    text->setText(*out);
                    return true;
                }
            }

            return false; });

    this->addItem(keyItem);

    auto *ok = new tsl::elm::ListItem("OK");
    ok->setClickListener([this](u64 keys)
                         {
            if (keys & HidNpadButton_A)
            {
                if (this->onOk)
                {
                    this->onOk();
                }

                 tsl::goBack();
                 return true;
            }

            return false; });
    this->addItem(ok);
}

void Keyboard::layout(u16 parentX, u16 parentY, u16 parentWidth, u16 parentHeight)
{
    // now instead of having one item per row, we have 4 items per row

    s32 x = this->getX() + 5;
    s32 y = this->getY() - this->m_offset;

    this->m_listHeight = (this->m_items.size() / 4) * (tsl::style::ListItemDefaultHeight + 10) + 10;

    for (auto &entry : this->m_items)
    {
        if (((tsl::elm::ListItem *)entry)->getValue() == "KEYBOARD_ITEM")
        {
            entry->setBoundaries(x, y, tsl::style::ListItemDefaultHeight, tsl::style::ListItemDefaultHeight);
            entry->invalidate();
            x += entry->getWidth() + 10;
            if (x >= this->getX() + this->getWidth())
            {
                x = this->getX() + 5;
                y += entry->getHeight() + 10;
            }
        }
        else
        {
            if (x != this->getX() + 5)
            {
                x = this->getX() + 5;
                y += entry->getHeight() + 10;
            }

            entry->setBoundaries(this->getX(), y, this->getWidth(), entry->getHeight());
            entry->invalidate();
            y += entry->getHeight() + 10;
        }
    }
}

tsl::elm::Element *Keyboard::requestFocus(Element *oldFocus, tsl::FocusDirection direction)
{
    Element *newFocus = nullptr;

    if (this->m_clearList || this->m_itemsToAdd.size() > 0)
        return nullptr;

    if (direction == tsl::FocusDirection::None)
    {
        u16 i = 0;

        if (oldFocus == nullptr)
        {
            s32 elementHeight = 0;
            while (elementHeight < this->m_offset && i < this->m_items.size() - 1)
            {
                i++;
                elementHeight += this->m_items[i]->getHeight();
            }
        }

        for (; i < this->m_items.size(); i++)
        {
            newFocus = this->m_items[i]->requestFocus(oldFocus, direction);

            if (newFocus != nullptr)
            {
                this->m_focusedIndex = i;

                this->updateScrollOffset();
                return newFocus;
            }
        }
    }
    else
    {

        /*

        Exemple to select the next item in the list:

        for (u16 i = this->m_focusedIndex + 1; i < this->m_items.size(); i++)
                             {
                                 newFocus = this->m_items[i]->requestFocus(oldFocus, direction);

                                 if (newFocus != nullptr && newFocus != oldFocus)
                                 {
                                     this->m_focusedIndex = i;

                                     this->updateScrollOffset();
                                     return newFocus;
                                 }
                             }*/

        u16 newFocusIndex = this->m_focusedIndex;

        if (direction == tsl::FocusDirection::Down)
        {
            if (this->m_focusedIndex == 0)
            {
                newFocusIndex = 1;
            }
            else if (this->m_focusedIndex >= 16)
                newFocusIndex = 18;
            else
                newFocusIndex += 5;
        }
        else if (direction == tsl::FocusDirection::Up)
        {
            if (this->m_focusedIndex == 18)
                newFocusIndex = 16;
            else if (this->m_focusedIndex <= 5)
                newFocusIndex = 0;
            else
                newFocusIndex -= 5;
        }
        else if (direction == tsl::FocusDirection::Left)
        {
            if (this->m_focusedIndex > 0)
                newFocusIndex -= 1;
        }
        else if (direction == tsl::FocusDirection::Right)
        {
            if (this->m_focusedIndex < 18)
                newFocusIndex += 1;
        }

        for (u16 i = newFocusIndex; i < this->m_items.size(); i++)
        {
            newFocus = this->m_items[i]->requestFocus(oldFocus, direction);

            if (newFocus != nullptr && newFocus != oldFocus)
            {
                this->m_focusedIndex = i;

                this->updateScrollOffset();
                return newFocus;
            }
        }
    }

    return oldFocus;
}

KeyboardItem::KeyboardItem(std::string text) : ListItem(text, "KEYBOARD_ITEM")
{
}

void KeyboardItem::draw(tsl::gfx::Renderer *renderer)
{
    if (this->m_touched && Element::getInputMode() == tsl::InputMode::Touch)
    {
        renderer->drawRect(ELEMENT_BOUNDS(this), a(tsl::style::color::ColorClickAnimation));
    }

    if (this->m_maxWidth == 0)
    {
        this->m_maxWidth = this->getWidth() - 40;
        auto [width, height] = renderer->drawString(this->m_text.c_str(), false, 0, 0, 23, tsl::style::color::ColorTransparent);
        this->m_textWidth = width;
    }

    renderer->drawString(this->m_text.c_str(), false, this->getX() + 20, this->getY() + 45, 23, a(tsl::style::color::ColorText));
}