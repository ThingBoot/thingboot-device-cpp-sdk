# ThingBoot Device SDK for C++ Host

芯步设备的 C++ 主机 SDK（Linux / Windows / macOS），面向软网关、上位机和主机应用。公开 API 保留 `ThingBootDevice` 成员对象形式，业务统一由共享 Native Core 提供。

- 闭源预编译分发：公开头 + 单一静态库 + 示例 + 文档。
- C++17、CMake ≥ 3.20；无 asio/boost/paho 等外部链接依赖。
- 支持平台注册、MQTT、NTP、Config/Event/State/Timer/Message/Api、Gateway、私有化 broker/gateway 和 JSON 元数据 OTA。
- ESP 硬件管理、Azure/TLS 不属于当前能力，不以成功桩伪造支持。

## 快速开始

```cpp
#include "ThingBootSDK.h"

ThingBootDevice device;
device.onDebug([](const std::string& category, const std::string& message) {
    printf("[%s] %s\n", category.c_str(), message.c_str());
});
device.setProduct("PRODUCT_KEY", "PRODUCT_SECRET", "my-board", "host", "0.1.0");
device.setActiveCode("ACTIVE_CODE");
device.Order.onOrder([&](const std::string& mid, const std::string& data) {
    device.Order.replyMessage(mid, "{\"result\":0}");
});
device.State.onFetch([] { return std::string("{\"relay1\":1}"); });
device.setup();
// 主线程继续工作；无需 loop()。
device.close();
```

## 构建与消费

```bash
python build.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
```

开发构建默认使用相邻的 `../thingboot-device-native-core`，也可用 `THINGBOOT_NATIVE_CORE` 指定固定源码目录。C++ SDK 与核心版本独立，锁定信息见 `native-core.json`。

`python release.py --skip-build` 将 C++ 薄封装与核心合并为一个静态库并组装 dist 和可复现的 `dist.zip`，不执行发布。消费方把 `include/` 加入头文件路径，链接 `lib/<os>-<arch>/thingboot_device.lib` 或 `libthingboot_device.a`。Windows 还需 `ws2_32`、`advapi32`；dist 示例的 CMake 已处理。

发布分为两个阶段：`python release.py --internal` 只向公司 Gitea 推送内部验证 main、不可覆盖的内部标签和 `internal.json`；内部产物验证后，`python release.py --push --approved <YYYYMMDD.NNN>` 核对其标签与 ZIP SHA-256，再向 GitHub 推送公开 main 和版本标签。公开版本不能与既有版本的不同产物复用。

## 平台差异

- 无公开 `loop()`；`setup()` 启动核心后台任务，`close()` 停止并释放。
- JSON 载荷使用 `std::string`，不移植 MCU 的 JSONVar 层。
- 持久化使用 `storageDir/storage.json` 和 `storageDir/gwchild.json`。
- 网络由 OS 管理，WiFi/以太网/GSM 管理方法返回 `ERR_UNSUPPORTED`。
- 普通 `restart()` 返回 `ERR_UNSUPPORTED`。OTA 仅请求并校验 JSON 元数据，`onOTA` 以单个 `std::string` 原样接收合法 JSON（包括错误响应 JSON）；文件下载、安装、换入和重启由应用自行处理，核心不再执行。
- `setCredentials` 支持量产/许可证预置；`setPrivateConfig` 可在首次 setup 前写入 broker/gateway 配置。
- `addConfig` 支持当前 IDF 形式和旧 `(name, defaultValue, hide, group)` 兼容形式。

## API 速查

| 模块 | 方法 |
|---|---|
| Core | 版本/系统信息、设备与工作台/主设备/网关 ID、时区/模式、时间、产品/激活码/凭证/存储、系统与调试回调、setup/close |
| Config | `addConfig` `readConfig` `saveConfig` `clearConfig` `resetConfig` `onConfig` |
| Order / Message | `onOrder` `replyMessage`；`newMid` `publishMessage` `publishTo` |
| Event / State | 属性变化、防抖与事件上报；`onFetch` `onState` `reportState` |
| Timer | `every` `everyCount` `remove` `pause` `resume` |
| Api | `request` `onResponse` `pending` |
| Gateway | 子设备增删查、扩展字段、上下线、对账、命令与配置回调 |
| OTA | `onOTA` `onRestart` `OTA.begin` `OTA.status` |

错误码见 `include/ThingBootSDK/Errors.h`。许可与第三方组件见发布包对应文件。
