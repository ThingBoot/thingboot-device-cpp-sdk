// ThingBoot Device SDK (C++ Host) - 公共类型定义
#ifndef THINGBOOT_SDK_TYPES_H
#define THINGBOOT_SDK_TYPES_H

#include <cstdint>
#include <functional>
#include <string>

#define THINGBOOT_SDK_VERSION "0.2.0"

enum SysTopic : int {
    TOPIC_UNKNOWN = 0,
    TOPIC_ORDER,
    TOPIC_EVENT,
    TOPIC_STATE,
    TOPIC_CONFIG,
    TOPIC_NTP,
    TOPIC_OTA,
    TOPIC_API,
    TOPIC_DEBUG
};

enum SysState : int {
    STATE_IDLE = 0,
    STATE_BOOT,
    STATE_LOOP,
    STATE_ONLINE,
    STATE_OFFLINE
};

struct ThingBootChild {
    uint32_t id = 0;
    std::string key;
    int64_t active = 0;
    bool online = false;
    std::string extStr1;
    std::string extStr2;
    uint32_t extVal1 = 0;
    uint32_t extVal2 = 0;
};

using OrderCallback = std::function<void(const std::string& mid, const std::string& data)>;
using ConfigCallback = std::function<void(const std::string& name, const std::string& value)>;
using SystemStateCallback = std::function<void(SysState current, SysState previous)>;
using SystemProgressCallback = std::function<void(const std::string& desc, double progress)>;
using DebugCallback = std::function<void(const std::string& category, const std::string& message)>;
using FetchCallback = std::function<std::string()>;
using EventCallback = std::function<void(const std::string& current, const std::string& previous)>;
using StateCallback = std::function<void(const std::string& name)>;
using TimerCallback = std::function<void()>;
using ChildOrderCallback = std::function<void(uint32_t device, SysTopic topic,
    const std::string& mid, const std::string& data)>;
using ChildConfigCallback = std::function<void(uint32_t device,
    const std::string& mid, const std::string& data)>;
using ApiCallback = std::function<void(int code, int status,
    const std::string& data, const std::string& msg)>;
using OtaCallback = std::function<int(const std::string& data)>;
using RestartCallback = std::function<void()>;

#endif
