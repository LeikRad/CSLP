#include "Golomb.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include "BitStream.h"

Golomb::Golomb(int M, const char *OutputFileName) : M(M)
{
    out.open(OutputFileName, std::ios::binary);
}

Golomb::~Golomb()
{
    Close();
}

void Golomb::Close()
{
    if (out.is_open())
    {
        out.close();
    }
    if (in.is_open())
    {
        in.close();
    }
}

void Golomb::Encode(int num)
{
    // u_char byte = buffer[0];
    u_char q = num / M;
    u_char r = num % M;

    for (int i = 0; i < q; i++)
    {
        out << 1;
    }

    out << 0;

    u_char b = log2(M);

    if (r < pow(2, b + 1))
    {
        for (int i = b - 1; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
    else
    {
        r += pow(2, b + 1) - M;
        for (int i = b; i >= 0; i--)
        {
            out << (r >> i) % 2;
        }
    }
};

int Golomb::Decode()
{
    BitStream bs = BitStream("test.txt", "out.txt");
    int q = 0;

    // Read quotient bits
    while (bs.ReadBit() == 1)
    {
        q++;
    }

    int b = static_cast<int>(log2(M));

    int r = 0;
    // Read the rest of the bits as remainder
    for (int i = b - 1; i >= 0; i--)
    {
        r = (r << 1) | bs.ReadBit();
    }

    // Combine quotient and remainder to decode the number
    int num = (q * M) + r;

    // Adjust for negative numbers
    if (num % 2 == 1)
    {
        num = -(num + 1) / 2;
    }
    else
    {
        num = num / 2;
    }

    return num;
}