#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Row {
    std::vector<std::uint8_t> data;

    Row() = default;
    explicit Row(const std::vector<std::string>& values);

    static Row deserialize(const std::vector<std::uint8_t>& buffer, size_t& offset);
    void serializeInto(std::vector<std::uint8_t>& buffer) const;
};
