// ThingBoot Device SDK (C++ Host) - 消息通信子模块
#ifndef THINGBOOT_SDK_MESSAGE_H
#define THINGBOOT_SDK_MESSAGE_H

#include <cstdint>
#include <string>

#include "Types.h"

class ThingBootDeviceImpl;

// 消息通信，通过 device.Message 访问。
class ThingBootMessage {
public:
    explicit ThingBootMessage(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 生成消息 ID（mid，8 位），与 SDK 内部同规则（md5 居中截 8 位）。
    std::string newMid();

    // 向平台发布消息（event 主题，自动 mid）。
    // data 为合法 JSON 片段；device > 0 时标记消息来源设备（子设备上行，
    // sys_message 带 device 键）。需要指定主题/沿用 mid 时用 publishTo。
    bool publishMessage(const std::string& data, uint32_t device = 0);

    // 全参发布：指定主题与 mid。
    // 子设备应答平台命令时，沿用 onChildOrder 回调给出的 topic 与 mid。
    bool publishTo(SysTopic topic, const std::string& mid, const std::string& data,
        uint32_t device = 0);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_MESSAGE_H
