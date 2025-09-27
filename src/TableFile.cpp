#include "../include/TableFile.hpp"
#include <iostream>

TableFile::TableFile(const std::string& fname) : filename(fname) {}

void TableFile::writePage(size_t pageNumber, const Page& page) {
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        file.open(filename, std::ios::out | std::ios::binary);
        file.close();
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
    file.seekp(pageNumber * Page::PAGE_SIZE, std::ios::beg);
    file.write(reinterpret_cast<const char*>(page.buffer.data()), Page::PAGE_SIZE);
}

Page TableFile::readPage(size_t pageNumber) {
    Page page;
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("File not found: " + filename);
    }
    file.seekg(pageNumber * Page::PAGE_SIZE, std::ios::beg);
    file.read(reinterpret_cast<char*>(page.buffer.data()), Page::PAGE_SIZE);
    return page;
}

size_t TableFile::allocatePage() {
    std::ofstream file(filename, std::ios::app | std::ios::binary);
    Page empty;
    file.write(reinterpret_cast<const char*>(empty.buffer.data()), Page::PAGE_SIZE);
    file.close();
    return (std::filesystem::file_size(filename) / Page::PAGE_SIZE) - 1;
}
