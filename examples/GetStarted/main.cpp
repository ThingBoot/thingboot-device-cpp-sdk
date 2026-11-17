// GetStarted - ThingBoot Device SDK (C++ Host) 最小示例
//
// 填入产品密钥与激活码后运行；设备上线后可在平台下发命令，
// 本示例把收到的命令原样应答回去（echo）。
#include <cstdio>
#include <string>

#include "ThingBootSDK.h"

int main() {
    ThingBootDevice device;

    // 调试日志（默认静默）
    device.onDebug([](const std::string& category, const std::string& message) {
        std::printf("[%s] %s\n", category.c_str(), message.c_str());
    });

    // 系统状态变化
    device.onSystemStateChange([](SysState cur, SysState) {
        std::printf("== state: %d ==\n", static_cast<int>(cur));
    });

    // 必填：产品信息（在 ThingBoot 平台创建产品后获得）
    device.setProduct("YOUR_PRODUCT_KEY", "YOUR_PRODUCT_SECRET",
        "host-demo", "host", "0.1.0");
    // 设备未预烧录凭证时的激活码
    device.setActiveCode("YOUR_ACTIVE_CODE");

    // 命令回调：原样应答（沿用同一 mid，平台据此关联命令与应答）
    device.Order.onOrder([](const std::string& mid, const std::string& data) {
        std::printf("order mid=%s data=%s\n", mid.c_str(), data.c_str());
    });

    // 状态获取：上线与系统查询时上报
    device.State.onFetch([] { return std::string("{\"demo\":1}"); });

    // 周期上报示例：每 30s 触发一次（回调内 reportState）
    device.State.onState("demo", 30, [&device](const std::string& name) {
        device.State.reportState(name, "{\"v\":1}");
    });

    if (device.setup() != ERR_OK) {
        std::printf("setup failed\n");
        return 1;
    }

    std::printf("ThingBoot device running, press ENTER to quit\n");
    std::getchar();

    device.close();
    return 0;
}
