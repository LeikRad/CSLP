// write main
#include "Golomb.h"
#include <iostream>

int main() {
    Golomb encoder(8, "encoded.bin"); // Initialize Golomb encoder with M=4

    int num1 = 69;

    encoder.Encode(num1);
    encoder.Close();

    // Print the encoded values in binary
    std::ifstream encodedFile("encoded.bin", std::ios::binary);
    if (encodedFile.is_open()) {
        char bit;
        std::cout << "Encoded values in binary: ";
        while (encodedFile.get(bit)) {
            std::cout << bit;
        }
        std::cout << std::endl;
        encodedFile.close();
    }

    Golomb decoder(8, "encoded.bin"); // Initialize Golomb decoder with M=4
    int decoded1 = decoder.Decode();
    int decoded2 = decoder.Decode();
    decoder.Close();

    std::cout << "Decoded values: " << decoded1 << " and " << decoded2 << std::endl;

    return 0;
}
