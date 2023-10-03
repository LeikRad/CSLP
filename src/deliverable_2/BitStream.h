#include <fstream>
#include <vector>

class BitStream
{
private:
    FILE *pFile;
    int M;
    std::vector<u_char> buffer;
    std::ofstream out;

public:
    BitStream(char *FileName, int M, char *OutputFileName);
    ~BitStream();
    int ReadBit();
    int ReadByte(int n_bits);
    void WriteBit();
    void WriteBits(int n_bits);
};