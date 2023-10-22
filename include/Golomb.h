/**
 * @file Golomb.h
 * @brief Header file for the Golomb class
 */

#include <fstream>

class BitStream;

/**
 * @brief A class for encoding and decoding non-negative integers using Golomb coding
 *
 * The Golomb class provides methods for encoding and decoding non-negative integers using Golomb coding with a specified divisor M. The class uses a BitStream object to read from and write to an input/output file.
 */
class Golomb
{
private:
    int M; /**< The divisor used for Golomb encoding/decoding */
    BitStream *bs; /**< A pointer to the BitStream object used for input/output */

public:
    /**
     * @brief Constructs a new Golomb encoder/decoder with the specified divisor and input/output file names
     *
     * The constructor initializes a new BitStream object with the specified input and output file names.
     *
     * @param M The divisor used for Golomb encoding/decoding
     * @param InputFileName The name of the input file
     * @param OutputFileName The name of the output file
     */
    Golomb(int M, const char *InputFileName, const char *OutputFileName);

    /**
     * @brief Destroys the Golomb encoder/decoder and closes the input/output file
     *
     * The destructor flushes the BitStream buffer and deletes the BitStream object.
     */
    ~Golomb();

    /**
     * @brief Encodes a non-negative integer using Golomb encoding
     *
     * The function maps the input number to a new number using the map function, and then encodes the new number using Golomb encoding.
     *
     * @param num The non-negative integer to be encoded
     */
    void Encode(int num);

    /**
     * @brief Decodes a non-negative integer using Golomb decoding
     *
     * The function decodes a non-negative integer using Golomb decoding and the current divisor M.
     *
     * @return int The decoded non-negative integer
     */
    int Decode();

    /**
     * @brief Closes the input/output file and frees the BitStream object
     *
     * The function flushes the BitStream buffer and deletes the BitStream object.
     */
    void Close();

    /**
     * @brief Maps a non-negative integer to a new non-negative integer
     *
     * The function maps a non-negative integer to a new non-negative integer using the formula (2 * num + 1) / (2 * M).
     *
     * @param num The non-negative integer to be mapped
     * @return int The mapped non-negative integer
     */
    int map(int num);

    /**
     * @brief Unmaps a non-negative integer to its original value
     *
     * The function unmaps a non-negative integer to its original value using the formula q * M + r.
     *
     * @param num The non-negative integer to be unmapped
     * @return int The unmapped non-negative integer
     */
    int reverse_map(int num);
};