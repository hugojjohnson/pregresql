#include "../include/Schema.hpp"
#include "../include/utils.hpp"
#include <deque>
#include <iomanip> // for std::hex and std::setw
#include <iostream>
#include <string>
#include <vector>

void Schema::addField(const std::string &name, FieldType type, size_t maxLength, bool can_be_null) { fields.push_back({name, type, maxLength, can_be_null}); }

void Schema::load(std::vector<uint8_t> data) {

  utils::ByteReader reader(data.data(), data.size());
  auto num_of_fields = reader.read_le<uint32_t>();

  pkIndex = reader.read_le<uint8_t>();
  pkIndex = 0;

  [[maybe_unused]] auto _ = reader.read_le<uint32_t>(); // Row length: Coming soon.

  // Useful helper function for displaying bytes from a vector<uint8_t>.
  // for (uint8_t byte : data) {
  //   // Cast to int so it prints as a number instead of a char
  //   std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
  // }

  for (int i = 0; i < num_of_fields; i++) {
    uint16_t field_type = reader.read_le<uint16_t>();
    auto field_max_size = reader.read_le<uint32_t>();
    auto can_be_null = reader.read_le<uint8_t>();
    auto field_name_length = reader.read_le<uint32_t>();
    auto name = reader.read_string(field_name_length);
    addField(name, parseFieldType(field_type), field_max_size, can_be_null);
  }
  // std::cout << "Header size: " << header_size;
}

std::vector<uint8_t> Schema::serialize() const {
  std::deque<uint8_t> buffer;

  uint32_t num_fields = getNumFields();
  utils::push_back_bytes(buffer, num_fields);

  uint8_t primary_key_index = pkIndex;
  utils::push_back_bytes(buffer, primary_key_index);

  uint32_t row_length = getRowLength();
  utils::push_back_bytes(buffer, row_length);

  for (size_t i = 0; i < fields.size(); ++i) {
    auto f = fields[i];

    uint16_t field_type = (uint16_t)f.type;
    uint32_t max_size;
    uint8_t can_be_null = f.can_be_null;
    uint32_t field_name_length = f.name.size();
    std::string field_name;
    switch (f.type) {
    case Schema::FieldType::FLOAT:
      max_size = 4;
      break;
    case Schema::FieldType::INT:
      max_size = 4;
      break;
    case Schema::FieldType::STRING:
      max_size = f.maxStringLength;
      break;
    }

    utils::push_back_bytes(buffer, field_type);
    utils::push_back_bytes(buffer, max_size);
    utils::push_back_bytes(buffer, can_be_null);
    utils::push_back_bytes(buffer, field_name_length);
    buffer.insert(buffer.end(), f.name.begin(), f.name.end());
  }

  // int v = std::get<int>(val);
  // uint8_t *p = reinterpret_cast<uint8_t *>(&v);
  // buffer.insert(buffer.end(), p, p + sizeof(int));

  uint32_t header_size = buffer.size() + 4;
  utils::push_front_bytes(buffer, header_size);
  return std::vector<uint8_t>(buffer.begin(), buffer.end());
}

size_t Schema::getNumFields() const { return fields.size(); }

const Schema::Field &Schema::getField(size_t index) const {
  if (index >= fields.size()) {
    throw std::out_of_range("index");
  }
  return fields[index];
}

const Schema::Field &Schema::getField(const std::string &name) const {
  for (auto &f : fields)
    if (f.name == name) {
      return f;
    }
  throw std::out_of_range("field name");
}

void Schema::canBeNull(int index) {
  if (index > getNumFields()) {
    throw std::runtime_error("Index greater than the number of fields.");
  }
  fields[index].can_be_null = true;
}

size_t Schema::getRowLength() const {
  size_t length = 0;
  length += getBitmapLength();
  for (auto &f : fields) {
    switch (f.type) {
    case FieldType::INT:
      length += sizeof(int);
      break;
    case FieldType::FLOAT:
      length += sizeof(float);
      break;
    case FieldType::STRING:
      length += f.maxStringLength + sizeof(GLOBAL_MAX_STRING_LEN); // Stores string length too!
      break;
    }
  }
  return length;
}

size_t Schema::getBitmapLength() const {
  // Number of bytes needed = ceil(num_fields / 8)
  return (getNumFields() + 7) / 8;
}

// Print schema
std::ostream &operator<<(std::ostream &os, Schema &s) {
  int len = s.getNumFields();
  for (auto i = 0; i < len; i++) {
    const auto entry = s.fields[i];
    os << "  " << entry.name << ": ";
    switch (entry.type) {
    case Schema::FieldType::INT:
      os << "INT";
      break;
    case Schema::FieldType::FLOAT:
      os << "FLOAT";
      break;
    case Schema::FieldType::STRING:
      os << "STRING";
      break;
    }
    if (i == s.pkIndex) {
      os << " (PK)";
    }
    if (entry.can_be_null) {
      os << " (NULL)";
    }
    os << "\n";
  }
  return os;
}