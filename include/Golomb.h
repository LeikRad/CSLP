#include <fstream>

class BitStream;
class Golomb
{
private:
    int M;
    BitStream *bs;

public:
    Golomb(int M, const char *InputFileName, const char *OutputFileName);
    ~Golomb();

    void Encode(int num);
    int Decode();

    void Close();

    int map(int num);
    int reverse_map(int num);
};