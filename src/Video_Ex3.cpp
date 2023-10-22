#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// variables for settings:
// Watermark
Mat watermarkImage = imread("../../static/images/baboon.ppm", IMREAD_UNCHANGED);
int watermarkSize = watermarkImage.rows * watermarkImage.cols;

// YUV
double Wr = 0.299;
double Wb = 0.114;
double Wg = 1 - Wr - Wb;
double Umax = 0.436;
double Vmax = 0.615;

// Histogram
int histSize = 256;
float range[] = {0, 256};

// Gaussian Blur / Blur
int kernelSize = 5;
double sigma = 3;

// Threshold
int threshold = 156;

Mat applyWatermark(Mat frame)
{
    int size = frame.rows * frame.cols;
    if (size < watermarkSize)
    {
        return frame;
    }
    for (int y = 0; y < watermarkImage.rows; y++)
    {
        for (int x = 0; x < watermarkImage.cols; x++)
        {
            // get pixel values, ignoring alpha channel
            Vec3b pixel = watermarkImage.at<Vec3b>(y, x);

            // add watermark to bottom right corner
            frame.at<Vec3b>(frame.rows - watermarkImage.rows + y, frame.cols - watermarkImage.cols + x) = pixel;
        }
    }
    return frame;
}
Mat toYuv(Mat frame)
{

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
            frame.at<Vec3b>(i, j)[0] = y * 255;
            frame.at<Vec3b>(i, j)[1] = (u + 0.5) * 255;
            frame.at<Vec3b>(i, j)[2] = (v + 0.5) * 255;
        }
    }
    return frame;
}

void showHistogram(Mat frame)
{

    const float *histRange = {range};
    Mat histR(histSize, 1, CV_32F, Scalar(0));
    Mat histG(histSize, 1, CV_32F, Scalar(0));
    Mat histB(histSize, 1, CV_32F, Scalar(0));

    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            Vec3b rgb = frame.at<Vec3b>(i, j);
            histR.at<float>(rgb[2])++;
            histG.at<float>(rgb[1])++;
            histB.at<float>(rgb[0])++;
        }
    }

    normalize(histR, histR, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, 1, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, 1, NORM_MINMAX, -1, Mat());

    int plotWidth = 512;
    int plotHeight = 400;
    Mat plot(plotHeight, plotWidth, CV_8UC3, Scalar(255, 255, 255));

    int binWidth = cvRound((double)plotWidth / histSize);
    // Draw grid lines
    for (int i = 0; i < plotHeight; i += 20)
    {
        line(plot, Point(0, i), Point(plotWidth, i), Scalar(200, 200, 200), 1, LINE_AA);
    }
    for (int i = 0; i < plotWidth; i += 32)
    {
        line(plot, Point(i, 0), Point(i, plotHeight), Scalar(200, 200, 200), 1, LINE_AA);
    }

    // Draw x and y axes
    line(plot, Point(0, plotHeight - 1), Point(plotWidth - 1, plotHeight - 1), Scalar(0, 0, 0), 2, LINE_AA);
    line(plot, Point(0, plotHeight - 1), Point(0, 0), Scalar(0, 0, 0), 2, LINE_AA);

    // // Draw tick marks and labels on x axis
    // for (int i = 0; i < histSize; i += 32) {
    //     line(plot, Point(i * binWidth, plotHeight - 1), Point(i * binWidth, plotHeight - 5), Scalar(0, 0, 0), 2, LINE_AA);
    //     char buffer[16];
    //     sprintf(buffer, "%d", i);
    //     putText(plot, buffer, Point(i * binWidth, plotHeight - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    // }

    // Draw tick marks and labels on y axis
    for (int i = 0; i <= 10; i++)
    {
        line(plot, Point(0, plotHeight - 1 - i * plotHeight / 10), Point(5, plotHeight - 1 - i * plotHeight / 10), Scalar(0, 0, 0), 2, LINE_AA);
        char buffer[16];
        sprintf(buffer, "%d%%", i * 10);
        putText(plot, buffer, Point(10, plotHeight - 1 - i * plotHeight / 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    }

    for (int i = 1; i < histSize; i++)
    {
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histR.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histR.at<float>(i) * plotHeight)), Scalar(0, 0, 255), 2, LINE_AA);
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histG.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histG.at<float>(i) * plotHeight)), Scalar(0, 255, 0), 2, LINE_AA);
        line(plot, Point(binWidth * (i - 1), plotHeight - cvRound(histB.at<float>(i - 1) * plotHeight)),
             Point(binWidth * i, plotHeight - cvRound(histB.at<float>(i) * plotHeight)), Scalar(255, 0, 0), 2, LINE_AA);
    }

    // Add labels and title to the plot
    // putText(plot, "Pixel Value", Point(plotWidth / 2, plotHeight - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    // putText(plot, "Frequency", Point(10, plotHeight / 2), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);
    putText(plot, "Frequency (%) / Pixel Value", Point(plotWidth / 2, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, LINE_AA);

    imshow("Histogram", plot);
}

Mat applyGrayScale(Mat frame)
{
    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            Vec3b rgb = frame.at<Vec3b>(i, j);
            int gray = (int)(0.299 * rgb[2] + 0.587 * rgb[1] + 0.114 * rgb[0]);
            frame.at<Vec3b>(i, j) = Vec3b(gray, gray, gray);
        }
    }
    return frame;
}

