// write main
#include "Golomb.h"
#include <iostream>

int main()
{
    // write to encoded.bin encoded values, read from encoded.bin and decode
    Golomb codec(8, "encoded.bin", "encoded.bin"); // Initialize Golomb encoder with M=4

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

    n = -50;
    while (n != 51)
    {
        std::cout << "Decode: " << codec.Decode() << std::endl;
        n++;
    }

    // return 0;
}
