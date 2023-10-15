#include <fstream>

class Golomb {
private:
    int M;
    std::ofstream out;
    std::ifstream in;

public:
    Golomb(int M, const char* OutputFileName);
    ~Golomb();

    void Encode(int num);
    int Decode();

    void Close();
};