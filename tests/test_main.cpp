#include "../include/Row.hpp"
#include "../include/Schema.hpp"
#include "../include/Table.hpp"
#include <iostream>
#include <vector>

#include "../include/utils.hpp"

int main() {
  std::ifstream f("tests/golden/people.tbl", std::ios::binary);
  if (!f) {
    std::cerr << "Failed to open file\n";
    return 1;
  }

  // Read file into a vector
  std::vector<uint8_t> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

  std::cout << "people.tbl:\n";

  // Print each byte in hex
  for (uint8_t byte : data) {
    std::cout << std::hex << (int)byte << " ";
  }
  std::cout << "\n";
}
// int main() {
//   Schema schema;
//   Table table("people", schema);
//   table.load();

//   std::cout << table;
//   return 0;
// }
// int main() {
//     Schema schema;
//     schema.addField("id", Schema::FieldType::INT);
//     schema.addField("name", Schema::FieldType::STRING, 20);
//     schema.pkIndex = -1;

//     Table table("people", schema);

//     table.setPk(0);

//     table.writeSchema();

//     table.insertRow(std::vector<Row::FieldValue>{1, "alan"});
//     // table.insertRow(std::vector<Row::FieldValue>{2, "JOHN"});
//     // table.insertRow(std::vector<Row::FieldValue>{3, "Steve"});
//     // table.insertRow(std::vector<Row::FieldValue>{3, "Steve 2"});

//     auto res = table.getRows();
//     for (const auto i : res) {
//         std::cout << i.print(schema);
//     }
// }
