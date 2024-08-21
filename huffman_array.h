#ifndef HUFFMAN_ARRAY_H
#define HUFFMAN_ARRAY_H

#include <stdint.h>

typedef struct HuffmanLeaf_
{
    uint8_t code;    // original code
    char* bitString; // Huffman representation as string of 1s and 0s, e.g "1001001"
} HuffmanLeaf;

typedef struct HuffmanArray_
{
    HuffmanLeaf* data;
    uint32_t length;
    uint32_t capacity;
} HuffmanArray;


HuffmanArray* createHuffmanArray(uint32_t size);
int32_t appendToHuffmanArray(HuffmanArray* array, uint8_t code, const char* bitString);
void sortHuffmanArray(HuffmanArray* array);
void freeHuffmanArray(HuffmanArray** array);

#endif