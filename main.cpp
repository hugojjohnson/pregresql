#include "TableFile.hpp"
#include <iostream>

int main() {
    TableFile users("users.tbl");

    // Allocate first page
    size_t pageId = users.allocatePage();
    Page page;

    // Insert rows
    Row r1({"1", "Alice"});
    Row r2({"2", "Bob"});

    page.insertRow(r1);
    page.insertRow(r2);

    // Save page to file
    users.writePage(pageId, page);

    // Read it back
    Page loaded = users.readPage(pageId);
    auto rows = loaded.getRows();

    std::cout << "Read back " << rows.size() << " rows\n";
    for (auto& r : rows) {
        std::cout << "[Row length=" << r.data.size() << "]\n";
    }
}
