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

    delete bs;
    // delete bs;

    bs = new BitStream("out.bin", "out2.bin");

    int *bits;

    std::cout << std::endl;
    int control = 0;
    while (control == 0)
    {
        auto [bits, size] = bs->ReadBits(32);
        if (size != 32)
        {
            control = 1;
            bs->Flush();
        }
        for (int i = 0; i < size; i++)
        {
            std::cout << bits[i];
        }
        bs->WriteBits(bits, size);
    }
}
