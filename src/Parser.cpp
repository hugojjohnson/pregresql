#include "../include/Parser.hpp"
#include "../include/Schema.hpp"
#include "Table.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Table Parser::parseCreateTable(Schema& schema) {

  expect("CREATE");
  expect("TABLE");

  std::string name = next();

  Table table(name, schema);

  expect("(");

  // Parse columns until ')'
  while (peek() != ")") {
      Schema::FieldType field = Schema::parseFieldType(next());
      std::string name = next();
      schema.addField(name, field);

    if (peek() == ",") {
      next(); // consume the comma
    } else if (peek() != ")") {
      throw std::runtime_error("Expected ',' or ')'");
    }
  }

  expect(")");

  return table;
}

void Parser::tokenize(const std::string &input) {
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
  if (pos >= tokens.size())
    throw std::runtime_error("Unexpected end of input");
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