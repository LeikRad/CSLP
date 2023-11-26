#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // Open the video file (replace "video.mp4" with your video file)
    VideoCapture cap("../../static/videos/video.mp4");

    if (!cap.isOpened())
    {
        cerr << "Error: Could not open video file." << endl;
        return -1;
    }

    namedWindow("RGB Video", WINDOW_NORMAL);
    namedWindow("YUV 4:4:4", WINDOW_NORMAL);
    namedWindow("YUV 4:2:2", WINDOW_NORMAL);
    namedWindow("YUV 4:2:0", WINDOW_NORMAL);
    while (true)
    {
        Mat frame;     // RGB frame
        Mat yuvFrame;  // YUV 4:4:4 frame
        Mat yuvFrame2; // YUV 4:2:2 frame
        Mat yuvFrame3; // YUV 4:2:0 frame
        // Read a frame from the video
        cap >> frame;

        // Check for end of video
        if (frame.empty())
        {
            break;
        }

        // Convert RGB to YUV

        double Wr = 0.299;
        double Wb = 0.114;
        double Wg = 1 - Wr - Wb;
        double Umax = 0.436;
        double Vmax = 0.615;

        yuvFrame = frame.clone();
        yuvFrame2 = frame.clone();
        yuvFrame3 = frame.clone();

        for (int i = 0; i < frame.rows; i++)
        {
            for (int j = 0; j < frame.cols; j++)
            {

                // Get RGB values
                Vec3b rgb = frame.at<Vec3b>(i, j);
                double r = rgb[2] / 255.0;
                double g = rgb[1] / 255.0;
                double b = rgb[0] / 255.0;

                // Convert RGB to YUV
                double y = Wr * r + Wg * g + Wb * b;
                double u = Umax * ((b - y) / (1 - Wb));
                double v = Vmax * ((r - y) / (1 - Wr));

                // Set YUV values for 4:4:4
                yuvFrame.at<Vec3b>(i, j)[0] = y * 255;
                yuvFrame.at<Vec3b>(i, j)[1] = (u + 0.5) * 255;
                yuvFrame.at<Vec3b>(i, j)[2] = (v + 0.5) * 255;

                // set the YUV values for 4:2:2
                yuvFrame2.at<Vec3b>(i, j)[0] = y * 255;
                if (j % 2 == 0)
                {
                    yuvFrame2.at<Vec3b>(i, j)[1] = (u + 0.5) * 255;
                    yuvFrame2.at<Vec3b>(i, j)[2] = (v + 0.5) * 255;
                }
                else
                {
                    yuvFrame2.at<Vec3b>(i, j)[1] = yuvFrame2.at<Vec3b>(i, j - 1)[1];
                    yuvFrame2.at<Vec3b>(i, j)[2] = yuvFrame2.at<Vec3b>(i, j - 1)[2];
                }

                // set the YUV values for 4:2:0
                yuvFrame3.at<Vec3b>(i, j)[0] = y * 255;
                if (i % 2 == 0 && j % 2 == 0)
                {
                    yuvFrame3.at<Vec3b>(i, j)[1] = (u + 0.5) * 255;
                    yuvFrame3.at<Vec3b>(i, j)[2] = (v + 0.5) * 255;
                }
                else if (i % 2 == 0 && j % 2 != 0)
                {
                    yuvFrame3.at<Vec3b>(i, j)[1] = yuvFrame3.at<Vec3b>(i, j - 1)[1];
                    yuvFrame3.at<Vec3b>(i, j)[2] = yuvFrame3.at<Vec3b>(i, j - 1)[2];
                }
                else if (i % 2 != 0 && j % 2 == 0)
                {
                    yuvFrame3.at<Vec3b>(i, j)[1] = yuvFrame3.at<Vec3b>(i - 1, j)[1];
                    yuvFrame3.at<Vec3b>(i, j)[2] = yuvFrame3.at<Vec3b>(i - 1, j)[2];
                }
                else
                {
                    yuvFrame3.at<Vec3b>(i, j)[1] = yuvFrame3.at<Vec3b>(i - 1, j - 1)[1];
                    yuvFrame3.at<Vec3b>(i, j)[2] = yuvFrame3.at<Vec3b>(i - 1, j - 1)[2];
                }
            }
        }
        imshow("RGB Video", frame);

        imshow("YUV 4:4:4", yuvFrame);
        imshow("YUV 4:2:2", yuvFrame2);
        imshow("YUV 4:2:0", yuvFrame3);
        char key = waitKey(30);
        if (key == 27)
        { // Exit when the Esc key is pressed
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
