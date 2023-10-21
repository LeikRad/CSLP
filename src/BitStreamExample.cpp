#include "BitStream.h"
#include <iostream>

int main(int argc, char *argv[])
{
    BitStream *bs = new BitStream("test.txt", "out.bin");

    int bit;
    while ((bit = bs->ReadBit()) != -1)
    {
        std::cout << bit;
        bs->WriteBit(bit);
    }

    // delete bs;
    delete bs;

    bs = new BitStream("test.txt", "out2.bin");

    int *bits;

    std::cout << std::endl;
    while (true)
    {
        bits = bs->ReadBits(1);
        if (bits == NULL)
        {
            break;
        }
        int size = sizeof(bits) / sizeof(int);
        std::cout << size << std::endl;
    }
}
