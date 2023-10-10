
class Colomb
{
private:
    int M;
public:
    Colomb(int M);
    ~Colomb();
    void Encode(char *FileName, char *OutputFileName);
    void Decode(char *FileName, char *OutputFileName);
};