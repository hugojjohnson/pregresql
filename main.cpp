#include "include/Row.hpp"
#include "include/Schema.hpp"
#include "include/Table.hpp"
#include <iostream>
#include <vector>

#include "include/utils.hpp"

int main() {
  Schema schema;
  Table table("people", schema);
  table.load();

  std::cout << table;
}
// int main() {
//     Schema schema;
//     schema.addField("id", Schema::FieldType::INT);
//     schema.addField("name", Schema::FieldType::STRING, 20);
//     schema.canBeNull(1);

//     Table table("people", schema);
    
//     table.setPk(0);

//     table.writeSchema();

//     table.insertRow(std::vector<Row::FieldValue>{1, "alan"});
//     table.insertRow(std::vector<Row::FieldValue>{2, "JOHN"});
//     table.insertRow(std::vector<Row::FieldValue>{3, std::nullopt});
//     // table.insertRow(std::vector<Row::FieldValue>{3, "Steve"});
//     // table.insertRow(std::vector<Row::FieldValue>{3, "Steve 2"});

//     auto res = table.getRows();
//     for (const auto i : res) {
//         std::cout << i.print(schema);
//     }
// }
