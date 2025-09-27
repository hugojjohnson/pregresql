#pragma once
#include <string>
#include <vector>

class Schema {
public:
  enum class FieldType { INT, FLOAT, STRING };

  struct Field {
    std::string name;             // Column name
    FieldType type;               // INT, FLOAT, STRING
    size_t maxStringLength = 100; // Only used for STRING
  };

  Schema() = default;

  void addField(const std::string &name, FieldType type, size_t maxLength);

  size_t getNumFields() const;

  const Field &getField(size_t index) const;

  const Field &getField(const std::string &name) const;

  size_t getRowLength() const;

private:
  std::vector<Field> fields;
  std::uint8_t GLOBAL_MAX_STRING_LEN = 255;
};
