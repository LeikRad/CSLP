#include <iostream>
struct Pixel
{
    u_char r;
    u_char g;
    u_char b;
};

class Image
{
private:
    int width;
    int height;
    int maxVal;
    Pixel *pixels; // Pointer to a array (which is a pointer in itself) so pointer to pointer

public:
    Image(char *FileName);
    ~Image();
    int *Metadata();
    Pixel *PixelData();
    void WriteImage(char *FileName);
    void WriteImageWaterMark(char *FileName);
};
