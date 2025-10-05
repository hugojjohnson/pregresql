#include "include/Executor.hpp"
#include "include/Parser.hpp"
#include "include/Row.hpp"
#include "include/Schema.hpp"
#include "include/Statements.hpp"
#include "include/Table.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <vector>

int main() {
  std::string query;
  Parser parser;
  Executor executor;
  Statements::StatementVariant stmt;

  while (true) {
    std::getline(std::cin, query);
    std::cout << "Received: " << query << "\n";

    try {
      stmt = parser.parse(query);
    } catch (const std::exception &ex) {
        std::cerr << "Parse error: " << ex.what() << "\n";
        continue;
    }

    try {
      std::visit([&executor](auto&& s){ executor.execute(s); }, stmt);
    } catch (const std::exception& ex) {
        std::cerr << "Execution error: " << ex.what() << "\n";
    }
  }

}

// === Queries implemented so far ===
// CREATE TABLE MyTable ( INT id, FLOAT val, STRING str )

// === Up next ===
// INSERT INTO MyTable VALUES (3, 2.3, "Hugo");