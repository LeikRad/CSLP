#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <cmath>

BitStream::BitStream(char *FileName, char *OutputFileName = "golomb.bin")
{
    FILE *pFile = fopen(FileName, "rb");
    std::vector<int> readbuffer;
    pos = -1;
    byte = 0;

    if (!pFile)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    this->pFile = pFile;
    // this->M = M;
    // this->buffer = std::vector<u_char>();
    this->out = std::ofstream(OutputFileName);
};

BitStream::~BitStream()
{
    fclose(pFile);
    out.close();
};

int BitStream::ReadBit()
{
    if (pos < 0)
    {
        fread(&byte, sizeof(u_char), 1, pFile);
        pos = 7;
    }
    return (byte >> pos--) % 2;
};

int* BitStream::ReadBits(int n_bits)
{
    int buffer[n_bits];

    for (int i = 0; i < n_bits; i++)
    {
        buffer[i] = ReadBit();
    }
    return buffer;
};

// https://en.wikipedia.org/wiki/Golomb_coding
void BitStream::WriteBit(int bit)
{
    out << bit;
};

void BitStream::WriteBits(int *bits)
{
    int size = sizeof(bits)/sizeof(int);

    for (int i = 0; i < size; i++)
    {
        WriteBit(bits[i]);
    }
};
