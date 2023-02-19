#include <Utils.hpp>
#include <iomanip>
#include <iostream>
#include <inttypes.h>

bool Utils::utf8ToUtf16(const char *str, u16 *out)
{
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *out = (uint16_t)val;
    return true;
}

std::string Utils::utf16ToUtf8(u16 *str)
{
    std::string out;
    while (*str)
    {
        if (*str < 0x80)
        {
            out += (char)*str;
        }
        else if (*str < 0x800)
        {
            out += (char)(0xC0 | (*str >> 6));
            out += (char)(0x80 | (*str & 0x3F));
        }
        else
        {
            out += (char)(0xE0 | (*str >> 12));
            out += (char)(0x80 | ((*str >> 6) & 0x3F));
            out += (char)(0x80 | (*str & 0x3F));
        }
        str++;
    }
    return out;
}

std::string Utils::Hex2String(u64 hex)
{
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << hex;

    return ss.str();
}

std::string Utils::Hex2String(void *hex, int length)
{
    std::stringstream ss;
    ss << std::setw(length) << std::setfill('0') << std::hex << hex;

    return ss.str();
}