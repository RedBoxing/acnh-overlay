#pragma once

#include <switch.h>
#include <string>

namespace Utils
{
    bool str2u16(const char *str, u16 *out);
    std::string Hex2String(u64 hex);
    std::string Hex2String(void *hex);
}