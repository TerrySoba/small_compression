#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "bitfile.h"

#include <stdint.h>
#include <stdlib.h>


// Node structure for the Huffman tree
typedef struct _Node {
    struct _Node* firstChild;
    struct _Node* secondChild;
    uint64_t frequency;
    int16_t character;
} Node;


int compareNodes(const void* a, const void* b);
void freeTree(Node** node);
Node* createHuffmanTree(uint32_t* characterCounter, size_t characterCounterSize);
void printTree(Node* node, int depth, char* buffer, uint64_t* sum);
void writeTreeToFile(Node* node, const char* filename);

#endif