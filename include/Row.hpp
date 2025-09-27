#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Row {
    std::vector<std::uint8_t> data;

    Row() = default; // Saying "I want the default constructor". Just initialises to garbage values tho.
    explicit Row(const std::vector<std::string>& values);

    static Row deserialize(const std::vector<std::uint8_t>& buffer, size_t& offset);
    void serializeInto(std::vector<std::uint8_t>& buffer) const; // const -> I'm not gonna modify data.
};
