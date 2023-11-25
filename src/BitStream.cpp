
#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <filesystem>

BitStream::BitStream(const char *FileName, const char *OutputFileName) : BitStream()
{
    open_input_file(FileName);
    open_output_file(OutputFileName);
};

BitStream::BitStream(const char *FileName, BitStream::Mode mode) : BitStream()
{
    if (mode == BitStream::WRITE)
    {
        open_output_file(FileName);
    }
    else
    {
        open_input_file(FileName);
    }
};

BitStream::BitStream() : read_pos(-1), read_buffer(0), write_pos(7), write_buffer(0), in(std::ifstream()), out(std::ofstream())
{
}

BitStream::~BitStream()
{
    in.close();
    out.close();
};

bool BitStream::open_input_file(const char *FileName)
{
    if (in.is_open())
    {
        in.close();
    }
    in.open(FileName, std::ios::binary);
    if (!in)
    {
        std::cerr << "BitStream::open_input_file: failed to open file: " << FileName << std::endl;
        return false;
    }
    return true;
};

bool BitStream::open_output_file(const char *OutputFileName)
{
    if (out.is_open())
    {
        out.close();
    }
    out.open(OutputFileName, std::ios::binary);
    if (!out)
    {
        std::cerr << "BitStream::open_output_file: failed to open file: " << OutputFileName << std::endl;
        return false;
    }
    return true;
}

bool BitStream::can_write()
{
    return out.is_open();
};

bool BitStream::eof()
{
    return in.eof();
};

bool BitStream::can_read()
{
    return in.is_open() && !in.eof();
};

void BitStream::close_input()
{
    if (in.is_open())
        in.close();
}

void BitStream::close_output()
{
    if (out.is_open())
        out.close();
}

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