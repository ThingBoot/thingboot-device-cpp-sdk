// ThingBoot Device SDK (C++ Host) - 主头文件
//
// 芯步（ThingBoot）设备 SDK 主机版（Linux / Windows / macOS）。
// 用法与家族各版本一致：
//
//   ThingBootDevice device;
//   device.setProduct(key, secret, board, mcu, firmware);
//   device.Order.onOrder(...);
//   device.setup();
//   ...
//   device.close();
//
// 无 Arduino 式 loop()：setup() 启动后台线程（连接保障 + 事件/状态轮询 +
// 网关巡检），close() 优雅停止。所有公开方法线程安全；
// 回调在 SDK 内部工作线程触发，回调内可调用 publish 等发布方法。
#ifndef THINGBOOT_SDK_H
#define THINGBOOT_SDK_H

#include <cstdint>
#include <string>

#include "ThingBootSDK/Types.h"
#include "ThingBootSDK/Errors.h"
#include "ThingBootSDK/Utils.h"
#include "ThingBootSDK/Config.h"
#include "ThingBootSDK/Order.h"
#include "ThingBootSDK/Event.h"
#include "ThingBootSDK/Message.h"
#include "ThingBootSDK/State.h"
#include "ThingBootSDK/Timer.h"
#include "ThingBootSDK/Network.h"
#include "ThingBootSDK/Gateway.h"

class ThingBootDeviceImpl;

// 芯步设备主类
class ThingBootDevice {
private:
    // 注意：_impl 必须先于子模块成员声明（构造初始化顺序依赖）。
    ThingBootDeviceImpl* _impl;

public:
    ThingBootDevice();
    ~ThingBootDevice();
    ThingBootDevice(const ThingBootDevice&) = delete;
    ThingBootDevice& operator=(const ThingBootDevice&) = delete;

    // ---- Core 核心接口 ----

    // 获取 SDK 版本号
    std::string getVersion();

    // 获取系统信息（JSON 字符串）
    std::string getSystemInfo();

    // 获取设备 ID（未激活/未注册时为 0）
    uint32_t getDeviceID();

    // 获取/设置工作台 ID
    uint16_t getBenchID();
    int setBenchID(uint16_t benchID);

    // 获取/设置主设备 ID
    uint32_t getMasterID();
    int setMasterID(uint32_t masterDeviceID);

    // 获取/设置网关设备 ID
    uint32_t getGatewayID();
    int setGatewayID(uint32_t gatewayDeviceID);

    // 获取/设置时区（默认 "CST-8"）
    std::string getTimezone();
    int setTimezone(const std::string& timezone);

    // 获取/设置当前运行模式：platform / safe / private / config
    std::string getMode();
    int setMode(const std::string& mode);

    // 重启设备（不支持：主机进程不能自重启，恒返回 ERR_UNSUPPORTED）
    int restart(uint32_t delayMs = 0);

    // 设备启动以来的毫秒数（单调时钟，对应 Arduino millis()）
    int64_t millis();

    // 计算从某个 millis() 时间点到当前时刻经过的毫秒数
    int64_t millElapsed(int64_t since);

    // 获取当前时间戳（秒）；NTP 已同步时用 NTP 标准时间，否则回退本机时钟
    int64_t getTimestamp();

    // 获取当前时间戳（毫秒）
    int64_t getTimestampMs();

    // 获取格式化后的当前时间字符串，支持 %Y %m %d %H %M %S 占位符（本机时区）
    std::string getTimeString(const std::string& format = "%Y-%m-%d %H:%M:%S");

    // 设置产品信息
    int setProduct(const std::string& key, const std::string& secret,
        const std::string& board, const std::string& mcu, const std::string& firmware);

    // 设置设备激活码（用于未预烧录设备时的平台激活）
    int setActiveCode(const std::string& code);

    // 设置持久化目录（系统键、设备配置、网关子设备表），默认 "thingboot-data"。
    // 须在 Gateway.install 与 setup 之前调用。
    int setStorageDir(const std::string& dir);

    // 调试日志回调（文本日志行），默认静默
    int onDebug(DebugCallback callback);

    // 系统状态变化回调
    int onSystemStateChange(SystemStateCallback callback);

    // 系统初始化进度变化回调
    int onSystemProgress(SystemProgressCallback callback);

    // ---- 生命周期 ----

    // 系统初始化：启动后台线程（连接保障 + 事件/状态轮询 + 网关巡检）。
    // 成功后若订阅完成则设备上线（onSystemStateChange 收到 STATE_ONLINE）；
    // 失败时 SDK 按退避策略在后台持续重试。可重复调用，仅首次生效。
    int setup();

    // 释放资源：停止全部后台线程与周期任务，断开连接。析构自动调用。
    void close();

    // ---- 子模块（与家族一致的成员对象形态）----
    ThingBootConfig Config;
    ThingBootOrder Order;
    ThingBootEvent Event;
    ThingBootMessage Message;
    ThingBootState State;
    ThingBootTimer Timer;
    ThingBootNetwork Network;
    ThingBootGateway Gateway;

    // @internal SDK 内部句柄（单元测试用；dist 只含公开头文件，
    // ThingBootDeviceImpl 为不完整类型，dist 用户不可用）。
    ThingBootDeviceImpl* handle() const { return _impl; }
};

#endif // THINGBOOT_SDK_H
