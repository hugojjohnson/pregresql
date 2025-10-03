cmake_minimum_required(VERSION 3.16)
project(tinydb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect sources
set(SOURCES
    src/Row.cpp
    src/Schema.cpp
    src/StorageManager.cpp
    src/Table.cpp
    src/Parser.cpp
)

# set(TEST_SOURCES
#     tests/*
# )
# file(GLOB TEST_SOURCES "tests/*.cpp") # or "*.cxx" or whatever
file(GLOB TEST_CPP "tests/*.cpp")
file(GLOB TEST_HPP "tests/*.hpp")


add_executable(main ${SOURCES} main.cpp)


enable_testing()  # tell CMake you’re going to have tests

# add_executable(tinydb_test ${TINYDB_SOURCES} tests/main.cpp)
add_executable(t ${SOURCES} ${TEST_CPP} ${TEST_HPP})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/tests/golden DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/tests)


# Register the test with CTest
add_test(NAME tinydb_test COMMAND t)
