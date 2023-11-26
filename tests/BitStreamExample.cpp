#include "BitStream.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    BitStream *bs = new BitStream("test.txt", "out.bin");

    int bit;
    while ((bit = bs->ReadBit()) != -1)
    {
        cout << bit;
        bs->WriteBit(bit);
    }

    delete bs;
    // delete bs;

    bs = new BitStream("out.bin", "out2.bin");

    int *bits;

    cout << endl;
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
            cout << bits[i];
        }
        bs->WriteBits(bits, size);
    }
}
