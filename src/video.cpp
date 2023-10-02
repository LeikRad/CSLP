#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // Open the video file (replace "video.mp4" with your video file)
    VideoCapture cap("../videos/video.mp4");

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    namedWindow("YUV Video", WINDOW_NORMAL);
    namedWindow("RGB Video", WINDOW_NORMAL);

    while (true) {
        Mat frame; // RGB frame
        Mat yuvFrame; // YUV frame

        // Read a frame from the video
        cap >> frame;

        // Check for end of video
        if (frame.empty()) {
            break;
        }

        // Convert RGB to YUV
        cvtColor(frame, yuvFrame, COLOR_BGR2YUV);

        // Display frames
        imshow("RGB Video", frame);
        imshow("YUV Video", yuvFrame);

        char key = waitKey(30);
        if (key == 27) { // Exit when the Esc key is pressed
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
