#include <iostream>
using namespace std;

class Image
{
private:
    int width;
    int height;
    int maxVal;
    int[] pixels;

public:
    int printNum(void);
    Image(int width, int height, int maxVal);
};