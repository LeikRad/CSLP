// write main
#include "Golomb.hpp"
#include "BitStream.hpp"
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
    ifstream encodedFile("encoded.bin", ios::binary);
    if (encodedFile.is_open())
    {
        char bit;
        cout << "Encoded values in binary: ";
        while (encodedFile.get(bit))
        {
            cout << bit;
        }
        cout << endl;
        encodedFile.close();
    }

    bs.close_output();
    bs.open_input_file("encoded.bin");

    n = -50;
    while (n != 51)
    {
        cout << "Decode: " << codec.Decode() << endl;
        n++;
    }

    // return 0;
}
