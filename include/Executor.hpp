#pragma once
#include "Schema.hpp"
#include "Statements.hpp"
#include "Table.hpp"
#include <map>
#include <string>
#include <vector>

class Executor {
public:
  void execute(const Statements::CreateTableStmt &stmt);
  // More coming soon

private:
  Table *getTable(const std::string &name);
  std::map<std::string, Table> tables;
};