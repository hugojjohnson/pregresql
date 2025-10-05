#pragma once
#include "Schema.hpp"
#include <string>
#include <variant>
#include <vector>

namespace Statements {

struct CreateTableStmt {
  struct CreateTableField {
    std::string name;
    Schema::FieldType type;
  };
  std::string tableName;
  std::vector<CreateTableField> fields;
};

struct InsertStmt {
  std::string tableName;
  std::vector<int> fields;
};

using StatementVariant = std::variant<CreateTableStmt>;
}