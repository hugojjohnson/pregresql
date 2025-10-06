#pragma once
#include "../include/Schema.hpp"
#include <string>
#include <variant>
#include <vector>
#include<iostream>

class Row {
public:
  using FieldValue = std::optional<std::variant<int, float, std::string>>;

  Row() = default;

  // Initialise with values
  Row(const std::vector<FieldValue> &values) : values(values) {}

  // Accessors
  const FieldValue &getField(size_t index) const;
  void setField(size_t index, FieldValue val);

  // Serialize row into bytes (fixed-width)
  std::vector<uint8_t> serialize(const Schema &schema) const;

  // Deserialize bytes into a Row
  static Row deserialize(const Schema &schema, const uint8_t *data);

  static bool validType(const std::vector<Row::FieldValue>& values, Schema schema);
  static std::vector<Row::FieldValue> parse(const std::vector<std::string> &values, Schema schema);
  
  std::string print(const Schema& schema) const;

private:
  std::vector<FieldValue> values;
  std::vector<uint8_t> generateNullBitmap(const Schema &schema) const;
};