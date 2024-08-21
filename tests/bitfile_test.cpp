#include <catch2/catch_test_macros.hpp>

#include <cstring>

extern "C" {
#include "bitfile.h"
}


TEST_CASE("Open BitFile Test")
{
    BitFile bf;
    REQUIRE(openBitFile(&bf, "///", "wb") == -1);
    REQUIRE(openBitFile(&bf, "test.bin", "wb") == 0);
    closeBitFile(&bf);
}

TEST_CASE("Write BitFile Test")
{
    BitFile bf;
    REQUIRE(openBitFile(&bf, "test.bin", "wb") == 0);

    REQUIRE(writeBitString(&bf, "01100001" "01100010") == 16); // "ab"

    // "ab"
    REQUIRE(writeBitString(&bf, "0110000") == 7);
    REQUIRE(writeBitString(&bf, "101100010") == 9);

    // "ab"
    REQUIRE(writeBitString(&bf, "0110000101100") == 13);
    REQUIRE(writeBitString(&bf, "010") == 3);

    closeBitFile(&bf);

    FILE* fp = fopen("test.bin", "rb");
    REQUIRE(fp != NULL);
    // read 6 bytes
    uint8_t buffer[6];
    REQUIRE(fread(buffer, 1, 6, fp) == 6);
    REQUIRE(fread(buffer, 1, 1, fp) == 0); // check EOF
    fclose(fp);

    REQUIRE(memcmp(buffer, "ababab", 6) == 0);
}

TEST_CASE("Write Bit Data Test aligned data")
{
    BitFile bf;
    REQUIRE(openBitFile(&bf, "test.bin", "wb") == 0);

    REQUIRE(writeBitData(&bf, "12", 2) == 16); // "ab"

    closeBitFile(&bf);

    FILE* fp = fopen("test.bin", "rb");
    REQUIRE(fp != NULL);
    // read 2 bytes
    uint8_t buffer[2];
    REQUIRE(fread(buffer, 1, 2, fp) == 2);
    REQUIRE(fread(buffer, 1, 1, fp) == 0); // check EOF
    fclose(fp);

    REQUIRE(memcmp(buffer, "12", 2) == 0);
}


TEST_CASE("Write Bit Data Test unaligned data")
{
    BitFile bf;
    REQUIRE(openBitFile(&bf, "test.bin", "wb") == 0);

    REQUIRE(writeBitString(&bf, "111") == 3);
    REQUIRE(writeBitData(&bf, "12", 2) == 16);

    closeBitFile(&bf);

    FILE* fp = fopen("test.bin", "rb");
    REQUIRE(fp != NULL);
    // read 3 bytes
    uint8_t buffer[3];
    REQUIRE(fread(buffer, 1, 3, fp) == 3);
    REQUIRE(fread(buffer, 1, 1, fp) == 0); // check EOF
    fclose(fp);

    uint16_t reconstructed;
    reconstructed = buffer[0] | (buffer[1] << 8);
    reconstructed >>= 3;
    reconstructed &= 0xff;

    REQUIRE(reconstructed == '1');

    reconstructed = buffer[1] | (buffer[2] << 8);
    reconstructed >>= 3;
    reconstructed &= 0xff;

    REQUIRE(reconstructed == '2');

    // REQUIRE(memcmp(buffer, "12", 2) == 0);
}


static void fillFileWithText(const char* filename, const char* str)
{
    FILE* fp = fopen(filename, "wb");
    fwrite(str, strlen(str), 1, fp);
    fclose(fp);
}


TEST_CASE("Read Bit Data Test aligned")
{
    fillFileWithText("test.bin", "1234");

    BitFile bf;
    REQUIRE(openBitFile(&bf, "test.bin", "rb") == 0);

    char buffer[4];
    REQUIRE(readBitData(&bf, buffer, 4) == 32);

    closeBitFile(&bf);

    REQUIRE(memcmp(buffer, "1234", 4) == 0);
}


TEST_CASE("Read Bit Data Test unaligned")
{
    {
        BitFile bf;
        REQUIRE(openBitFile(&bf, "test.bin", "wb") == 0);

        REQUIRE(writeBitString(&bf, "10010") == 5);

        REQUIRE(writeBitData(&bf, "12", 2) == 16);

        closeBitFile(&bf);
    }

    {
        BitFile bf;
        REQUIRE(openBitFile(&bf, "test.bin", "rb") == 0);

        char buffer[6];
        REQUIRE(readBitString(&bf, buffer, 5) == 5);
        REQUIRE(strcmp("10010", buffer) == 0);

        REQUIRE(readBitData(&bf, buffer, 2) == 16);

        REQUIRE(memcmp(buffer, "12", 2) == 0);

        closeBitFile(&bf);
    }
}

