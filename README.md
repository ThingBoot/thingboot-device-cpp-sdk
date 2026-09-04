# ThingBoot Device SDK for C++ Host

芯步（ThingBoot）设备 SDK 的 **C++ 主机版**（Linux / Windows / macOS），与
ESP-IDF / Arduino / Flutter / Go 各版本协议逐字节一致，开箱接入 ThingBoot 平台。

- 闭源预编译分发：本仓库（GitHub）只含头文件 + 预编译静态库（`lib/<os>-<arch>/`）+ 示例 + 文档
- 零依赖自包含：不需要 asio/boost/paho 等任何第三方库（MQTT/HTTP/MD5/JSON 均内置）
- C++17，CMake ≥ 3.20，静态库目标名 `thingboot_device`

ThingBoot Device SDK for C++ **host platforms** (Linux / Windows / macOS),
byte-compatible with the ThingBoot device protocol family (ESP-IDF / Arduino /
Flutter / Go). Distributed as prebuilt static libraries + public headers.
Zero external dependencies. C++17, CMake ≥ 3.20.

---

## 快速开始 / Quick Start

```cpp
#include "ThingBootSDK.h"

ThingBootDevice device;
device.onDebug([](const std::string& line) { printf("%s\n", line.c_str()); });
device.setProduct("PRODUCT_KEY", "PRODUCT_SECRET", "my-board", "host", "0.1.0");
device.setActiveCode("ACTIVE_CODE");          // 未预烧录凭证的设备激活用

device.Order.onOrder([](const std::string& mid, const std::string& data) {
    // 处理平台命令；应答沿用同一 mid
    // device.Order.replyMessage(mid, "{\"result\":0}");
});
device.State.onFetch([] { return std::string("{\"relay1\":1}"); });

device.setup();   // 启动后台线程：连接保障 + 事件/状态轮询 + 网关巡检
// ... 主线程继续干活；无 Arduino 式 loop()
device.close();   // 优雅停止
```

完整示例见 `examples/GetStarted/`。

## 构建 / Build

```bash
# 一键：版本宏同步检查 + 编译（库+示例+测试）+ ctest
python build.py

# 或手动 cmake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

消费方（dist）：把 `include/` 加入头文件路径，链接 `lib/<os>-<arch>/thingboot_device.lib`
（或 `libthingboot_device.a`）。Windows 需同时链接 `ws2_32`、`advapi32`，MSVC 需 `/utf-8`
（公开头含 UTF-8 注释；两者在 CMake 下均由库目标自动传播，`examples/GetStarted`
在 dist 包内可直接 `cmake -S . -B build` 独立构建）。

## 与 MCU 版的差异 / Differences from the MCU editions

- **无 loop()**：`setup()` 启动后台线程，`close()` 停止；全部公开方法线程安全，
  回调在 SDK 内部工作线程触发（回调内可调用 publish）
- **错误码为 `int`**：家族 MCU 版是 `uint16_t`，但 `ERR_UNSUPPORTED=90001` 超出
  uint16_t；错误值与家族逐值一致（与 Go 版同为 int）
- **JSON 载荷用 `std::string`**：不移植 MCU 版的 JSONVar 兼容层。`onOrder` 收到的是
  命令数据的原始 JSON 文本；`onFetch` 返回 JSON 对象文本；`reportEvent`/`reportState`/
  `postEvent` 的 data 为合法 JSON 对象文本则按对象内嵌，否则按字符串内嵌
- **严格 JSON**：无单引号伪 JSON 兼容（家族 v1.17.3 起已砍）
- **存储为文件**：`storageDir/storage.json`（系统键 + 设备配置）、`storageDir/gwchild.json`
  （网关子设备表）；默认目录 `thingboot-data/`，`setStorageDir()` 修改
- **网络恒在线**：由操作系统管理；WiFi/以太网/GSM 切换类方法返回 `ERR_UNSUPPORTED`
- **`restart()` 返回 `ERR_UNSUPPORTED`**：主机进程不能自重启
- **无硬件模块**：Peripheral/Bridge 不出现在公开 API；OTA 下行应答
  `{"error":"ota is not support"}`（v0）
- `addConfig(name, defaultValue, hide, group)`：无 EEPROM pos/length，改为默认值
- 定时器 API 为 `Timer.every(ms, cb)`（返回定时器 ID）+ `remove/pause/resume`
- 设备唯一标识：Linux `/etc/machine-id`、macOS `kern.hostuuid`、Windows 注册表
  `MachineGuid`，取不到回退主机名派生（持久化伪 MAC）

## API 速查 / API Reference

| 模块 | 方法 |
|---|---|
| Core | `getVersion` `getSystemInfo` `getDeviceID` `getBenchID`/`setBenchID` `getMasterID`/`setMasterID` `getGatewayID`/`setGatewayID` `getTimezone`/`setTimezone` `getMode`/`setMode` `millis` `millElapsed` `getTimestamp`/`getTimestampMs` `getTimeString` `setProduct` `setActiveCode` `setStorageDir` `onDebug` `onSystemStateChange` `onSystemProgress` `setup` `close` |
| Config | `addConfig(name, default="", hide=false, group="")` `readConfig` `saveConfig` `clearConfig` `resetConfig` `onConfig` |
| Order | `onOrder` `replyMessage(mid, data)` |
| Event | `onEvent(name, cb)` `postEvent(name, value)` `reportEvent(name, data)` |
| State | `onFetch(cb)` `onState(name, intervalSec, cb)` `reportState(name, data)` |
| Message | `newMid()` `publishMessage(data, device=0)` `publishTo(topic, mid, data, device=0)` |
| Timer | `every(intervalMs, cb)` `remove(id)` `pause(id)` `resume(id)` |
| Network | `getNetworkInfo()`（其余返回 `ERR_UNSUPPORTED`） |
| Gateway | `install()` `installed()` `addChild(id, key, active=0)` `findChild(id, &child)` `findChildByKey(key, &child)` `updateChildActive(id)` `childList("all"/"online"/"offline")` `removeChild(id)` `countChildren()` `clearChildren()` `isChildOnline(id)` `setOfflineTimeout(sec)` `onChildOrder(cb)` |

错误码见 `include/ThingBootSDK/Errors.h`（与家族逐值一致，如
`ERR_CONFIG_NAME_CONFLICT=12005`、`ERR_GATEWAY_DRIVER_MISSING=20004`、
`ERR_GATEWAY_CHILD_FULL=20005`、`ERR_GATEWAY_CHILD_NOT_FOUND=20006`、
`ERR_UNSUPPORTED=90001`）。

## 路线图 / Roadmap

- OTA 固件升级（主机程序自更新）
- pvt_gateway / pvt_broker 私有化客户端
- TLS（MQTT/HTTP over TLS）

## 许可 / License

闭源预编译分发。第三方组件：`src/common/third_party/` 内的 nlohmann/json（MIT）
与 RFC 1321 MD5 参考实现，均不进入公开头文件。
