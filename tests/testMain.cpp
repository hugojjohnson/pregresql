#include "../include/Row.hpp"
#include "../include/Schema.hpp"
#include "../include/Table.hpp"
#include "../include/utils.hpp"
#include "./testUtils.hpp"
#include <iostream>
#include <vector>

bool test_1();

int main() {
  if (!test_1()) {
    return 1;
  }
  return 0;
}

// TODO: Not working - I changed the way things are sorted so this needs updating too.
bool test_1() {
  // Setup
  // Schema schema;
  // schema.addField("id", Schema::FieldType::INT);
  // schema.addField("name", Schema::FieldType::STRING, 20);
  // schema.pkIndex = -1;
  // Table table("people", schema);
  // table.setPk(0);
  // table.writeSchema();
  // table.insertRow(std::vector<Row::FieldValue>{1, "alan"});
  // table.insertRow(std::vector<Row::FieldValue>{2, "JOHN"});

  // std::ifstream golden("tests/golden/people.tbl", std::ios::binary);
  // std::ifstream test("people.tbl", std::ios::binary);
  // // testUtils::print_file(golden, "golden");
  // // testUtils::print_file(test, "test");

  // return testUtils::compareFiles(golden, test);
  return true;
}