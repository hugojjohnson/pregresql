#include "../include/Parser.hpp"
#include "../include/Schema.hpp"
#include "../include/Statements.hpp"
#include "Table.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Statements;

StatementVariant Parser::parse(std::string& input) {
  tokenize(input);
  if (peek() == "CREATE") {
    return parseCreateTable();
  }
  if (peek() == "INSERT") {
    return parseInsert();
  }
  throw std::runtime_error("Statement not found.");
}


CreateTableStmt Parser::parseCreateTable() {
  expect("CREATE");
  expect("TABLE");
  std::string name = next();
  std::vector<CreateTableStmt::CreateTableField> fields;
  expect("(");
  // Parse columns until ')'
  while (peek() != ")") {
    Schema::FieldType field = Schema::parseFieldType(next());
    std::string name = next();
    fields.push_back(CreateTableStmt::CreateTableField{name, field});
    if (peek() == ",") {
      next(); // consume the comma
    } else if (peek() != ")") {
      throw std::runtime_error("Expected ',' or ')'");
    }
  }
  expect(")");
  return CreateTableStmt{name, fields};
}

InsertStmt Parser::parseInsert() {
  expect("INSERT");
  expect("INTO");
  std::string tableName = next();
  expect("VALUES");
  expect("(");

  std::vector<std::string> values;
  while (peek() != ")") {
    values.push_back(next());
  }
  return InsertStmt{tableName, values};
}

void Parser::tokenize(const std::string &input) {
  pos = 0;
  tokens.clear();
  std::istringstream iss(input);
  std::string token;
  while (iss >> token) {
    // split off commas and parentheses
    if (token.back() == ',' || token.back() == ')') {
      char c = token.back();
      token.pop_back();
      if (!token.empty()) {
        tokens.push_back(token);
      }
      tokens.emplace_back(1, c);
    } else if (token.front() == '(') {
      tokens.emplace_back("(");
      token = token.substr(1);
      if (!token.empty())
        tokens.push_back(token);
    } else {
      tokens.push_back(token);
    }
  }
}

std::string Parser::peek() const {
  if (pos >= tokens.size()) {
    throw std::runtime_error("Unexpected end of input");
  }
  return tokens[pos];
}

std::string Parser::next() {
  std::string t = peek();
  pos++;
  return t;
}

void Parser::expect(const std::string &expected) {
  std::string t = next();
  if (t != expected) {
    throw std::runtime_error("Expected '" + expected + "', got '" + t + "'");
  }
}