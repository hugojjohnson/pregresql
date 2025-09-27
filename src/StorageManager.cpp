#include "../include/StorageManager.hpp"
#include <filesystem>
#include <stdexcept>

StorageManager::StorageManager(const std::string& tableName, const Schema& schema_)
    : filename(tableName + ".tbl"), schema(schema_)
{
    rowLength = schema.getRowLength();

    // Ensure file exists
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    file.close();
}

void StorageManager::appendRow(const Row& row) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) throw std::runtime_error("Cannot open table file for writing.");

    auto buffer = row.serialize(schema);
    file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

Row StorageManager::readRow(size_t index) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open table file for reading.");

    file.seekg(rowOffset(index));
    std::vector<uint8_t> buffer(rowLength);
    file.read(reinterpret_cast<char*>(buffer.data()), rowLength);

    if (file.gcount() != rowLength)
        throw std::runtime_error("Failed to read complete row.");

    return Row::deserialize(schema, buffer.data());
}

size_t StorageManager::numRows() const {
    if (!std::filesystem::exists(filename)) return 0;
    return std::filesystem::file_size(filename) / rowLength;
}