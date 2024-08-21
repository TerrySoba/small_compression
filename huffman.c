#include "huffman.h"
#include "huffman_array.h"
#include "bitfile.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


int compareNodes(const void* a, const void* b)
{
    Node** nodeA = (Node**)a;
    Node** nodeB = (Node**)b;

    if ((*nodeA)->frequency < (*nodeB)->frequency)
    {
        return -1;
    }
    else if ((*nodeA)->frequency > (*nodeB)->frequency)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void freeTree(Node** node)
{
    if ((*node)->firstChild != NULL)
    {
        freeTree(&(*node)->firstChild);
    }

    if ((*node)->secondChild != NULL)
    {
        freeTree(&(*node)->secondChild);
    }

    free(*node);
    *node = NULL;
}

// Function to build the Huffman tree
Node* createHuffmanTree(uint32_t* characterCounter, size_t characterCounterSize)
{
    Node** nodes = (Node**)malloc(sizeof(Node*) * characterCounterSize);
    for (int i = 0; i < characterCounterSize; i++)
    {
        nodes[i] = (Node*)malloc(sizeof(Node));
        nodes[i]->frequency = characterCounter[i];
        nodes[i]->firstChild = NULL;
        nodes[i]->secondChild = NULL;
        nodes[i]->character = i;
    }

    // sort the nodes by frequency
    qsort(nodes, characterCounterSize, sizeof(Node*), compareNodes);
   
    for (int i = 0; i < characterCounterSize - 1; ++i)
    {
        // combine first two elements, as they have the lowest frequencies
        Node* n = (Node*)malloc(sizeof(Node));
        n->firstChild = nodes[0];
        n->secondChild = nodes[1];
        n->character = -1;
        n->frequency = nodes[0]->frequency + nodes[1]->frequency;
        nodes[0] = n;
    
        // now move everything one position to the left
        for (int j = 1; j < characterCounterSize - i; ++j)
        {
            nodes[j] = nodes[j + 1];
        }
    
        // sort the nodes by frequency
        qsort(nodes, characterCounterSize - i - 1, sizeof(Node*), compareNodes);
    }

    Node* root = (Node*)malloc(sizeof(Node));
    *root = *nodes[0];
    free(nodes);
    return root;
}


void printTree(Node* node, int depth, char* buffer, uint64_t* sum)
{
    if (node->firstChild != NULL)
    {
        buffer[depth] = '0';
        printTree(node->firstChild, depth + 1, buffer, sum);
    }

    // for (int i = 0; i < depth; i++)
    // {
    //     printf("  ");
    // }

    if (node->firstChild == NULL)
    {
        buffer[depth] = 0;
        printf("Character: %d, Frequency: %ld\n", node->character, node->frequency);
        printf("%s\n", buffer);
        *sum += node->frequency * depth;
    }

    if (node->secondChild != NULL)
    {
        buffer[depth] = '1';
        printTree(node->secondChild, depth + 1, buffer, sum);
    }
}

void writeTreeToArray(Node* node, int depth, char* buffer, HuffmanArray* array)
{
    if (!node)
    {
        return;
    }

    if (node->firstChild != NULL)
    {
        buffer[depth] = '0';
        writeTreeToArray(node->firstChild, depth + 1, buffer, array);
    }

    if (node->firstChild == NULL)
    {
        buffer[depth] = 0;
        assert(appendToHuffmanArray(array, node->character, buffer) >= 0);
    }

    if (node->secondChild != NULL)
    {
        buffer[depth] = '1';
        writeTreeToArray(node->secondChild, depth + 1, buffer, array);
    }
}

void writeTreeToFile(Node* node, const char* filename)
{
    // at first create sorted representation of tree
    HuffmanArray* array = createHuffmanArray(256);

    char buffer[256];

    writeTreeToArray(node, 0, buffer, array);

    sortHuffmanArray(array);

    // print the sorted nodes
    // for (int i = 0; i < array->length; i++)
    // {
    //     printf("Character: %d, Bitstring: %s\n", array->data[i].code, array->data[i].bitString);
    // }

    printf("Array length: %d\n", array->length);
    assert(array->length == 256);

    BitFile bf;
    openBitFile(&bf, filename, "wb");

    for (int i = 0; i < array->length; i++)
    {
        uint8_t bitStringLength = strlen(array->data[i].bitString);
        writeBitData(&bf, (const char*)&bitStringLength, 1);
        writeBitString(&bf, array->data[i].bitString);
    }

    closeBitFile(&bf);

    freeHuffmanArray(&array);
}