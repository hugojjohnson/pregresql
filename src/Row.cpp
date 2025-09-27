#include "../include/Row.hpp"
#include <cstring>

Row::Row(const std::vector<std::string>& values) {
    for (const auto& v : values) {
        uint32_t len = static_cast<uint32_t>(v.size());
        data.insert(data.end(),
                    reinterpret_cast<uint8_t*>(&len),
                    reinterpret_cast<uint8_t*>(&len) + sizeof(len));
        data.insert(data.end(), v.begin(), v.end());
    }
}

Row Row::deserialize(const std::vector<std::uint8_t>& buffer, size_t& offset) {
    Row row;
    while (offset < buffer.size()) {
        if (offset + sizeof(uint32_t) > buffer.size()) break;

        uint32_t len;
        std::memcpy(&len, buffer.data() + offset, sizeof(len));
        offset += sizeof(len);

        if (offset + len > buffer.size()) break;

        row.data.insert(row.data.end(), buffer.data() + offset, buffer.data() + offset + sizeof(len) + len);
        offset += len;
    }
    return row;
}

void Row::serializeInto(std::vector<std::uint8_t>& buffer) const {
    buffer.insert(buffer.end(), data.begin(), data.end());
}
