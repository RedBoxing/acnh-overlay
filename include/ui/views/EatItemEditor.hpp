#pragma once

#include <tesla.hpp>

struct ItemField
{
    const char *category;
    const char *name;
    u16 id;
};

class EatItemEditor : public tsl::Gui
{
public:
    EatItemEditor();
    virtual tsl::elm::Element *createUI();
};