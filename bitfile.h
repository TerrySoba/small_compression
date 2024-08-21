#ifndef BITFILE_H
#define BITFILE_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    FILE* fp;
    uint8_t overflowData;
    uint8_t overflowDataSize;
} BitFile;


void closeBitFile(BitFile* bf);
int openBitFile(BitFile* bf, const char* filename, const char* mode);


// Write a number of 8bit blocks to the file. The number of blocks is determined by dataSize. The written data is not aligned to 8bit blocks.
int writeBitData(BitFile *bf, const char *data, uint64_t dataSize);

// Write a bit string to the file. The bit string is a null-terminated string of '0' and '1' characters.
int writeBitString(BitFile* bf, const char* bitString);

// Read a number of 8bit blocks from the file. The number of blocks is determined by dataSize.
int readBitData(BitFile *bf, char *data, uint64_t dataSize);

// Read a bit string from file. The bit string is a null-terminated string of '0' and '1' characters.
// The butString needs to have room for bits + 1 8bit values.
int readBitString(BitFile* bf, char* bitString, uint64_t bits);

#endif