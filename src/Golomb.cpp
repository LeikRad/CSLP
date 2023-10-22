/**
 * @file Golomb.cpp
 * @brief Implementation of the Golomb class
 */

#include "Golomb.h"
#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <cmath>

/**
 * @brief Constructs a new Golomb encoder/decoder with the specified divisor and input/output file names
 *
 * The constructor initializes a new BitStream object with the specified input and output file names.
 *
 * @param M The divisor used for Golomb encoding/decoding
 * @param InputFileName The name of the input file
 * @param OutputFileName The name of the output file
 */
Golomb::Golomb(int M, const char *InputFileName, const char *OutputFileName) : M(M)
{
    this->bs = new BitStream(InputFileName, OutputFileName);
}

/**
 * @brief Destroys the Golomb encoder/decoder and closes the input/output file
 *
 * The destructor flushes the BitStream buffer and deletes the BitStream object.
 */
Golomb::~Golomb()
{
    Close();
}

/**
 * @brief Closes the input/output file and frees the BitStream object
 *
 * The function flushes the BitStream buffer and deletes the BitStream object.
 */
void Golomb::Close()
{
    bs->Flush();
    delete bs;
}

/**
 * @brief Encodes a integer using Golomb encoding
 *
 * The function maps the input number to a new number using the map function, and then encodes the new number using Golomb encoding.
 *
 * @param num The integer to be encoded
 */
void Golomb::Encode(int num)
{
    num = map(num);
    u_char q = num / M;
    u_char r = num % M;

    for (int i = 0; i < q; i++)
    {
        std::cout << "1";
        bs->WriteBit(1);
    }
    std::cout << "0";
    bs->WriteBit(0);

    u_char b = log2(M);

    if (r < pow(2, b + 1))
    {
        for (int i = b - 1; i >= 0; i--)
        {
            std::cout << (r >> i) % 2;
            bs->WriteBit((r >> i) % 2);
        }
    }
    else
    {
        r += pow(2, b + 1) - M;
        for (int i = b; i >= 0; i--)
        {
            std::cout << (r >> i) % 2;
            bs->WriteBit((r >> i) % 2);
        }
    }
};

/**
 * @brief Decodes a integer using Golomb decoding
 *
 * The function decodes a integer using Golomb decoding and the current divisor M.
 *
 * @return int The decoded integer
 */
int Golomb::Decode()
{
    int q = 0;

    // Read quotient bits
    while (bs->ReadBit() == 1)
    {
        q++;
    }

    u_char b = log2(M);

    int r = 0;
    // Read the rest of the bits as remainder
    for (int i = b - 1; i >= 0; i--)
    {
        r = (r << 1) | bs->ReadBit();
    }
    // Combine quotient and remainder to decode the number
    int num = (q * M) + r;

    // Adjust for negative numbers
    num = reverse_map(num);

    return num;
}

/**
 * @brief Maps a integer to a new integer
 *
 * The function maps a integer to a new integer using the formula (2 * num + 1) / (2 * M).
 *
 * @param num The integer to be mapped
 * @return int The mapped integer
 */
int Golomb::map(int num)
{
    if (num >= 0)
    {
        return 2 * num;
    }
    else
    {
        return -2 * num - 1;
    }
}

/**
 * @brief Unmaps a integer to its original value
 *
 * The function unmaps a integer to its original value using the formula q * M + r.
 *
 * @param num The integer to be unmapped
 * @return int The unmapped integer
 */
int Golomb::reverse_map(int num)
{
    if (num % 2 == 0)
    {
        return num / 2;
    }
    else
    {
        return -(num + 1) / 2;
    }
}