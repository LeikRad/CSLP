#include "YUVReader.hpp"

int main(int argc, char const *argv[])
{
    YUVReader in(argv[1]);

    while (in.nextFrame_exists())
    {
        Mat frame = in.get_nextFrame();
        imshow("Frame", frame);
        waitKey(1);
    }

    cout << in.get_file_header() << endl;
    return 0;
}
