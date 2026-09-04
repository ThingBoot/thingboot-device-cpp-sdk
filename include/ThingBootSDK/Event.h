// ThingBoot Device SDK (C++ Host) - 事件管理子模块
#ifndef THINGBOOT_SDK_EVENT_H
#define THINGBOOT_SDK_EVENT_H

#include <cstdint>
#include <string>

#include "Types.h"

class ThingBootDeviceImpl;

// 事件管理，通过 device.Event 访问。
class ThingBootEvent {
public:
    explicit ThingBootEvent(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 注册命名事件（槽位 20 个，同名重复注册为更新回调）。
    int onEvent(const std::string& name, EventCallback callback);

    // 触发已注册事件并上报 event/client，消息体 {"name":X,"data":Y}。
    // value 为合法 JSON 对象文本则按对象内嵌，否则按字符串内嵌。
    int postEvent(const std::string& name, const std::string& value);

    // 直接上报事件（无需注册），消息体 {"name":X,"data":Y}。
    // data 为合法 JSON 对象文本则按对象内嵌，否则按字符串内嵌。
    bool reportEvent(const std::string& name, const std::string& data);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_EVENT_H
