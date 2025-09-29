#include "../include/Table.hpp"
#include "../include/StorageManager.hpp"
#include <stdexcept>
#include <iostream>

Table::Table(const std::string& name_, const Schema& schema_)
    : name(name_), schema(schema_), storage(name_, schema_) {}

void Table::insertRow(const std::vector<Row::FieldValue>& values) {
    if (values.size() != schema.getNumFields()) {
        std::cout << "Error: Row size (" << values.size() << ") does not match schema size (" << schema.getNumFields() << ").\n";
        throw std::runtime_error("Value count does not match schema.");
    }

    // Optional: type check against schema
    for (size_t i = 0; i < values.size(); ++i) {
        const auto& fieldType = schema.getField(i).type;
        bool typeMatches = false;
        std::visit([&](auto&& val){ // You visit variants.
            using T = std::decay_t<decltype(val)>; // Strip away references, const, etc.. to just get the type.
            switch (fieldType) {
                case Schema::FieldType::INT:
                    typeMatches = std::is_same_v<T,int>;
                    break;
                case Schema::FieldType::FLOAT:
                    typeMatches = std::is_same_v<T,float>;
                    break;
                case Schema::FieldType::STRING:
                    typeMatches = std::is_same_v<T,std::string>;
                    break;
            }
        }, values[i]);
        if (!typeMatches) {
            throw std::runtime_error("Type of value does not match schema.");
        }
    }

    Row row(values);
    rows.push_back(row);

    // Later: write to file via storage
    storage.appendRow(row);
}

std::vector<Row> Table::selectRows(std::function<bool(const Row&)> predicate) const {
    std::vector<Row> result;
    for (const auto& row : rows) {
        if (predicate(row)) {
            result.push_back(row);
        }
    }
    return result;
}
