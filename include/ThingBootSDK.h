// ThingBoot Device SDK (C++ Host) - 共享 Native Core 的公开薄封装
#ifndef THINGBOOT_SDK_H
#define THINGBOOT_SDK_H

#include <cstdint>
#include <string>

#include "ThingBootSDK/Types.h"
#include "ThingBootSDK/Errors.h"
#include "ThingBootSDK/Utils.h"
#include "ThingBootSDK/Config.h"
#include "ThingBootSDK/Api.h"
#include "ThingBootSDK/Ota.h"
#include "ThingBootSDK/Order.h"
#include "ThingBootSDK/Event.h"
#include "ThingBootSDK/Message.h"
#include "ThingBootSDK/State.h"
#include "ThingBootSDK/Timer.h"
#include "ThingBootSDK/Network.h"
#include "ThingBootSDK/Gateway.h"

class ThingBootDeviceImpl;

class ThingBootDevice {
private:
    ThingBootDeviceImpl* _impl;

public:
    ThingBootDevice();
    ~ThingBootDevice();
    ThingBootDevice(const ThingBootDevice&) = delete;
    ThingBootDevice& operator=(const ThingBootDevice&) = delete;

    std::string getVersion();
    std::string getSystemInfo();
    uint32_t getDeviceID();
    uint16_t getBenchID();
    int setBenchID(uint16_t benchID);
    uint32_t getMasterID();
    int setMasterID(uint32_t masterDeviceID);
    uint32_t getGatewayID();
    int setGatewayID(uint32_t gatewayDeviceID);
    std::string getTimezone();
    int setTimezone(const std::string& timezone);
    std::string getMode();
    int setMode(const std::string& mode);
    int restart(uint32_t delayMs = 0);
    int64_t millis();
    int64_t millElapsed(int64_t since);
    int64_t getTimestamp();
    int64_t getTimestampMs();
    std::string getTimeString(const std::string& format = "%Y-%m-%d %H:%M:%S");
    int setProduct(const std::string& key, const std::string& secret,
        const std::string& board, const std::string& mcu, const std::string& firmware);
    int setActiveCode(const std::string& code);
    int setStorageDir(const std::string& dir);
    int setCredentials(uint32_t id, const std::string& key, const std::string& secret);
    int setPrivateConfig(const std::string& kind, const std::string& info,
        const std::string& userInfo = "");
    int onDebug(DebugCallback callback);
    int onSystemStateChange(SystemStateCallback callback);
    int onSystemProgress(SystemProgressCallback callback);
    int onOTA(OtaCallback callback);
    int onRestart(RestartCallback callback);
    int setup();
    void close();

    ThingBootConfig Config;
    ThingBootApi Api;
    ThingBootOrder Order;
    ThingBootEvent Event;
    ThingBootMessage Message;
    ThingBootState State;
    ThingBootTimer Timer;
    ThingBootNetwork Network;
    ThingBootGateway Gateway;
    ThingBootOta OTA;

    ThingBootDeviceImpl* handle() const { return _impl; }
};

#endif
