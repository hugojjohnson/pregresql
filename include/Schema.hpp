#pragma once
#include <string>
#include <vector>

class Schema {
public:
  enum class FieldType { INT = 0, FLOAT = 1, STRING = 2 };

  static FieldType parseFieldType(uint16_t val) {
        switch (val) {
            case 0: return FieldType::INT;
            case 1: return FieldType::FLOAT;
            case 2: return FieldType::STRING;
            default:
                throw std::runtime_error("Invalid FieldType value: " + std::to_string(val));
        }
    }

  struct Field {
    std::string name;             // Column name
    FieldType type;               // INT, FLOAT, STRING
    size_t maxStringLength = 100; // Only used for STRING
  };

  Schema() = default;

  void load(std::vector<uint8_t> data);
  std::vector<uint8_t> serialize() const;

  void addField(const std::string &name, FieldType type, size_t maxLength = 0);

  size_t getNumFields() const;

  const Field &getField(size_t index) const;

  const Field &getField(const std::string &name) const;

  size_t getRowLength() const;

  friend std::ostream& operator<<(std::ostream&os, Schema& s);

private:
  std::vector<Field> fields;
  std::uint8_t GLOBAL_MAX_STRING_LEN = 255;
};
