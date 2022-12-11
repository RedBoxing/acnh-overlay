#include <Utils.hpp>
#include <iomanip>
#include <iostream>
#include <inttypes.h>

bool Utils::checkIfServiceIsRunning(const char* serviceName) {
    Handle handle;
    SmServiceName service_name = smEncodeName(serviceName);
    bool running = R_FAILED(smRegisterService(&handle, service_name, false, 1));

    svcCloseHandle(handle);

    if (!running)
        smUnregisterService(service_name);

    return running;
}

bool Utils::ReadUserKeyboard(char* buffer, size_t buffer_size, const char* header_text, const char* sub_text, const char* guide_text, SwkbdType type) {
    SwkbdConfig kbd;
	Result rc = swkbdCreate(&kbd, 0);

	if (R_SUCCEEDED(rc))
	{
		swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetType(&kbd, type);
        swkbdConfigSetHeaderText(&kbd, header_text);
        swkbdConfigSetSubText(&kbd, sub_text);
        swkbdConfigSetGuideText(&kbd, guide_text);
        swkbdConfigSetStringLenMax(&kbd, buffer_size);

		rc = swkbdShow(&kbd, buffer, sizeof(buffer));

		if(R_FAILED(rc))
			return false;
		
		swkbdClose(&kbd);
	} else {
		return false;
	}

    return true;
}

bool Utils::str2u16(const char* str, u16* out) {
    char *end;
    errno = 0;
    intmax_t val = strtoimax(str, &end, 10);
    if (errno == ERANGE || val < 0 || val > UINT16_MAX || end == str || *end != '\0')
        return false;
    *out = (uint16_t) val;
    return true;
}

std::string Utils::Hex2String(u64 hex) {
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << hex;

    return ss.str();
}

std::string Utils::Hex2String(void* hex) {
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << std::hex << hex;

    return ss.str();
}