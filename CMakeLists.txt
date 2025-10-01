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
    main.cpp
)

add_executable(tinydb ${SOURCES})