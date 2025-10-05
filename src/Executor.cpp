#include "../include/Executor.hpp"
#include <string>
#include "../include/Statements.hpp"

using namespace Statements;

Table *Executor::getTable(const std::string &name) {
  auto it = tables.find(name);
  if (it == tables.end())
    return nullptr; // doesn’t exist
  return &it->second;
}

void Executor::execute(const CreateTableStmt& stmt) {
  if (tables.contains(stmt.tableName)) {
    throw std::runtime_error("Table already created.");
  }
  tables.emplace(stmt.tableName, Table(stmt.tableName));
  Table *t = getTable(stmt.tableName);
  for (auto f : stmt.fields) {
    t->schema.addField(f.name, f.type);
  }
}

