#include "huffman.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    uint32_t characterCounter[256];

    FILE* fp = fopen("small_compression_test", "rb");
    if (fp == NULL)
    {
        printf("Failed to open file\n");
        return 1;
    }

    for (int i = 0; i < 256; i++)
    {
        characterCounter[i] = 0;
    }

    uint8_t buffer[1024];
    while (1)
    {
        size_t bytesRead = fread(buffer, 1, 1024, fp);
        if (bytesRead == 0)
        {
            break;
        }

        for (int i = 0; i < bytesRead; i++)
        {
            characterCounter[buffer[i]]++;
        }
    }

    fclose(fp);

    // for (int i = 0; i < 256; i++)
    // {
    //     if (characterCounter[i] != 0)
    //     {
    //         printf("Character %d: %d\n", i, characterCounter[i]);
    //     }
    // }



    Node* tree = createHuffmanTree(characterCounter, 256);

    printf("\n\nPrinting tree\n");

    uint64_t sum = 0;
    // printTree(tree, 0, (char*)buffer, &sum);

    // printf("Sum: %ld(bytes)\n", sum / 8);

    writeTreeToFile(tree, "tree");

    freeTree(&tree);

    return 0;
}
