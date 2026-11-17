// ThingBoot Device SDK (C++ Host) - 错误码
// 与家族各版本逐值一致（ESP-IDF / Arduino / Flutter / Go）。
// 家族惯例：公开方法返回 int 错误码（ERR_UNSUPPORTED=90001 超出 uint16_t），ERR_OK 表示成功。
#ifndef THINGBOOT_SDK_ERRORS_H
#define THINGBOOT_SDK_ERRORS_H

#include <cstdint>

// 成功
constexpr int ERR_OK = 0;

// ---- 系统核心（10xxx）----
constexpr int ERR_SYSTEM_VALUE_NULL = 10011;

// ---- 配置管理（12xxx）----
constexpr int ERR_CONFIG_NAME_EMPTY    = 12001;
constexpr int ERR_CONFIG_MAP_FULL      = 12002;
constexpr int ERR_CONFIG_POS_INVALID   = 12003;
constexpr int ERR_CONFIG_LENGTH_ZERO   = 12004;
constexpr int ERR_CONFIG_NAME_CONFLICT = 12005;
constexpr int ERR_CONFIG_NOT_FOUND     = 12010;
constexpr int ERR_CONFIG_VALUE_NULL    = 12011;
constexpr int ERR_CONFIG_WRITE_FAILED  = 12012;

// ---- 事件管理（14xxx）----
constexpr int ERR_EVENT_NAME_EMPTY    = 14001;
constexpr int ERR_EVENT_CALLBACK_NULL = 14002;
constexpr int ERR_EVENT_NO_SLOT       = 14003;
constexpr int ERR_EVENT_VALUE_NULL    = 14011;
constexpr int ERR_EVENT_NOT_FOUND     = 14020;

// ---- 状态管理（16xxx）----
constexpr int ERR_STATE_NAME_EMPTY    = 16001;
constexpr int ERR_STATE_CALLBACK_NULL = 16002;
constexpr int ERR_STATE_INTERVAL_ZERO = 16003;
constexpr int ERR_STATE_NO_SLOT       = 16004;

// ---- 定时器（17xxx）----
constexpr int ERR_TIMER_INTERVAL_ZERO = 17001;
constexpr int ERR_TIMER_CALLBACK_NULL = 17002;
constexpr int ERR_TIMER_NO_SLOT       = 17003;

// ---- 网络 / 网关（20xxx）----
constexpr int ERR_GATEWAY_DRIVER_MISSING = 20004; // 未 install
constexpr int ERR_GATEWAY_CHILD_FULL     = 20005; // 子设备表满（128）
constexpr int ERR_GATEWAY_CHILD_NOT_FOUND = 20006;

// ---- 其他 ----
// 当前平台不支持该功能（主机版无硬件能力时返回，如 restart / 网络切换）
constexpr int ERR_UNSUPPORTED = 90001;

#endif // THINGBOOT_SDK_ERRORS_H
