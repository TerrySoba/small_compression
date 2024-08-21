#include <catch2/catch_test_macros.hpp>

#include <cstring>

extern "C" {
#include "huffman_array.h"
}


TEST_CASE("Huffman Array Test1")
{
    HuffmanArray* array = createHuffmanArray(256);
    REQUIRE(array != 0);

    REQUIRE(array->length == 0);
    REQUIRE(array->capacity == 256);

    REQUIRE(appendToHuffmanArray(array, 4, "Hallo Welt!4") == 0);
    REQUIRE(appendToHuffmanArray(array, 2, "Hallo Welt!2") == 1);
    REQUIRE(appendToHuffmanArray(array, 7, "Hallo Welt!7") == 2);

    REQUIRE(array->length == 3);
    REQUIRE(array->capacity == 256);

    REQUIRE(array->data[0].code == 4);
    REQUIRE(array->data[1].code == 2);
    REQUIRE(array->data[2].code == 7);

    REQUIRE(strcmp(array->data[0].bitString, "Hallo Welt!4") == 0);
    REQUIRE(strcmp(array->data[1].bitString, "Hallo Welt!2") == 0);
    REQUIRE(strcmp(array->data[2].bitString, "Hallo Welt!7") == 0);

    sortHuffmanArray(array);

    REQUIRE(array->data[0].code == 2);
    REQUIRE(array->data[1].code == 4);
    REQUIRE(array->data[2].code == 7);

    REQUIRE(strcmp(array->data[0].bitString, "Hallo Welt!2") == 0);
    REQUIRE(strcmp(array->data[1].bitString, "Hallo Welt!4") == 0);
    REQUIRE(strcmp(array->data[2].bitString, "Hallo Welt!7") == 0);

    freeHuffmanArray(&array);
}

TEST_CASE("Huffman Array Test2")
{
    HuffmanArray* array = createHuffmanArray(256);
    REQUIRE(array != 0);

    for (int i = 0; i < 256; i++)
    {
        char buffer[256];
        sprintf(buffer, "Hallo Welt!%d", i);
        appendToHuffmanArray(array, i, buffer);
    }

    REQUIRE(array->length == 256);

    freeHuffmanArray(&array);
}