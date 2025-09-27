#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include "Schema.hpp"
#include "Row.hpp"

class StorageManager {
public:
    StorageManager(const std::string& tableName, const Schema& schema);

    // Append a row to the file
    void appendRow(const Row& row);

    // Read a row at a specific index
    Row readRow(size_t index);

    // Number of rows in the file
    size_t numRows() const;

private:
    std::string filename;
    const Schema& schema;       // Table owns the schema
    size_t rowLength;           // Cache row size for offsets

    // Helper: compute row offset
    size_t rowOffset(size_t index) const { return index * rowLength; }
};