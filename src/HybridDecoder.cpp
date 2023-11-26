#include "HybridDecoder.hpp"

HybridDecoder::HybridDecoder(string input_file)
{
    this->input_file = input_file;
    predictors = getPredictors();
}

void HybridDecoder::decode()
{
    Converter conv;
    GolombDecoder dec(input_file);

    int format = dec.decode();
    int predictor = dec.decode();
    int block_range = dec.decode();
    int shift = dec.decode();
    int period = dec.decode();
    int n_frames = dec.decode();
    int block_size = dec.decode();
    int width = dec.decode();
    int height = dec.decode();

    cout << endl;
    cout << "Parameter m: " << dec.get_m() << endl;
    cout << "format: " << format << endl;
    cout << "Predictor: " << predictor << endl;
    cout << "Block Range: " << block_range << endl;
    cout << "Shift: " << shift << endl;
    cout << "Period: " << period << endl;
    cout << "Number of Frames: " << n_frames << endl;
    cout << "Block Size: " << block_size << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;

    cout << endl;

    IntraDecoder intra_dec(dec, shift);
    InterDecoder inter_dec(dec, block_size, block_range, shift);

    Mat curr_frame;
    Mat old_frame;

    int old_frame_cost = 0;
    int curr_frame_cost = 1;
    int count = 0;
    switch (format)
    {
    case 0:
    {
        while (count < n_frames)
        {
            curr_frame = Mat::zeros(height, width, CV_8UC3);

            if (curr_frame_cost > old_frame_cost || count % period == 0)
            {
                curr_frame_cost = intra_dec.decode(curr_frame, predictors[predictor]);
                old_frame_cost = curr_frame_cost;
                curr_frame.copyTo(old_frame);
            }
            else
            {
                curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
            }

            imshow("Image", conv.yuv444_to_rgb(curr_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << count++ << " decoded." << endl;
        }
    }
    case 1:
    {
        while (count < n_frames)
        {
            curr_frame = Mat::zeros(height, width, CV_8UC1);

            if (curr_frame_cost > old_frame_cost || count % period == 0)
            {
                curr_frame_cost = intra_dec.decode(curr_frame, predictors[predictor]);
                old_frame_cost = curr_frame_cost;
                curr_frame.copyTo(old_frame);
            }
            else
            {
                curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
            }

            imshow("Image", conv.yuv422_to_rgb(curr_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << count++ << " decoded." << endl;
        }
    }
    case 2:
    {
        cout << count << endl;
        cout << n_frames << endl;
        while (count < n_frames)
        {
            curr_frame = Mat::zeros(height, width, CV_8UC1);
            cout << "here" << endl;

            if (curr_frame_cost > old_frame_cost || count % period == 0)
            {
                curr_frame_cost = intra_dec.decode(curr_frame, predictors[predictor]);
                old_frame_cost = curr_frame_cost;
                curr_frame.copyTo(old_frame);
            }
            else
            {
                curr_frame_cost = inter_dec.decode(old_frame, curr_frame);
            }

            imshow("Image", conv.yuv420_to_rgb(curr_frame));
            if (waitKey(10) == 27)
            {
                destroyAllWindows();
            }; // Wait for a keystroke in the window
            cout << "Frame " << count++ << " decoded." << endl;
        }
    }
    }
}