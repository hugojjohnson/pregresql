#include "../include/StorageManager.hpp"
#include "../include/utils.hpp"
#include <filesystem>
#include <stdexcept>

// [HEADER]
//   [header size: 4 bytes] <- including itself!
//   [number of fields: 4 bytes]
//   [row length: 4 bytes]
//   for each field:
//     [field type: 2 bytes]
//     [field max size: 4 bytes]
//     [field name length: 4 bytes]
//     [field name: N bytes]

// [ROWS]
//   repeat:
//     [null bitmap: ceil(numFields / 8) bytes]
//     [field1 value] (if not null, else skipped)
//     [field2 value]

StorageManager::StorageManager(const std::string &tableName, const Schema &schema_) : filename(tableName + ".tbl"), schema(schema_) {
  rowLength = schema.getRowLength();

  // Ensure file exists
  std::ofstream file(filename, std::ios::binary | std::ios::app);
  file.close();
}

void StorageManager::load(std::vector<uint8_t> &header, std::vector<uint8_t> &rows) const {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Couldn't open file.");
  }

  // Read the header size (first 4 bytes)
  uint32_t headerSize = 0;
  file.read(reinterpret_cast<char *>(&headerSize), sizeof(headerSize));
  if (!file) { // Falsey if failed to read 4 bytes
    throw std::runtime_error("Failed to read header size");
  }
  std::cout << "Header size from StorageManager: " << headerSize;

  // Read header
  header.resize(headerSize);
  if (!file.read(reinterpret_cast<char *>(header.data()), headerSize)) {
    throw std::runtime_error("Failed to read header");
  }

  // Read the rest of the file into rows
  // Lazily reads the rest of the file (I'm pretty sure)
  rows.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void StorageManager::writeSchema() const {
  std::ofstream file(filename, std::ios::binary); // Note, you're NOT appending: You're writing the file header again.
  if (!file)
    throw std::runtime_error("Cannot open table file for writing.");
  auto buffer = schema.serialize();
  file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}

void StorageManager::appendRow(const Row &row) {
  std::ofstream file(filename, std::ios::binary | std::ios::app);
  if (!file)
    throw std::runtime_error("Cannot open table file for writing.");

  auto buffer = row.serialize(schema);
  file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}

Row StorageManager::readRow(size_t index) {
  std::ifstream file(filename, std::ios::binary);
  if (!file)
    throw std::runtime_error("Cannot open table file for reading.");

  file.seekg(rowOffset(index));
  std::vector<uint8_t> buffer(rowLength);
  file.read(reinterpret_cast<char *>(buffer.data()), rowLength);

  if (file.gcount() != rowLength)
    throw std::runtime_error("Failed to read complete row.");

  return Row::deserialize(schema, buffer.data());
}

size_t StorageManager::numRows() const {
  if (!std::filesystem::exists(filename))
    return 0;
  return std::filesystem::file_size(filename) / rowLength;
}