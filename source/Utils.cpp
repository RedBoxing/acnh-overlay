#include <Utils.hpp>
#include <iomanip>
#include <iostream>
#include <inttypes.h>

bool Utils::str2u16(const char *str, u16 *out)
{
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *out = (uint16_t)val;
    return true;
}

std::string Utils::Hex2String(u64 hex)
{
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << hex;

    return ss.str();
}

std::string Utils::Hex2String(void *hex)
{
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << hex;

    return ss.str();
}