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
    ofstream file(output_file, ios::binary);
    int predictor = dec.decode();
    int block_range = dec.decode();
    int shift = dec.decode();
    int period = dec.decode();
    int n_frames = dec.decode();
    // variable char array to store file header
    char temp_arr[100];
    int i = 0;
    while (true)
    {
        int a = dec.decode();
        if (a == 10)
        {
            temp_arr[i] = '\0';
            break;
        }
        temp_arr[i] = static_cast<char>(a);
        i++;
    }
    // create new array to store file header
    char file_header[i];
    for (int j = 0; j < i; j++)
    {
        file_header[j] = temp_arr[j];
    }


    // turn file header into vector of strings
    string file_header_str(file_header, i);
    file << file_header_str << "\n";
    istringstream iss(file_header_str);
    vector<string> tokens{istream_iterator<string>{iss},
                          istream_iterator<string>{}};
    
    // get height and width
    int width = stoi(tokens[1].substr(1));
    int height = stoi(tokens[2].substr(1));


    
    int block_size = dec.decode();
    IntraDecoder intra_dec(dec, shift);
    InterDecoder inter_dec(dec, block_size, block_range, shift);

    Mat curr_frame;
    Mat old_frame;

    int old_frame_cost = 0;
    int curr_frame_cost = 1;
    int count = 0;
    while (count < n_frames)
    {
        file << "FRAME\n";
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
        // go through the y, then u, v channel and write it to file
        
        for (int channels = 0; channels < 3; channels++)
        {
            for (int row = 0; row < height; row++)
            {
                for (int col = 0; col < width; col++)
                {
                    // write the bytes of the pixel to the file
                    file.write((char * ) &curr_frame.at<Vec3b>(row, col)[channels], sizeof(uchar));
                }
            }
        }
        count++;
    }
}