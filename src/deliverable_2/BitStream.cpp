#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <cmath>

BitStream::BitStream(char *FileName, int M = 8, char *OutputFileName = "golomb.bin")
{
    FILE *pFile = fopen(FileName, "rb");

    if (!pFile)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    this->pFile = pFile;
    this->M = M;
    this->buffer = std::vector<u_char>();
    this->out = std::ofstream(OutputFileName);
};

BitStream::~BitStream()
{
    fclose(pFile);
};

int BitStream::ReadBit()
{
    u_char byte;
    fread(&byte, sizeof(u_char), 1, pFile);
    buffer.push_back(byte);
    return 0;
};

int BitStream::ReadByte(int n_bits)
{
    for (int i = 0; i < n_bits; i++)
    {
        ReadBit();
    }
    return 0;
};

// https://en.wikipedia.org/wiki/Golomb_coding
void BitStream::WriteBit()
{
    u_char byte = buffer[0];
    // remove the first element
    buffer.erase(buffer.begin());

    u_char q = byte / M;
    u_char r = byte % M;

    for (int i = 0; i < q; i++)
    {
        out << 1;
    }

    out << 0;

    u_char b = ceil(log2(M));

    if (r < pow(2, b + 1))
    {
        for (int i = b; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
    else
    {
        r += pow(2, b + 1) - M;
        for (int i = b + 1; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
};

void BitStream::WriteBits(int n_bits)
{
    for (int i = 0; i < n_bits; i++)
    {
        WriteBit();
    }
};
