// write main
#include "BitStream.h"
#include <iostream>

int main(int argc, char *argv[])
{
    BitStream bs = BitStream("test.txt", "out.txt");

    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;
    std::cout << bs.ReadBit() << std::endl;

    bs.WriteBit(0x1);
}
