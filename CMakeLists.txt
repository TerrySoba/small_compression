cmake_minimum_required(VERSION 3.28)
project(small_compression)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")


find_package(Catch2 REQUIRED)

add_library(small_compression_lib STATIC
    bitfile.c
    huffman.c
    huffman_array.c
    )

add_executable(small_compression 
    main.c)

target_link_libraries(small_compression small_compression_lib)


## Tests

add_executable(small_compression_test
    tests/bitfile_test.cpp
    tests/huffman_array_test.cpp
    )

target_include_directories(small_compression_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}_test
    small_compression_lib
    Catch2::Catch2WithMain)

