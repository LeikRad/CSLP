#include <fstream>
#include <vector>

struct WriteBitReturn
{
    int *bits;
    int size;
};
class BitStream
{
private:
    std::ifstream in;
    std::ofstream out;
    short read_pos;
    short write_pos;
    u_char read_buffer;
    u_char write_buffer;

public:
    BitStream(const char *FileName, const char *OutputFileName);
    ~BitStream();
    int ReadBit();
    WriteBitReturn ReadBits(int n_bits);
    void WriteBit(int bit);
    void WriteBits(int *bits, int size);
    void Flush();
};