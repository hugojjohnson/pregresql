#pragma once
#include <functional>
#include <string>
#include <vector>
#include "Row.hpp"
#include "Schema.hpp"
#include "StorageManager.hpp"

class Table {
public:
  Table(const std::string &name, const Schema &schema);

  // Insert a row given a vector of values
  void insertRow(const std::vector<Row::FieldValue> &values);

  // Access rows in memory
  const std::vector<Row> &getRows() const { return rows; }
  size_t numRows() const { return rows.size(); }

  // Optional: find rows matching a simple predicate
  std::vector<Row> selectRows(std::function<bool(const Row &)> predicate) const;

private:
  std::string name;
  Schema schema;
  std::vector<Row> rows;                   // in-memory storage of rows for now
  StorageManager storage; // handles file I/O later
};
