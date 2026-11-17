// ThingBoot Device SDK (C++ Host) - 状态管理子模块
#ifndef THINGBOOT_SDK_STATE_H
#define THINGBOOT_SDK_STATE_H

#include <cstdint>
#include <string>

#include "Types.h"

class ThingBootDeviceImpl;

// 状态管理，通过 device.State 访问。
class ThingBootState {
public:
    explicit ThingBootState(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 注册设备状态获取回调。设备每次 MQTT 上线后，SDK 通过该回调获取
    // 全部属性当前值（JSON 对象文本）并随 online 消息上报平台；
    // 系统命令 {"system":"state"} 的应答同样走该回调。
    int onFetch(FetchCallback callback);

    // 注册逐属性周期上报（槽位 20 个，同名重复注册为更新）。
    // 每 intervalSec 秒（在线时）触发 callback(name)，回调内调 reportState 完成上报。
    int onState(const std::string& name, uint32_t intervalSec, StateCallback callback);

    // 立即上报某个状态，消息体 {"name":X,"data":Y}。
    // data 为合法 JSON 对象文本则按对象内嵌，否则按字符串内嵌。
    bool reportState(const std::string& name, const std::string& data);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_STATE_H
