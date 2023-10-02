#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int video()
{
    VideoCapture cap("video.mp4");
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }
    Mat frame; // Your video frame in RGB format
    Mat yuvFrame;

    cvtColor(frame, yuvFrame, COLOR_BGR2YUV);

    imshow("YUV Video", yuvFrame);

    while (true)
    {
        cap >> frame;
        if (frame.empty())
        {
            break;
        }
        // Perform RGB to YUV conversion if needed
        // Display frames
        imshow("Video Player", frame);

        char key = waitKey(30);
        if (key == 27)
        { // Escape key to exit
            break;
        }
    }

    cap.release();
    destroyAllWindows();
}