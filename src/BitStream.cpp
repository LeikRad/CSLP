#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <filesystem>

BitStream::BitStream(const char *FileName, const char *OutputFileName = "out.bin")
{
    this->in = std::ifstream(FileName, std::ios::binary);
    read_pos = -1;
    read_buffer = 0;

    write_pos = 7;
    write_buffer = 0;

    if (!in)
    {
        throw std::runtime_error("Failed to open the file.");
    }

    this->out = std::ofstream(OutputFileName, std::ios::binary | std::ios::out);

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
    if (read_pos < 0)
    {
        in.read((char *)&read_buffer, sizeof(u_char));
        if (in.eof())
        {
            return -1;
        }
        read_pos = 7;
    }
    return (read_buffer >> read_pos--) % 2;
};

WriteBitReturn BitStream::ReadBits(int n_bits)
{
    int *bits = new int[n_bits];
    for (int i = 0; i < n_bits; i++)
    {
        int bit = ReadBit();
        if (bit == -1)
        {
            int *new_bits = new int[i];
            for (int j = 0; j < i; j++)
            {
                new_bits[j] = bits[j];
            }
            return {new_bits, i};
        }
        bits[i] = bit;
    }
    return {bits, n_bits};
};

void BitStream::WriteBit(int bit)
{
    if (bit != 0 && bit != 1)
    {
        std::cerr << "BitStream::WriteBit: bit must be 0 or 1, input will be ignored: " << bit << std::endl;
        return;
    }

    write_buffer |= bit << write_pos--;
    if (write_pos < 0)
    {
        out.write((char *)&write_buffer, sizeof(u_char));
        out.flush();
        write_pos = 7;
        write_buffer = 0;
    }
};

void BitStream::WriteBits(int *bits, int size)
{
    for (int i = 0; i < size; i++)
    {
        WriteBit(bits[i]);
    }
};

void BitStream::Flush()
{
    if (write_pos < 7)
    {
        out.write((char *)&write_buffer, sizeof(u_char));
        out.flush();
        write_pos = 7;
        write_buffer = 0;
    }
};
