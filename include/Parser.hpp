#pragma once
#include "Table.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "Schema.hpp"

// Dumb "parser"
class Parser {
public:
  explicit Parser(const std::string &input) { tokenize(input); }

  Table parseCreateTable(Schema& schema);

private:
  std::vector<std::string> tokens;
  size_t pos = 0;

  void tokenize(const std::string &input);

  std::string peek() const;

  std::string next();

  void expect(const std::string &expected);
};