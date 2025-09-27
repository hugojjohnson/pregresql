#include "Page.hpp"

Page::Page() : buffer(PAGE_SIZE, 0), rowCount(0) {}

bool Page::insertRow(const Row& row) {
    if (row.data.size() + buffer.size() > PAGE_SIZE) {
        return false; // doesn't fit
    }
    std::vector<uint8_t> tmp = buffer; // not efficient but simple
    size_t offset = 0;
    row.serializeInto(tmp);
    buffer = tmp;
    rowCount++;
    return true;
}

std::vector<Row> Page::getRows() const {
    std::vector<Row> rows;
    size_t offset = 0;
    while (offset < buffer.size()) {
        Row row = Row::deserialize(buffer, offset);
        rows.push_back(row);
    }
    return rows;
}
