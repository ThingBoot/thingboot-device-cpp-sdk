#ifndef THINGBOOT_SDK_API_H
#define THINGBOOT_SDK_API_H

#include <cstdint>
#include <string>
#include "Types.h"

class ThingBootDeviceImpl;

class ThingBootApi {
public:
    explicit ThingBootApi(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}
    int request(const std::string& method, const std::string& url);
    int request(const std::string& method, const std::string& url,
        const std::string& headers, const std::string& body,
        const std::string& contentType = "application/json",
        uint32_t timeoutMs = 10000);
    int onResponse(ApiCallback callback);
    bool pending();

private:
    ThingBootDeviceImpl* _impl;
};

#endif
