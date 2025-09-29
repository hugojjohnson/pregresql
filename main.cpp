#include <iostream>
#include <vector>
#include "include/Schema.hpp"
#include "include/Table.hpp"
#include "include/Row.hpp"

int main() {
    Schema schema;
    schema.addField("id", Schema::FieldType::INT);
    schema.addField("name", Schema::FieldType::STRING, 20);

    Table table("people", schema);
    table.insertRow(std::vector<Row::FieldValue>{1, "hi"});

    auto res = table.getRows();
    for (const auto i : res) {
        std::cout << i.print(schema);
    }

    // Table(const std::string &name, const Schema &schema);
}
