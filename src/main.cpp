#include "image.h"
#include <iostream>
using namespace std;

int main()
{
    FILE *file = fopen("../images/bike3.ppm", "rb");

    if (!file)
    {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }
    Image img(file);

    fclose(file);

    // file = fopen("../images/airplane.ppm", "rb");

    // if (!file)
    // {
    //     std::cerr << "Failed to open the file." << std::endl;
    //     return 1;
    // }

    // Image img2(file);
    
    // fclose(file);

    img.WriteImage("../images/airplane2.ppm");

    return 0;
}