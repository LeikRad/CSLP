// write main
#include "Golomb.h"
#include "BitStream.h"
#include <iostream>

int main()
{
    // write to encoded.bin encoded values, read from encoded.bin and decode
    BitStream bs("encoded.bin", BitStream::WRITE);

    Golomb codec(bs, 8); // Initialize Golomb encoder with M=4

    int n = -50;
    while (n != 51)
    {
        codec.Encode(n++);
    }

    // Print the encoded values in binary
    std::ifstream encodedFile("encoded.bin", std::ios::binary);
    if (encodedFile.is_open())
    {
        char bit;
        std::cout << "Encoded values in binary: ";
        while (encodedFile.get(bit))
        {
            std::cout << bit;
        }
        std::cout << std::endl;
        encodedFile.close();
    }

    bs.close_output();
    bs.open_input_file("encoded.bin");

    n = -50;
    while (n != 51)
    {
        std::cout << "Decode: " << codec.Decode() << std::endl;
        n++;
    }

    // return 0;
}
