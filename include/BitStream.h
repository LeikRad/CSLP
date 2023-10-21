#include <fstream>
#include <vector>

class BitStream
{
private:
    std::ifstream in;
    std::ofstream out;
    short pos;
    u_char byte;

public:
    BitStream(const char *FileName, const char *OutputFileName);
    ~BitStream();
    int ReadBit();
    int *ReadBits(int n_bits);
    void WriteBit(int bit);
    void WriteBits(int *bits);
};