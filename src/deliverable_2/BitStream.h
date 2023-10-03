#include <fstream>
#include <vector>

class BitStream
{
private:
    FILE *pFile;
    std::ofstream out;
    std::vector<int> readbuffer;
    short pos;
    u_char byte;

public:
    BitStream(char *FileName, char *OutputFileName);
    ~BitStream();
    int ReadBit();
    int* ReadBits(int n_bits);
    void WriteBit(int bit);
    void WriteBits(int *bits);
};