// ThingBoot Device SDK (C++ Host) - 网关 addon 子模块
#ifndef THINGBOOT_SDK_GATEWAY_H
#define THINGBOOT_SDK_GATEWAY_H

#include <cstdint>
#include <string>
#include <vector>

#include "Types.h"

class ThingBootDeviceImpl;

// 网关子模块，通过 device.Gateway 访问。
//
// install 即角色：install() 从持久化（storageDir/gwchild.json）加载子设备表，
// setup() 后由 SDK 自动挂上下线巡检（每秒）。子设备表容量 128。
// 变更即上报（event 主题，平台侧判重）：gateway_child_add/remove/clear/online/offline。
// 未 install 时设备表操作方法返回 ERR_GATEWAY_DRIVER_MISSING。
class ThingBootGateway {
public:
    explicit ThingBootGateway(ThingBootDeviceImpl* impl = nullptr) : _impl(impl) {}

    // 安装网关 addon，须在 setup() 前调用。install 即角色。
    int install();

    // 是否已安装网关 addon。
    bool installed();

    // 添加子设备（已存在则刷新活动时间，幂等）。active 传 0 表示当前时间。
    // key 最长 16 字符（与 MCU 版 key[17] 对齐，超出截断）。
    int addChild(uint32_t id, const std::string& key, int64_t active = 0);

    // 查找子设备（按 ID 或按键），找到返回 true 并填充 child。
    bool findChild(uint32_t id, ThingBootChild& child);
    bool findChildByKey(const std::string& key, ThingBootChild& child);

    // 更新子设备活动时间（并置在线）。
    int updateChildActive(uint32_t id);
    int updateChildExt(uint32_t id, const std::string& str1, const std::string& str2,
        uint32_t val1, uint32_t val2);

    // 子设备列表：type 为 "all" / "online" / "offline"，返回 ID 数组。
    std::vector<uint32_t> childList(const std::string& type = "all");

    // 移除子设备（不存在视为成功）。
    int removeChild(uint32_t id);

    // 子设备数量。
    int countChildren();

    // 清空子设备表（恢复出厂/重新配网用）。
    int clearChildren();

    // 判断子设备是否在线（最后活动时间在离线时限内）。
    bool isChildOnline(uint32_t id);

    // 设定离线判定秒数（默认 60，传 0 恢复默认）：超过该秒数无活动判离线。
    int setOfflineTimeout(int64_t seconds);

    // 子设备命令回调：平台命令带 device 键且查表命中时由此回调交给产品的
    // 传输层下发；子设备应答用 device.Message.publishTo(topic, mid, data, id) 上行。
    int onChildOrder(ChildOrderCallback callback);
    int onChildConfig(ChildConfigCallback callback);

private:
    ThingBootDeviceImpl* _impl;
};

#endif // THINGBOOT_SDK_GATEWAY_H