Mat applyEqualization(Mat frame)
{
    Mat src(frame.rows, frame.cols, CV_8UC1);
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    Mat channels[3];
    split(hsv, channels);

    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    Mat hist;
    calcHist(&channels[2], 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

    equalizeHist(channels[2], channels[2]);

    Mat merged;
    merge(channels, 3, merged);

    Mat dst;
    cvtColor(merged, dst, COLOR_HSV2BGR);

    memcpy(frame.data, dst.data, frame.rows * frame.cols * 3);
    return frame;
}

Mat applyGaussianBlur(Mat frame)
{

    double kernel[kernelSize][kernelSize];
    double sum = 0;
    int radius = kernelSize / 2;
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            kernel[x + radius][y + radius] = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            sum += kernel[x + radius][y + radius];
        }
    }

    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] /= sum;
        }
    }

    // Apply the kernel to frame
    Mat dst(frame.rows, frame.cols, CV_8UC3);
    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            double sumR = 0, sumG = 0, sumB = 0;
            for (int x = -radius; x <= radius; x++)
            {
                for (int y = -radius; y <= radius; y++)
                {
                    int r = i + x;
                    int c = j + y;
                    if (r >= 0 && r < frame.rows && c >= 0 && c < frame.cols)
                    {
                        sumR += frame.at<Vec3b>(r, c)[2] * kernel[x + radius][y + radius];
                        sumG += frame.at<Vec3b>(r, c)[1] * kernel[x + radius][y + radius];
                        sumB += frame.at<Vec3b>(r, c)[0] * kernel[x + radius][y + radius];
                    }
                }
            }
            dst.at<Vec3b>(i, j)[2] = sumR;
            dst.at<Vec3b>(i, j)[1] = sumG;
            dst.at<Vec3b>(i, j)[0] = sumB;
        }
    }
    memcpy(frame.data, dst.data, frame.rows * frame.cols * 3);
    return frame;
}

Mat applyBlur(Mat frame)
{

    double kernel[kernelSize][kernelSize];
    double value = 1.0 / (kernelSize * kernelSize);
    for (int i = 0; i < kernelSize; i++)
    {
        for (int j = 0; j < kernelSize; j++)
        {
            kernel[i][j] = value;
        }
    }

    // Apply the kernel to frame
    Mat dst(frame.rows, frame.cols, CV_8UC3);
    int radius = kernelSize / 2;
    for (int i = 0; i < frame.rows; i++)
    {
        for (int j = 0; j < frame.cols; j++)
        {
            double sumR = 0, sumG = 0, sumB = 0;
            for (int x = -radius; x <= radius; x++)
            {
                for (int y = -radius; y <= radius; y++)
                {
                    int r = i + x;
                    int c = j + y;
                    if (r >= 0 && r < frame.rows && c >= 0 && c < frame.cols)
                    {
                        sumR += frame.at<Vec3b>(r, c)[2] * kernel[x + radius][y + radius];
                        sumG += frame.at<Vec3b>(r, c)[1] * kernel[x + radius][y + radius];
                        sumB += frame.at<Vec3b>(r, c)[0] * kernel[x + radius][y + radius];
                    }
                }
            }
            dst.at<Vec3b>(i, j)[2] = sumR;
            dst.at<Vec3b>(i, j)[1] = sumG;
            dst.at<Vec3b>(i, j)[0] = sumB;
        }
    }
    memcpy(frame.data, dst.data, frame.rows * frame.cols * 3);
    return frame;
}

Mat applyThreshold(Mat frame)
{
    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            Vec3b pixel = frame.at<Vec3b>(y, x);
            int gray = (pixel[0] + pixel[1] + pixel[2]) / 3;
            if (gray >= ::threshold)
            {
                frame.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
            }
            else
            {
                frame.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
            }
        }
    }
    return frame;
}
int main()
{
    // Open the video file (replace "video.mp4" with your video file)
    VideoCapture cap("../../static/videos/video.mp4");

    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    namedWindow("Video", WINDOW_NORMAL);

    bool WatermarkControl = false;
    bool YUVControl = false;
    bool HistogramControl = false;
    bool GrayControl = false;
    bool EqualizeHistControl = false;
    bool GaussianControl = false;
    bool BlurControl = false;
    bool ThresholdControl = false;
    while (true)
    {
        Mat frame; // RGB frame
        // Read a frame from the video
        cap >> frame;

        // Check for end of video
        if (frame.empty())
        {
            break;
        }

        if (WatermarkControl)
        {
            frame = applyWatermark(frame);
        }
        if (YUVControl)
        {
            frame = toYuv(frame);
        }
        if (HistogramControl)
        {
            showHistogram(frame);
        }
        if (GrayControl)
        {
            frame = applyGrayScale(frame);
        }
        if (EqualizeHistControl)
        {
            frame = applyEqualization(frame);
        }
        if (GaussianControl)
        {
            frame = applyGaussianBlur(frame);
        }
        if (BlurControl)
        {
            frame = applyBlur(frame);
        }
        if (ThresholdControl)
        {
            frame = applyThreshold(frame);
        }

        imshow("Video", frame);

        char key = waitKey(30);
        if (key == 27)
        { // Exit when the Esc key is pressed
            break;
        }
        if (key == 'y')
        {
            YUVControl = !YUVControl;
        }
        if (key == 'w')
        {
            WatermarkControl = !WatermarkControl;
        }
        if (key == 'h')
        {
            HistogramControl = !HistogramControl;
        }
        if (key == 'g')
        {
            GrayControl = !GrayControl;
        }
        if (key == 'e')
        {
            EqualizeHistControl = !EqualizeHistControl;
        }
        if (key == 'b')
        {
            GaussianControl = !GaussianControl;
        }
        if (key == 'f')
        {
            BlurControl = !BlurControl;
        }
        if (key == 't')
        {
            ThresholdControl = !ThresholdControl;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
