#include "../include/Table.hpp"
#include "../include/StorageManager.hpp"
#include <iostream>
#include <span>
#include <stdexcept>
#include <vector>

Table::Table(const std::string &name_) : name(name_), schema(), storage("") { storage = StorageManager(name_); }


void Table::load() {
  std::vector<uint8_t> header;
  
  std::vector<uint8_t> read_rows;

  storage.load(header, read_rows);

  schema.load(header);

  if (read_rows.size() % schema.getRowLength() != 0) {
    std::cout << read_rows.size() << " is not a multiple of " << schema.getRowLength();
    throw std::runtime_error("Remaining read_rows should be a multiple of the row length");
  }
  for (int i = 0; i < (int)(read_rows.size() / schema.getRowLength()); i++) {
    Row r = Row::deserialize(schema, read_rows.data() + schema.getRowLength() * i);
    rows.push_back(r);
  }
};

// Write schema to disk
void Table::writeSchema() const { storage.writeSchema(schema); }

void Table::insertRow(const std::vector<Row::FieldValue> &values) {
  if (values.size() != schema.getNumFields()) {
    std::cout << "Error: Row size (" << values.size() << ") does not match schema size (" << schema.getNumFields() << ").\n";
    throw std::runtime_error("Value count does not match schema.");
  }
  if (!Row::validType(values, schema)) {
    throw std::runtime_error("Type of value does not match schema.");
  }

  // pk check
  if (!values[schema.pkIndex]) {
    throw std::runtime_error("PK attribute cannot be null.");
  }
  int pk_value = std::get<int>(values[schema.pkIndex].value()); // Throws a runtime error on bad type
  if (pk_map.contains(pk_value)) {
    throw std::runtime_error("PK already in table.");
  }

  pk_map[pk_value] = rows.size();
  Row row(values);
  rows.push_back(row);
  storage.appendRow(row, schema);
}

std::vector<Row> Table::selectRows(std::function<bool(const Row &)> predicate) const {
  std::vector<Row> result;
  for (const auto &row : rows) {
    if (predicate(row)) {
      result.push_back(row);
    }
  }
  return result;
}

void Table::setPk(int index) {
  if (schema.getNumFields() <= index || index < 0) {
    throw std::runtime_error("ERROR: Index is out of range.");
  }
  if (schema.getField(index).type != Schema::FieldType::INT) {
    throw std::runtime_error("Error: (Currently) index must be type int.");
  }
  if (schema.pkIndex != -1) {
    throw std::runtime_error("ERROR: Can only set pk once as of now.");
  }
  schema.pkIndex = index;
}

std::ostream &operator<<(std::ostream &os, Table &t) {
  os << "Table: " << t.name << "\n";
  os << "Schema:\n";
  os << t.schema;
  os << "Rows:\n";
  for (auto row : t.rows) {
    os << row.print(t.schema);
  }
  return os;
}