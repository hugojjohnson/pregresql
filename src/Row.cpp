#include "../include/Row.hpp"
#include "../include/Schema.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

// Accessors
const Row::FieldValue &Row::getField(size_t index) const { return values[index]; }
void Row::setField(size_t index, FieldValue val) { values[index] = val; }

// Serialize row into bytes (fixed-width)
std::vector<uint8_t> Row::serialize(const Schema &schema) const {
  std::vector<uint8_t> buffer;
  for (size_t i = 0; i < values.size(); ++i) {
    const auto &f = schema.getField(i);
    const auto &val = values[i];

    switch (f.type) {
    case Schema::FieldType::INT: {
      int v = std::get<int>(val);
      uint8_t *p = reinterpret_cast<uint8_t *>(&v);
      buffer.insert(buffer.end(), p, p + sizeof(int));
      break;
    }
    case Schema::FieldType::FLOAT: {
      float v = std::get<float>(val);
      uint8_t *p = reinterpret_cast<uint8_t *>(&v);
      buffer.insert(buffer.end(), p, p + sizeof(float));
      break;
    }
    case Schema::FieldType::STRING: {
      const std::string &s = std::get<std::string>(val);
      uint8_t len = static_cast<uint8_t>(s.size());
      buffer.push_back(len); // actual length
      buffer.insert(buffer.end(), s.begin(), s.end());
      // padding
      buffer.insert(buffer.end(), f.maxStringLength - len, 0);
      break;
    }
    }
  }
  return buffer;
}

// Deserialize bytes into a Row
Row Row::deserialize(const Schema &schema, const uint8_t *data) {
  Row row;
  size_t offset = 0;

  for (size_t i = 0; i < schema.getNumFields(); ++i) {
    const Schema::Field &f = schema.getField(i);
    switch (f.type) {
    case Schema::FieldType::INT: {
      int v = *reinterpret_cast<const int *>(data + offset);
      row.values.push_back(v);
      offset += sizeof(int);
      break;
    }
    case Schema::FieldType::FLOAT: {
      float v = *reinterpret_cast<const float *>(data + offset);
      row.values.push_back(v);
      offset += sizeof(float);
      break;
    }
    case Schema::FieldType::STRING: {
      uint8_t len = *(data + offset);
      offset += 1;
      std::string s(reinterpret_cast<const char *>(data + offset), len);
      row.values.push_back(s);
      offset += f.maxStringLength; // skip fixed-width block
      break;
    }
    }
  }
  return row;
}

std::string Row::print(const Schema &schema) const {
  std::stringstream ss;
  int i = 0;
  // int max = schema.getNumFields();
  ss << "[";
  for (const auto val : values) {
    const auto &fieldType = schema.getField(i).type;
    bool typeMatches = false;
    std::visit(
        [&](auto &&val) {                        // You visit variants.
          using T = std::decay_t<decltype(val)>; // Strip away references, const, etc.. to just get the type.
          ss << (T)val;
          // switch (fieldType) {
          // case Schema::FieldType::INT:
          //   // typeMatches = std::is_same_v<T, int>;
          //   ss << (T)val;
          //   break;
          // case Schema::FieldType::FLOAT:
          //   typeMatches = std::is_same_v<T, float>;
          //   break;
          // case Schema::FieldType::STRING:
          //   typeMatches = std::is_same_v<T, std::string>;
          //   break;
          // }
        },
        val);
  }
  ss << "]\n";
  return ss.str();
}