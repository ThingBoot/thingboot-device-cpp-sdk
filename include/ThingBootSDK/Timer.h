// ThingBoot Device SDK (C++ Host) - 定时器子模块
#ifndef THINGBOOT_SDK_TIMER_H
#define THINGBOOT_SDK_TIMER_H

#include <cstdint>

#include "Types.h"

class ThingBootDeviceImpl;

// 定时器，通过 device.Timer 访问（槽位 20 个）。
class ThingBootTimer {
public:
    explicit ThingBootTimer(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 注册周期任务。intervalMs 为运行间隔（毫秒）。
    // 返回定时器 ID（>0）供 remove/pause/resume 使用；0 表示失败
    // （间隔为 0 / 回调为空 / 无空闲槽位）。
    int every(uint32_t intervalMs, TimerCallback callback);

    // 移除定时器。
    int remove(int id);

    // 暂停定时器（槽位保留，不再触发）。
    int pause(int id);

    // 恢复已暂停的定时器。
    int resume(int id);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_TIMER_H
