#include "../include/Parser.hpp"
#include "../include/Schema.hpp"
#include "../include/Statements.hpp"
#include "Table.hpp"
#include <algorithm>
#include <cctype> // for std::isalnum
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace Statements;

StatementVariant Parser::parse(const std::string &input) {
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
    if (peek() != ")") {
      expect(",");
    }
  }
  return InsertStmt{tableName, values};
}

void Parser::tokenize(const std::string &input) {
  if (input == "") {
    return;
  }
  pos = 0;
  tokens.clear();
  std::istringstream iss(input);
  std::string token;
  while (iss >> token) {
    char front = token.front();
    char back = token.back();
    bool add_back = false;
    if (front == ',' || front == '(') {
      tokens.emplace_back(1, front);
      token = token.substr(1);
    }
    if (back == ',' || back == ')') {
      token = token.substr(0, token.size() - 1);
      add_back = true;
    }
    bool is_alnum = std::all_of(token.begin(), token.end(), [](unsigned char c) { 
      return std::isalnum(c) || c == '.';
    });
    if (!is_alnum) {
      throw std::runtime_error("Invalid token: Spaces are needed. My bad.");
    }
    tokens.push_back(token);
    if (add_back) {
      tokens.emplace_back(1, back);
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