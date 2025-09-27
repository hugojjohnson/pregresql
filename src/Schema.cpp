#include "../include/Schema.hpp"
#include <string>
#include <vector>

void Schema::addField(const std::string &name, FieldType type, size_t maxLength) {
  fields.push_back({name, type, maxLength});
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

size_t Schema::getRowLength() const {
  size_t length = 0;
  for (auto &f : fields) {
    switch (f.type) {
    case FieldType::INT:
      length += sizeof(int);
      break;
    case FieldType::FLOAT:
      length += sizeof(float);
      break;
    case FieldType::STRING:
      length += f.maxStringLength + sizeof(GLOBAL_MAX_STRING_LEN);
      break;
    }
  }
  return length;
}
