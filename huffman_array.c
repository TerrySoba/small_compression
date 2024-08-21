#include "huffman_array.h"

#include <string.h>
#include <stdlib.h>

int32_t appendToHuffmanArray(HuffmanArray* array, uint8_t code, const char* bitString)
{
    if (!array)
    {
        return -1;
    }

    if (array->length >= array->capacity)
    {
        return -2;
    }

    HuffmanLeaf* data = &(array->data[array->length]);
    data->code = code;
    data->bitString = strdup(bitString);
    array->length++;

    return array->length - 1;
}

HuffmanArray* createHuffmanArray(uint32_t size)
{
    HuffmanArray* array = (HuffmanArray*)malloc(sizeof(HuffmanArray));
    array->capacity = size;
    array->length = 0;
    array->data = (HuffmanLeaf*)malloc(sizeof(HuffmanLeaf) * size);

    for (uint32_t i = 0; i < size; ++i)
    {
        array->data[i].bitString = NULL;
        array->data[i].code = 0;
    }

    return array;
}

int compareHuffmanLeaf(const void* a, const void* b)
{
    HuffmanLeaf* leafA = (HuffmanLeaf*)a;
    HuffmanLeaf* leafB = (HuffmanLeaf*)b;

    if (leafA->code < leafB->code)
    {
        return -1;
    }
    else if (leafA->code > leafB->code)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void sortHuffmanArray(HuffmanArray* array)
{
    qsort(array->data, array->length, sizeof(HuffmanLeaf), compareHuffmanLeaf);
}

void freeHuffmanArray(HuffmanArray** array)
{
    if (!array)
    {
        return;
    }

    if (!(*array)->data)
    {
        return;
    }

    for (int i = 0; i < (*array)->capacity; ++i)
    {
        free((*array)->data[i].bitString);
    }

    free((*array)->data);

    free(*array);

    *array = NULL;
}
