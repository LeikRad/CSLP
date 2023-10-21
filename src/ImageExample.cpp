#include "Image.h"
#include <iostream>
using namespace std;

int main()
{

    Image img("../../images/bike3.ppm");

    img.WriteImage("../../images/bike3Copy.ppm");

    return 0;
}