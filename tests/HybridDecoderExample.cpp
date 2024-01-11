#include "HybridDecoder.hpp"

int main(int argc, char const *argv[])
{
    string input;
    string output;
    for(int i = 1; i < argc; i++){
        string arg(argv[i]);
        if (arg == "-o")
        {
            output = argv[i + 1];
        }
        else if (arg == "-i")
        {
            input = argv[i + 1];
        }
    }

    // assert that all arguments are present
    if (output.empty() || input.empty())
    {
        cout << "Usage: " << argv[0] << " -i <input file> -o <output file>" << endl;
        return 1;
    }

    HybridDecoder decoder(input);

    auto start = chrono::high_resolution_clock::now();

    decoder.decode(output);

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << "ms" << endl;

    return 0;
}
