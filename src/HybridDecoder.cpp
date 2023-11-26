#include "HybridDecoder.hpp"

HybridDecoder::HybridDecoder(string input_file)
{
    this->input_file = input_file;
    predictors = getPredictors();
}

void HybridDecoder::decode(string output_file)
{
    Converter conv;
    GolombDecoder dec(input_file);

    int format = dec.decode();
    int predictor = dec.decode();
    int shift = dec.decode();
    int block_range = dec.decode();
    int period = dec.decode();
    int n_frames = dec.decode();
    int block_size = dec.decode();
    int width = dec.decode();
    int height = dec.decode();

    cout << "Format: " << format << endl;
    cout << "Predictor: " << predictor << endl;
    cout << "Shift: " << shift << endl;
    cout << "Block Range: " << block_range << endl;
    cout << "Period: " << period << endl;
    cout << "Number of Frames: " << n_frames << endl;
    cout << "Block Size: " << block_size << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;

    IntraDecoder intra_dec(dec, shift);
    InterDecoder inter_dec(dec, block_size, block_range, shift);

    Mat new_frame;
    Mat old_frame;

    int old_cost = 0, new_cost = 1;
    int frame_count = 0;

    switch (format)
    {
    case 0:
    {
        while (frame_count < n_frames)
        {
            new_frame = Mat::zeros(height, width, CV_8UC3);

            if (new_cost > old_cost || frame_count % period == 0)
            {
                new_cost = intra_dec.decode(new_frame, predictors[predictor]);
                old_cost = new_cost;
                new_frame.copyTo(old_frame);
            }
            else
            {
                new_cost = inter_dec.decode(old_frame, new_frame);
            }

            imshow("Image", conv.yuv444_to_rgb(new_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << frame_count++ << " decoded." << endl;
        }
    }
    case 1:
    {
        while (frame_count < n_frames)
        {
            new_frame = Mat::zeros(height, width, CV_8UC1);

            if (new_cost > old_cost || frame_count % period == 0)
            {
                new_cost = intra_dec.decode(new_frame, predictors[predictor]);
                old_cost = new_cost;
                new_frame.copyTo(old_frame);
            }
            else
            {
                new_cost = inter_dec.decode(old_frame, new_frame);
            }

            imshow("Image", conv.yuv422_to_rgb(new_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << frame_count++ << " decoded." << endl;
        }
    }
    case 2:
    {
        while (frame_count < n_frames)
        {
            new_frame = Mat::zeros(height, width, CV_8UC1);

            if (new_cost > old_cost || frame_count % period == 0)
            {
                new_cost = intra_dec.decode(new_frame, predictors[predictor]);
                old_cost = new_cost;
                new_frame.copyTo(old_frame);
            }
            else
            {
                new_cost = inter_dec.decode(old_frame, new_frame);
            }

            imshow("Image", conv.yuv420_to_rgb(new_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << frame_count++ << " decoded." << endl;
        }
    }
    }
}