#pragma once
#include <deque>
#include <vector>

// Note: All these assume little endian!
namespace utils {
// Concept: container must have push_back(T)
template<typename C, typename T>
concept PushBackable = requires(C c, T v) {
    c.push_back(v);
};

// Concept: container must have push_front(T)
template<typename C, typename T>
concept PushFrontable = requires(C c, T v) {
    c.push_front(v);
};

// Push bytes LSB-first (little endian) to the back
template<PushBackable<uint8_t> Container, typename T>
requires std::is_arithmetic_v<T>
void push_back_bytes(Container& c, T value) {
    uint8_t bytes[sizeof(T)];
    std::memcpy(bytes, &value, sizeof(T));
    for (size_t i = 0; i < sizeof(T); i++)
        c.push_back(bytes[i]);
}

// Push bytes LSB-first (little endian) to the front
template<PushFrontable<uint8_t> Container, typename T>
requires std::is_arithmetic_v<T>
void push_front_bytes(Container& c, T value) {
    uint8_t bytes[sizeof(T)];
    std::vector<uint8_t> temp;
    std::memcpy(bytes, &value, sizeof(T));
    for (size_t i = 0; i < sizeof(T); i++) {
        temp.push_back(bytes[i]);
    }
    c.insert(c.begin(), temp.begin(), temp.end());
}

// template <typename T>
// requires std::is_arithmetic_v<T>
// T read_le(const uint8_t* ptr) {
//     T value;
//     std::memcpy(&value, ptr, sizeof(T));
//     return value;
// }

class ByteReader {
    const uint8_t* ptr_;
    std::size_t remaining_;
public:
    ByteReader(const uint8_t* data, std::size_t size)
        : ptr_(data), remaining_(size) {}

    template<typename T>
    T read_le() {
        static_assert(std::is_trivial_v<T>, "Only trivial types supported");
        if (remaining_ < sizeof(T)) {
            throw std::runtime_error("Not enough bytes to read");
        }
        T value;
        std::memcpy(&value, ptr_, sizeof(T));
        ptr_ += sizeof(T);
        remaining_ -= sizeof(T);

        // if file is little-endian but machine is big-endian, swap bytes here
        // (endian swap code omitted for brevity)
        return value;
    }

    std::string read_string(std::size_t length) {
        if (remaining_ < length) {
            throw std::runtime_error("Not enough bytes to read string");
        }
        std::string s(reinterpret_cast<const char*>(ptr_), length);
        ptr_ += length;
        remaining_ -= length;
        return s;
    }

    const uint8_t* current() const { return ptr_; }
    std::size_t remaining() const { return remaining_; }
};

// // Read bytes LSB-first (little endian) from an iterator range into an arithmetic type
// template <typename T, typename It>
// requires std::is_arithmetic_v<T>
// T read_le(It first, It last) {
//     static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>,
//                   "T must be arithmetic");
//     if (std::distance(first, last) < static_cast<std::ptrdiff_t>(sizeof(T))) {
//         throw std::out_of_range("Not enough bytes to read value");
//     }

//     uint8_t bytes[sizeof(T)];
//     for (size_t i = 0; i < sizeof(T); i++, ++first) {
//         bytes[i] = static_cast<uint8_t>(*first);
//     }

//     T value;
//     std::memcpy(&value, bytes, sizeof(T)); // reassemble
//     return value;
// }
}