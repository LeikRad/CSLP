#include "image.h"
#include <iostream>
using namespace std;

int main()
{

    Image img("../images/bike3.ppm");

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