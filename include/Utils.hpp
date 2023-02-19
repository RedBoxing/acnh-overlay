#pragma once

#include <switch.h>
#include <string>

namespace Utils
{
    bool utf8ToUtf16(const char *str, u16 *out);
    std::string utf16ToUtf8(u16 *str);
    std::string Hex2String(u64 hex);
    std::string Hex2String(void *hex, int length);
}