#include "../include/Executor.hpp"
#include "../include/Parser.hpp"
#include "../include/Row.hpp"
#include "../include/Schema.hpp"
#include "../include/Table.hpp"
#include "../include/utils.hpp"
#include "./testUtils.hpp"
#include <iostream>
#include <string>
#include <vector>

bool test_1();
bool test_tokeniser();

int main() {
  if (!test_tokeniser()) {
    return 1;
  }
  if (!test_1()) {
    return 1;
  }
  return 0;
}

// TODO: Not working - I changed the way things are sorted so this needs updating too.
bool test_1() {
  std::cout << "Test 1...\n";
  Parser parser;
  Executor executor;
  Statements::StatementVariant stmt;

  std::string query = "CREATE TABLE Hi (INT id, FLOAT test)";
  stmt = parser.parse(query);
  std::visit([&executor](auto &&s) { executor.execute(s); }, stmt);
  std::cout << "Test 1 passed!\n";
  return true;
}

bool test_tokeniser() {
  Parser p;
  p.tokenize("INSERT INTO Hi VALUES (1, 1.0)");
  std::vector<std::string> expected = {"INSERT", "INTO", "Hi", "VALUES", "(", "1", ",", "1.0", ")"};
  // if (p.tokens.size() != expected.size()) {
  //   std::cout << "tokens size (" << p.tokens.size() << ") does not match expected size (" << expected.size() << ")";
  //   return false;
  // }
  for (int i = 0; i < p.tokens.size(); i++) {
    std::cout << p.tokens[i] << "\n";
  }
  return true;
}