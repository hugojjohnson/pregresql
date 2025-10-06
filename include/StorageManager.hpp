#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "Schema.hpp"
#include "Row.hpp"

class StorageManager {
public:
    StorageManager(const std::string& tableName);

    // load from disk
    void load(std::vector<uint8_t>& header, std::vector<uint8_t>& rows) const;

    // Append a row to the file
    void writeSchema(const Schema& schema) const;

    // Append a row to the file
    void appendRow(const Row& row, const Schema& schema);

    // Read a row at a specific index
    Row readRow(size_t index, const Schema& schema);

    // Number of rows in the file
    size_t numRows(const Schema& schema) const;

private:
    std::string filename;

    // Helper: compute row offset
    size_t rowOffset(size_t index, const Schema& schema) const { return index * schema.getRowLength(); }
};