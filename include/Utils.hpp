#pragma once

#include <switch.h>
#include <string>

namespace Utils {
    bool checkIfServiceIsRunning(const char *serviceName);
    bool ReadUserKeyboard(char* buffer, size_t buffer_size, const char* header_text = "", const char* sub_text = "", const char* guide_text = "", SwkbdType type = SwkbdType_Normal);

    bool str2u16(const char* str, u16* out);
    std::string Hex2String(u64 hex);
    std::string Hex2String(void* hex);
}