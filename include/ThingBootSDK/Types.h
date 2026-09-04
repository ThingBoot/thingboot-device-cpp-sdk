// ThingBoot Device SDK (C++ Host) - 公共类型定义
#ifndef THINGBOOT_SDK_TYPES_H
#define THINGBOOT_SDK_TYPES_H

#include <cstdint>
#include <functional>
#include <string>

// SDK 版本号。唯一来源是仓库根目录的 VERSION 文件，
// 发布前由 build.py 校验同步（不一致即中止构建）。
#define THINGBOOT_SDK_VERSION "0.1.0"

// 系统消息主题，与家族各版本的 SYS_TOPIC 一致
enum SysTopic : int {
    TOPIC_UNKNOWN = 0, // 未知主题
    TOPIC_ORDER,       // 命令
    TOPIC_EVENT,       // 事件
    TOPIC_STATE,       // 状态
    TOPIC_CONFIG,      // 配置
    TOPIC_NTP,         // 时间同步
    TOPIC_OTA,         // 固件升级
    TOPIC_API,         // 开放平台 API
    TOPIC_DEBUG        // 调试
};

// 系统状态，与家族各版本的 SYS_STATE 一致
enum SysState : int {
    STATE_IDLE = 0, // 系统空闲
    STATE_BOOT,     // 启动
    STATE_LOOP,     // 进入主循环
    STATE_ONLINE,   // 在线
    STATE_OFFLINE   // 离线
};

// 子设备记录（网关 addon，开发者可见）
struct ThingBootChild {
    uint32_t id = 0;       // 子设备 ID
    std::string key;       // 子设备键（最长 16 字符，与 MCU 版 key[17] 对齐）
    int64_t active = 0;    // 最后活动时间（毫秒，millis 时基，易失不落盘）
    bool online = false;   // 在线状态（由 SDK 按离线时限自动判定，易失不落盘）
};

// ---- 回调类型 ----
// 所有回调都在 SDK 内部工作线程触发（非网络接收线程），
// 回调内可直接调用 publish / replyMessage 等方法。
// 与 MCU 版有意的 API 差异：JSON 载荷一律以 std::string 传递（原始 JSON 文本）。

// 命令回调：data 为命令数据的原始 JSON 文本（无 data 键时为空串）。
// 应答时将同一个 mid 传给 Order.replyMessage / Message.publishTo，平台据此关联应答。
using OrderCallback = std::function<void(const std::string& mid, const std::string& data)>;

// 配置变化回调：(配置名, 新值)
using ConfigCallback = std::function<void(const std::string& name, const std::string& value)>;

// 系统状态变化回调：(当前状态, 之前状态)
using SystemStateCallback = std::function<void(SysState current, SysState previous)>;

// 系统初始化进度回调：(阶段描述, 进度百分比)
using SystemProgressCallback = std::function<void(const std::string& desc, double progress)>;

// 调试日志回调（文本日志行），默认静默
using DebugCallback = std::function<void(const std::string& line)>;

// 设备状态获取回调：返回全部属性当前值的 JSON 对象文本，如 {"relay1":1}
using FetchCallback = std::function<std::string()>;

// 事件触发回调：(事件名, 值)
using EventCallback = std::function<void(const std::string& name, const std::string& value)>;

// 逐属性周期上报回调（onState 注册），回调内调 State.reportState 完成上报
using StateCallback = std::function<void(const std::string& name)>;

// 用户定时器回调
using TimerCallback = std::function<void()>;

// 子设备命令回调（网关）：平台命令带 device 键且查表命中时，
// 由此回调交给产品的传输层下发；子设备应答用 Message.publishTo(topic, mid, data, id) 上行
using ChildOrderCallback = std::function<void(uint32_t device, SysTopic topic,
    const std::string& mid, const std::string& data)>;

#endif // THINGBOOT_SDK_TYPES_H
