// ThingBoot Device SDK (C++ Host) - 命令处理子模块
#ifndef THINGBOOT_SDK_ORDER_H
#define THINGBOOT_SDK_ORDER_H

#include <cstdint>
#include <string>

#include "Types.h"

class ThingBootDeviceImpl;

// 命令处理，通过 device.Order 访问。
class ThingBootOrder {
public:
    explicit ThingBootOrder(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 设置命令回调。系统命令（system/gateway 键）由 SDK 先行处理，
    // 其余命令数据以原始 JSON 文本传给回调。
    int onOrder(OrderCallback callback);

    // 命令应答。mid 为收到的命令消息 ID，data 为合法 JSON 片段。
    bool replyMessage(const std::string& mid, const std::string& data);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_ORDER_H
