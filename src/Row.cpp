#include "../include/Row.hpp"
#include "../include/Schema.hpp"
#include "../include/utils.hpp"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

// Accessors
const Row::FieldValue &Row::getField(size_t index) const { return values[index]; }
void Row::setField(size_t index, FieldValue val) { values[index] = val; }

// Helper function for serialize.
std::vector<uint8_t> Row::generateNullBitmap(const Schema &schema) const {
  size_t numFields = schema.getNumFields();
  size_t bitmapSize = schema.getBitmapLength();
  std::vector<uint8_t> bitmap(bitmapSize, 0);
  for (size_t i = 0; i < numFields; ++i) {
    if (!values[i]) { // i.e. it's null
      size_t byteIndex = i / 8;
      size_t bitIndex = i % 8;
      bitmap[byteIndex] |= (1u << bitIndex);
    }
  }
  return bitmap;
}
std::vector<bool> decodeNullBitmap(const std::vector<uint8_t> &bitmap, size_t numFields) {
  std::vector<bool> result(numFields, false);
  for (size_t i = 0; i < numFields; ++i) {
    size_t byteIndex = i / 8;
    size_t bitIndex = i % 8;
    result[i] = (bitmap[byteIndex] >> bitIndex) & 0x1;
  }
  return result;
}

// Serialize row into bytes (fixed-width)
std::vector<uint8_t> Row::serialize(const Schema &schema) const {
  std::vector<uint8_t> buffer;

  std::vector<uint8_t> null_bitmap = generateNullBitmap(schema);
  buffer.insert(buffer.end(), null_bitmap.begin(), null_bitmap.end());

  for (size_t i = 0; i < values.size(); ++i) {
    const auto &f = schema.getField(i);
    const auto &val = values[i];

    if (!val) {
      switch (f.type) {
      case Schema::FieldType::INT: {
        buffer.insert(buffer.end(), sizeof(int), 0);
        break;
      }
      case Schema::FieldType::FLOAT: {
        buffer.insert(buffer.end(), sizeof(float), 0);
        break;
      }
      case Schema::FieldType::STRING: {
        uint8_t len = 0;
        buffer.push_back(len); // Still need to push the length
        buffer.insert(buffer.end(), f.maxStringLength, 0);
        break;
      }
      }
      continue;
    }

    switch (f.type) {
    case Schema::FieldType::INT: {
      int v = std::get<int>(val.value());
      uint8_t *p = reinterpret_cast<uint8_t *>(&v);
      buffer.insert(buffer.end(), p, p + sizeof(int)); // TODO: Use new utils functions.
      break;
    }
    case Schema::FieldType::FLOAT: {
      float v = std::get<float>(val.value());
      uint8_t *p = reinterpret_cast<uint8_t *>(&v);
      buffer.insert(buffer.end(), p, p + sizeof(float));
      break;
    }
    case Schema::FieldType::STRING: {
      const std::string &s = std::get<std::string>(val.value());
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

  // Null bitmaps
  int bitmapSize = schema.getBitmapLength();
  std::vector<uint8_t> null_bitmap(data, data + bitmapSize);
  offset += bitmapSize;
  std::vector<bool> nulls = decodeNullBitmap(null_bitmap, schema.getNumFields());

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
    if (nulls[i]) {
      row.values[row.values.size() - 1] = std::nullopt;
    }
  }
  return row;
}

bool Row::validType(const std::vector<Row::FieldValue> &values, Schema schema) {
  for (size_t i = 0; i < values.size(); ++i) {
    const auto &fieldType = schema.getField(i).type;
    const auto &canBeNull = schema.getField(i).can_be_null;
    if (!values[i] && !canBeNull) {
      return false;
      // throw std::runtime_error("One of the values cannot be null.");
    }
    if (!values[i]) {
      continue; // Can be null - and is null!
    }
    auto trueVal = values[i].value();
    bool typeMatches = false;
    std::visit(
        [&](auto &&val) {                        // You visit variants.
          using T = std::decay_t<decltype(val)>; // Strip away references, const, etc.. to just get the type.
          switch (fieldType) {
          case Schema::FieldType::INT:
            typeMatches = std::is_same_v<T, int>;
            break;
          case Schema::FieldType::FLOAT:
            typeMatches = std::is_same_v<T, float>;
            break;
          case Schema::FieldType::STRING:
            typeMatches = std::is_same_v<T, std::string>;
            break;
          }
        },
        trueVal);
    if (!typeMatches) {
      return false;
    }
  }
  return true;
}

// Parses a vector of strings into a set that actually matches the schema.
std::vector<Row::FieldValue> Row::parse(const std::vector<std::string> &values, Schema schema) {
  if (values.size() != schema.getNumFields()) {
    std::ostringstream oss;
    oss << "Number of values to insert, " << values.size();
    oss << ", does not match the number of values in the schema, " << schema.getNumFields() << ".\n";
    throw std::runtime_error(oss.str());
  }

  std::vector<Row::FieldValue> output_values;
  for (int i = 0; i < values.size(); i++) {
    Schema::Field f = schema.getField(i);
    std::string v = values[i];
    // deal with null
    if (f.can_be_null && values[i] == "NULL") {
      output_values.push_back(std::nullopt);
      continue;
    }

    // cast to relevant type
    // For each type, check if it is valid and add it. Throw error if it cannot be parsed.
    size_t idx;
    int value_i;
    float value_f;
    switch (f.type) {
    case Schema::FieldType::INT:
      value_i = std::stoi(v, &idx);
      if (idx != v.size()) {
        throw std::invalid_argument("Extra junk after number");
      }
      output_values.push_back(value_i);
      break;
    case Schema::FieldType::FLOAT:
      value_f = std::stof(v, &idx);
      if (idx != v.size()) {
        throw std::invalid_argument("Extra junk after number");
      }
      output_values.push_back(value_f);
      break;
    case Schema::FieldType::STRING:
      output_values.push_back(v);
      break;
    }
  }
  return output_values;
}

std::string Row::print(const Schema &schema) const {
  std::stringstream ss;
  int i = 0;
  // int max = schema.getNumFields();
  ss << "[";
  for (auto it = values.begin(); it != values.end(); it++) {
    const auto &val = *it;
    const auto &fieldType = schema.getField(i).type;
    if (!val) {
      ss << "NULL";
    } else {
      std::visit(
          [&](auto &&val) {                        // You visit variants.
            using T = std::decay_t<decltype(val)>; // Strip away references, const, etc.. to just get the type.
            ss << (T)val;
          },
          val.value());
    }
    if (it != std::prev(values.end())) {
      ss << ", ";
    }
  }
  ss << "]\n";
  return ss.str();
}