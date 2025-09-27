#pragma once
#include "Row.hpp"
#include <vector>
#include <cstdint>

struct Page {
    static constexpr size_t PAGE_SIZE = 4096;

    std::vector<std::uint8_t> buffer;
    uint16_t rowCount;

    Page();

    bool insertRow(const Row& row);
    std::vector<Row> getRows() const;
};
