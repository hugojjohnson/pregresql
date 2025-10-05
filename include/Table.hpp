#pragma once
#include "Row.hpp"
#include "Schema.hpp"
#include "StorageManager.hpp"
#include <functional>
#include <map>
#include <string>
#include <vector>

class Table {
public:
  Schema schema;
  
  Table(const std::string &name);

  // Load schema and rows from disk
  void load();

  // Write schema to disk
  void writeSchema() const;

  // Insert a row given a vector of values
  void insertRow(const std::vector<Row::FieldValue> &values);

  // Access rows in memory
  const std::vector<Row> &getRows() const { return rows; }
  size_t numRows() const { return rows.size(); }

  // Optional: find rows matching a simple predicate
  std::vector<Row> selectRows(std::function<bool(const Row &)> predicate) const;

  void setPk(int index);

  friend std::ostream& operator<<(std::ostream& os, Table& t);

private:
  std::string name;
  std::vector<Row> rows;
  std::map<int, int> pk_map;

  StorageManager storage; // handles file I/O
};
