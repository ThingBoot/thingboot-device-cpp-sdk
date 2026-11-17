// ThingBoot Device SDK (C++ Host) - 网络子模块
#ifndef THINGBOOT_SDK_NETWORK_H
#define THINGBOOT_SDK_NETWORK_H

#include <cstdint>
#include <string>

#include "Errors.h"

class ThingBootDeviceImpl;

// 网络，通过 device.Network 访问。
//
// 主机版联网状态视为恒在线（由操作系统管理），network type 固定为 "wifi"；
// WiFi/以太网/GSM 管理在主机侧无意义，全部返回 ERR_UNSUPPORTED。
class ThingBootNetwork {
public:
    explicit ThingBootNetwork(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 获取当前网络信息（JSON 字符串）：
    // {"type":"wifi","hostname":"...","ips":["192.168.1.10",...]}
    std::string getNetworkInfo();

    // ---- 以下为主机版不支持的网络管理方法（恒返回 ERR_UNSUPPORTED）----
    int connectWiFi(const std::string& ssid, const std::string& psk) { return ERR_UNSUPPORTED; }
    int disconnectWiFi() { return ERR_UNSUPPORTED; }
    int clearWiFiConfig() { return ERR_UNSUPPORTED; }
    int connectEthernet() { return ERR_UNSUPPORTED; }
    int disconnectEthernet() { return ERR_UNSUPPORTED; }
    int connectGSM() { return ERR_UNSUPPORTED; }
    int disconnectGSM() { return ERR_UNSUPPORTED; }
    int installEthernet() { return ERR_UNSUPPORTED; }
    int installGSM() { return ERR_UNSUPPORTED; }

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_NETWORK_H
