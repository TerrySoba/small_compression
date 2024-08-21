#include "bitfile.h"

int writeBitData(BitFile *bf, const char *data, uint64_t dataSize)
{
    if (!bf) return -1;
    if (!data) return -2;
    if (bf->overflowDataSize >= 8) return -3;

    int writtenBytes = 0;

    for (int i = 0; i < dataSize; ++i)
    {
        uint8_t d = *((uint8_t*)&data[i]);
        uint16_t buffer = d;
        buffer <<= bf->overflowDataSize;
        uint8_t writeByte = (buffer & 0xFF) | bf->overflowData;
        writtenBytes += fwrite(&writeByte, 1, 1, bf->fp);
        bf->overflowData = buffer >> 8;
    }

    return writtenBytes * 8;
}

int readBitData(BitFile *bf, char *data, uint64_t dataSize)
{
    if (!bf) return -1;
    if (!data) return -2;
    if (bf->overflowDataSize >= 8) return -3;
    
    int readBytes = 0;
    for (int i = 0; i < dataSize; ++i)
    {
        uint8_t buffer;
        readBytes += fread(&buffer, 1, 1, bf->fp);
        uint16_t buffer16 = buffer;
        buffer16 <<= bf->overflowDataSize;
        buffer16 |= bf->overflowData;
        data[i] = buffer16 & 0xFF;
        buffer16 >>= 8;
        bf->overflowData = buffer16 & 0xFF;
    }

    return readBytes * 8;
}


int writeBitString(BitFile* bf, const char* bitString)
{
    if (!bf) return -1;
    if (!bitString) return -2;
    if (bf->overflowDataSize >= 8) return -3;

    int bitWriteCount = 0;

    while (*bitString)
    {
        bf->overflowData <<= 1;
        bf->overflowData |= (bitString[0] == '1') ? 1 : 0;
        bf->overflowDataSize += 1;
        ++bitString;
        ++bitWriteCount;

        if (bf->overflowDataSize == 8)
        {
            fwrite(&bf->overflowData, 1, 1, bf->fp);
            bf->overflowData = 0;
            bf->overflowDataSize = 0;
        }
    }

    return bitWriteCount;
}

int readBitString(BitFile* bf, char* bitString, uint64_t bits)
{
    if (!bf) return -1;
    if (!bitString) return -2;
    if (bf->overflowDataSize >= 8) return -3;

    int bitReadCount = 0;

    for (uint64_t i = 0; i < bits; ++i)
    {
        if (bf->overflowDataSize == 0)
        {
            fread(&bf->overflowData, 1, 1, bf->fp);
            bf->overflowDataSize = 8;
        }

        bitString[bits - i - 1] = (bf->overflowData & 1) ? '1' : '0';
        bf->overflowData >>= 1;
        bf->overflowDataSize -= 1;
        bitReadCount += 1;
    }

    bitString[bits] = 0;

    return bitReadCount;
}

void closeBitFile(BitFile* bf)
{
    if (bf->overflowDataSize > 0)
    {
        bf->overflowData &= (1 << bf->overflowDataSize) - 1;
        fwrite(&bf->overflowData, 1, 1, bf->fp);
    }

    fclose(bf->fp);
}

int openBitFile(BitFile* bf, const char* filename, const char* mode)
{
    bf->fp = fopen(filename, mode);
    if (bf->fp == NULL)
    {
        return -1;
    }
    bf->overflowData = 0;
    bf->overflowDataSize = 0;

    return 0;
}


