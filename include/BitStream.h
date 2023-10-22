/**
 * @file BitStream.h
 * @brief Header file for the BitStream class
 *
 * This header file defines the BitStream class, which provides a way to read and write individual bits from a binary file.
 */

#include <fstream>
#include <vector>

/**
 * @brief A struct representing the return value of the `ReadBits` method
 *
 * The `WriteBitReturn` struct contains an array of integers representing the bits that were read, and the number of bits that were read.
 */
struct WriteBitReturn
{
    int *bits; ///< An array of integers representing the bits that were read
    int size;  ///< The number of bits that were read
};

/**
 * @brief A class representing a binary stream of bits
 *
 * The `BitStream` class provides a way to read and write individual bits from a binary file.
 */
class BitStream
{
private:
    std::ifstream in;    ///< The input file stream
    std::ofstream out;   ///< The output file stream
    short read_pos;      ///< The current read position in the read buffer
    short write_pos;     ///< The current write position in the write buffer
    u_char read_buffer;  ///< The read buffer
    u_char write_buffer; ///< The write buffer

public:
    /**
     * @brief Construct a new BitStream object from a file
     *
     * The constructor reads a binary file from disk and creates a new `BitStream` object from it.
     * The constructor also initializes the read and write buffers and positions.
     *
     * @param FileName The name of the binary file to read
     * @param OutputFileName The name of the binary file to write (default is "out.bin")
     *
     * @throw std::runtime_error if the file fails to open
     */
    BitStream(const char *FileName, const char *OutputFileName = "out.bin");

    /**
     * @brief Destroy the BitStream object and close the input and output files
     *
     * The destructor closes the input and output files used by the `BitStream` object.
     */
    ~BitStream();

    /**
     * @brief Read a single bit from the input file
     *
     * The `ReadBit` method reads a single bit from the input file and returns its value.
     * If the end of the file is reached, the method returns -1.
     *
     * @return The value of the next bit in the input file, or -1 if the end of the file is reached
     */
    int ReadBit();

    /**
     * @brief Read a specified number of bits from the input file
     *
     * The `ReadBits` method reads a specified number of bits from the input file and returns them as an array of integers.
     * If the end of the file is reached before all bits are read, the method returns the bits that were read.
     *
     * @param n_bits The number of bits to read
     *
     * @return A `WriteBitReturn` struct containing an array of integers representing the bits that were read, and the number of bits that were read
     */
    WriteBitReturn ReadBits(int n_bits);

    /**
     * @brief Write a single bit to the output file
     *
     * The `WriteBit` method writes a single bit to the output file.
     * If the input bit is not 0 or 1, the method prints an error message and ignores the input.
     *
     * @param bit The bit to write (must be 0 or 1)
     */
    void WriteBit(int bit);

    /**
     * @brief Write an array of bits to the output file
     *
     * The `WriteBits` method writes an array of bits to the output file.
     * If any of the input bits are not 0 or 1, the method prints an error message and ignores the input.
     *
     * @param bits An array of integers representing the bits to write (must be 0 or 1)
     * @param size The number of bits to write
     */
    void WriteBits(int *bits, int size);

    /**
     * @brief Flush any remaining bits to the output file
     *
     * The `Flush` method writes any remaining bits in the write buffer to the output file.
     * If the write buffer is already empty, the method does nothing.
     */
    void Flush();
};