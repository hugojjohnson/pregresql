#pragma once
#include "Schema.hpp"
#include "Statements.hpp"
#include "Table.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Dumb "parser"
class Parser {
public:
  explicit Parser() {}

  Statements::StatementVariant parse(const std::string &input);
  friend bool test_tokeniser();

  Statements::CreateTableStmt parseCreateTable();
  Statements::InsertStmt parseInsert();
  // More to come soon!


private:
  std::vector<std::string> tokens;
  size_t pos = 0;

  void tokenize(const std::string &input);
  std::string peek() const;
  std::string next();
  void expect(const std::string &expected);
};