#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    // Open the video file (replace "video.mp4" with your video file)
    VideoCapture cap("../videos/video.mp4");

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }


    namedWindow("YUV Video", WINDOW_NORMAL);
    namedWindow("RGB Video", WINDOW_NORMAL);
    namedWindow("YUV Video 2", WINDOW_NORMAL);
    // namedWindow("YUV Video 3", WINDOW_NORMAL);
    namedWindow("YUV Channel Y", WINDOW_NORMAL);
    namedWindow("YUV Channel U", WINDOW_NORMAL);
    namedWindow("YUV Channel V", WINDOW_NORMAL);
    
    Mat uLut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++) {
        uLut.at<uchar>(i) = i;
    }

    Mat vLut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++) {
        vLut.at<uchar>(i) = i;
    }

    while (true) {
        Mat frame; // RGB frame
        Mat yuvFrame; // YUV frame
        Mat yuvFrame2; // YUV frame
        Mat yuvChannels[3];
        // Read a frame from the video
        cap >> frame;

        // Check for end of video
        if (frame.empty()) {
            break;
        }

        // Convert RGB to YUV
        cvtColor(frame, yuvFrame, COLOR_BGR2YUV);
        
        double Wr = 0.299;
        double Wb = 0.114;
        double Wg = 1 - Wr - Wb;
        double Umax = 0.436;
        double Vmax = 0.615;

        yuvFrame2 = frame.clone();

        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {

                // Get RGB values
                Vec3b rgb = frame.at<Vec3b>(i, j);
                double r = rgb[2] / 255.0;
                double g = rgb[1] / 255.0;
                double b = rgb[0] / 255.0;

                // Convert RGB to YUV
                double y = Wr * r + Wg * g + Wb * b;
                double u = Umax * ((b - y) / (1 - Wb));
                double v = Vmax * ((r - y) / (1 - Wr));

                // Set YUV values
                yuvFrame2.at<Vec3b>(i, j)[0] = y * 255;
                yuvFrame2.at<Vec3b>(i, j)[1] = (u + 0.5) * 255;
                yuvFrame2.at<Vec3b>(i, j)[2] = (v + 0.5) * 255;
            }
        }
        
        imshow("RGB Video", frame);
        imshow("YUV Video", yuvFrame);
        imshow("YUV Video 2", yuvFrame2);
        split(yuvFrame2, yuvChannels);
        cvtColor(yuvChannels[0], yuvChannels[0], COLOR_GRAY2BGR);
        cvtColor(yuvChannels[1], yuvChannels[1], COLOR_GRAY2BGR);
        cvtColor(yuvChannels[2], yuvChannels[2], COLOR_GRAY2BGR);   
        imshow("YUV Channel Y", yuvChannels[0]);
        imshow("YUV Channel U", yuvChannels[1]);
        imshow("YUV Channel V", yuvChannels[2]);     
        // cout << yuvChannels[0].channels() << endl;
        // cout << yuvChannels[1].channels() << endl;
        // cout << yuvChannels[2].channels() << endl;
        // Mat yuvFrame3;
        // yuvFrame3 = frame.clone();

        // LUT(yuvChannels[1], uLut, yuvChannels[1]);
        // LUT(yuvChannels[2], vLut, yuvChannels[2]);

        // mixChannels(yuvChannels[0], yuvFrame3, {0, 0});
        // mixChannels(yuvChannels[1], yuvFrame3, {1, 1});
        // mixChannels(yuvChannels[2], yuvFrame3, {2, 2});
        // imshow("YUV Video 3", yuvFrame3);
        char key = waitKey(30);
        if (key == 27) { // Exit when the Esc key is pressed
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
