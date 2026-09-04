// ThingBoot Device SDK (C++ Host) - 配置管理子模块
#ifndef THINGBOOT_SDK_CONFIG_H
#define THINGBOOT_SDK_CONFIG_H

#include <cstdint>
#include <string>

#include "Types.h"

class ThingBootDeviceImpl;

// 配置管理，通过 device.Config 访问。
// 与 MCU 版的差异：主机版无 EEPROM 布局，addConfig 不需要 pos/length，
// 改为携带默认值；读写走 JSON 文件存储（storageDir/storage.json）。
class ThingBootConfig {
public:
    explicit ThingBootConfig(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 添加设备配置项。defaultValue 为无存储值时的默认值；
    // hide 为 true 时 fetch 全部配置（{"fetch":"device"}）不可见；
    // group 非空时按分组名聚合读取。
    // 与系统配置名冲突返回 ERR_CONFIG_NAME_CONFLICT；已存在为幂等（返回 ERR_OK）。
    int addConfig(const std::string& name, const std::string& defaultValue = "",
        bool hide = false, const std::string& group = "");

    // 按 name 读取设备配置值；无存储值时返回 addConfig 注册的默认值。
    std::string readConfig(const std::string& name);

    // 按 name 保存设备配置值（变更触发 onConfig 回调）。
    int saveConfig(const std::string& name, const std::string& value);

    // 清空所有设备配置项的值。
    int clearConfig();

    // 清空指定设备配置项的值。
    int resetConfig(const std::string& name);

    // 配置变化回调（本机 saveConfig 与平台下行写配置都会触发）。
    int onConfig(ConfigCallback callback);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_CONFIG_H
