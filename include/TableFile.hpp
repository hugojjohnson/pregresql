#pragma once
#include "Page.hpp"
#include <string>
#include <fstream>

class TableFile {
public:
    explicit TableFile(const std::string& filename);

    void writePage(size_t pageNumber, const Page& page);
    Page readPage(size_t pageNumber);
    size_t allocatePage();

private:
    std::string filename;
};
