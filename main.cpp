#include "include/Parser.hpp"
#include "include/Row.hpp"
#include "include/Schema.hpp"
#include "include/Table.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <vector>

int main() {
  // std::string query = "CREATE TABLE MyTable ( INT id, FLOAT val, STRING str )";
  std::string query;
  // std::cin >> query;
  std::getline(std::cin, query);

  try {
    Parser parser(query);
    Schema schema;
    Table table = parser.parseCreateTable(schema);

    std::cout << table;
  } catch (const std::exception &ex) {
    std::cerr << "Parse error: " << ex.what() << "\n";
  }
}


// CREATE TABLE Bank_balances (INT id, STRING phone_number, FLOAT bank_balance)