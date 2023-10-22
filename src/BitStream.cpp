/**
 * @file BitStream.cpp
 * @brief Implementation file for the BitStream class
 *
 * This implementation file defines the methods of the BitStream class, which provides a way to read and write individual bits from a binary file.
 */

#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <filesystem>

/**
 * @brief Construct a new BitStream object from a file
 *
 * The constructor reads a binary file from disk and creates a new `BitStream` object from it.
 * The constructor also initializes the read and write buffers and positions.
 *
 * @param FileName The name of the binary file to read
 * @param OutputFileName The name of the binary file to write (default is "out.bin")
 *
 * @throw std::runtime_error if the file fails to open
 */
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

/**
 * @brief Destroy the BitStream object and close the input and output files
 *
 * The destructor closes the input and output files used by the `BitStream` object.
 */
BitStream::~BitStream()
{
    in.close();
    out.close();
};

/**
 * @brief Read a single bit from the input file
 *
 * The `ReadBit` method reads a single bit from the input file and returns its value.
 * If the end of the file is reached, the method returns -1.
 *
 * @return The value of the next bit in the input file, or -1 if the end of the file is reached
 */
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

/**
 * @brief Read a specified number of bits from the input file
 *
 * The `ReadBits` method reads a specified number of bits from the input file and returns them as an array of integers.
 * If the end of the file is reached before all bits are read, the method returns the bits that were read.
 *
 * @param n_bits The number of bits to read
 *
 * @return A `WriteBitReturn` struct containing an array of integers representing the bits that were read, and the number of bits that were read
 */
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

/**
 * @brief Write a single bit to the output file
 *
 * The `WriteBit` method writes a single bit to the output file.
 * If the input bit is not 0 or 1, the method prints an error message and ignores the input.
 *
 * @param bit The bit to write (must be 0 or 1)
 */
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

/**
 * @brief Write an array of bits to the output file
 *
 * The `WriteBits` method writes an array of bits to the output file.
 * If any of the input bits are not 0 or 1, the method prints an error message and ignores the input.
 *
 * @param bits An array of integers representing the bits to write (must be 0 or 1)
 * @param size The number of bits to write
 */
void BitStream::WriteBits(int *bits, int size)
{
    for (int i = 0; i < size; i++)
    {
        WriteBit(bits[i]);
    }
};

/**
 * @brief Flush any remaining bits to the output file
 *
 * The `Flush` method writes any remaining bits in the write buffer to the output file.
 * If the write buffer is already empty, the method does nothing.
 */
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
