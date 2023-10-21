#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <filesystem>

BitStream::BitStream(const char *FileName, const char *OutputFileName = "out.bin")
{
    this->in = std::ifstream(FileName, std::ios::binary);
    pos = -1;
    byte = 0;

    if (!in)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    this->out = std::ofstream(OutputFileName);

    // set in, out to first position
    in.seekg(0, std::ios::beg);
    out.seekp(0, std::ios::beg);
};

BitStream::~BitStream()
{
    in.close();
    out.close();
};

int BitStream::ReadBit()
{
    if (pos < 0)
    {
        in.read((char *)&byte, sizeof(u_char));
        if (in.eof())
        {
            return -1;
        }
        pos = 7;
    }
    return (byte >> pos--) % 2;
};

int *BitStream::ReadBits(int n_bits)
{
    int *bits = new int[n_bits];
    for (int i = 0; i < n_bits; i++)
    {
        bits[i] = ReadBit();
    }
    return bits;
};

void BitStream::WriteBit(int bit)
{
    if (bit != 0 && bit != 1)
    {
        std::cerr << "BitStream::WriteBit: bit must be 0 or 1, input will be ignored: " << bit << std::endl;
        return;
    }
    out << bit;
};

void BitStream::WriteBits(int *bits)
{
    int size = sizeof(bits) / sizeof(int);

    for (int i = 0; i < size; i++)
    {
        WriteBit(bits[i]);
    }
};
