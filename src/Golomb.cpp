
#include "Golomb.h"
#include "BitStream.h"
#include <iostream>
#include <fstream>
#include <cmath>

Golomb::Golomb(const char *InputFileName, const char *OutputFileName, int M) : M(M)
{
    this->bs = new BitStream(InputFileName, OutputFileName);
    this->b = ceil(log2(M));
}

Golomb::~Golomb()
{
    Close();
}

void Golomb::Close()
{
    bs->Flush();
    delete bs;
}

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

void Golomb::setM(int M)
{
    if (M <= 0)
    {
        return;
    }
    this->M = M;
    this->b = ceil(log2(M));
}

int Golomb::getM()
{
    return M;
}