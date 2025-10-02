#include <fstream>
#include <iostream>
#include <iterator>

namespace testUtils {
bool compareFiles(std::ifstream &file1, std::ifstream &file2) {
  // Make sure both files are open
  if (!file1.is_open() || !file2.is_open()) {
    throw std::runtime_error("One or both files are not open");
  }

  std::cout << "Hi there!";

  // Reset to beginning
  file1.clear();
  file1.seekg(0, std::ios::beg);
  file2.clear();
  file2.seekg(0, std::ios::beg);

  // Compare byte by byte
  char ch1, ch2;
  while (true) {
        bool r1 = static_cast<bool>(file1.get(ch1));
        bool r2 = static_cast<bool>(file2.get(ch2));

        if (r1 != r2) {
            // one file ended before the other
            return false;
        }
        if (!r1 && !r2) {
            // both files ended at same time
            return true;
        }
        if (ch1 != ch2) {
            return false;
        }
    }
}

void print_file(std::ifstream &file, const std::string &name) {

  // Reset to beginning
  file.clear();
  file.seekg(0, std::ios::beg);
  // Read file into a vector
  std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  std::cout << name << "\n";

  // Print each byte in hex
  for (uint8_t byte : data) {
    std::cout << std::hex << (int)byte << " ";
  }
  std::cout << "\n";
}

} // namespace testUtils