// ThingBoot Device SDK (C++ Host) - 公共工具函数
#ifndef THINGBOOT_SDK_UTILS_H
#define THINGBOOT_SDK_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

namespace thingboot {

// MD5 哈希（hex，小写 32 字符）。
// length < 32 时居中截取（与家族 md5(data, len) 一致：
// length=8 即 hex[12:20]，mid 生成同款规则）。
std::string md5(const std::string& data, uint8_t length = 32);

// 按分隔符拆分字符串（最多 maxCount 段，余下部分保留在最后一段），
// 与家族 str_split() 一致。
std::vector<std::string> split(const std::string& str, const std::string& sep,
    size_t maxCount = 0 /* 0 = 不限 */);

} // namespace thingboot

#endif // THINGBOOT_SDK_UTILS_H
