#ifndef THINGBOOT_SDK_OTA_H
#define THINGBOOT_SDK_OTA_H

#include <string>

class ThingBootDeviceImpl;

struct OtaStatus {
    std::string stage;
    std::string target;
    std::string error;
};

class ThingBootOta {
public:
    explicit ThingBootOta(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}
    int begin(const std::string& name);
    OtaStatus status();

private:
    ThingBootDeviceImpl* _impl;
};

#endif
