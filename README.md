# CSLP

## Setup

This project requirements are:

- Opencv 3.4.1
- Cmake 3.22.1

To install opencv, you can follow the instructions in the following link: https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html

To install cmake, you can follow the instructions in the following link: https://cmake.org/install/

## Build

To build the project, you can run the following commands in the root directory of the project:

```bash
cmake .
```

```bash
make
```

This will generate the build folder alongside with individual folders for each deliverable. Inside each folder, you can find the executable(s) for the respective deliverable.

## Run

The way to run each deliverable varies. The instructions for each deliverable are as follows:

### Deliverable 1

#### Image

To run 1.1, you can run the following command in the root directory of the project:

```bash
cd build/Deliverable_1
```

```bash
./ImageExample.out
```

This will generate a copy of the image that is hardcoded in the code. (If not changed, it will be the image "bike3.ppm")
Note: The ImageExample has commented code that showcases other functions like colorspace conversion(YUV, Gray), histogram equalization, etc. You can uncomment them to see the results.

#### Video

To run 1.2, you can run the following command in the root directory of the project:

```bash
cd build/Deliverable_1
```

```bash
./Video_Ex2.out
```

This will open 4 windows, RGB, YUV 4:2:0, YUV 4:2:2 and YUV 4:4:4. The video that is hardcoded in the code will be shown in each window with the respective colorspace conversion.

---

To run 1.3, you can run the following command in the root directory of the project:

```bash
cd build/Deliverable_1
```

```bash
./Video_Ex3.out
```

This will open a video player with some functionalities. The video that is hardcoded in the code will be shown in the video player.

The video player has the following functionalities:

- Pressing w will add a watermark to the video (hardcoded);
- Pressing y will convert the video to YUV 4:4:4;
- Pressing h will show the histogram of the video;
- Pressing g will show the grayscale version of the video;
- Pressing e will apply histogram equalization to the video;
- Pressing b will apply a Gaussian blur filter to the video;
- Pressing f will apply a blur filter to the video;
- Pressing t will apply a threshold to the video;
- Pressing esc will close the video player;

The functionalities are processed in a determined order, which is the order that they are listed above.
For example, it's impossible to apply a gaussian blur filter before applying a grayscale filter.

### Deliverable 2

To run the deliverable 2 example, you can run the following command in the root directory of the project:

Requirements:
This example requires the existance of a Test.txt file inside the Deliverable_2 folder. This file should contain text. (You may change the example code to read from a different file)

```bash
cd build/Deliverable_2
```

```bash
./BitStreamExample.out
```

This will read the Test.txt file and write the contents in binary to a new file called out.bin (Making use of the readBit() and writeBit() functions). It will after read the out.bin file and write the contents to a new file called out2.bin (Making use of the readBits() and writeBits()).

### Deliverable 3

To run the deliverable 3 example, you can run the following command in the root directory of the project:

Requirements:
This requires the existance of an empty encoded.bin file inside the Deliverable_3 folder.

```bash
cd build/Deliverable_3
```

```bash
./GolombExample.out
```

This will encode the numbers in the following range -50, 50 (inclusive) and write them to the encoded.bin file. After that, it will read the encoded.bin file and decode the numbers, writing them to the terminal.

### Deliverable 4 and 5 Results

The results can be found in the results.txt for the intra-encoder and the results-hybrid for the hybrid-encoder.

If you wish to reproduce these files run IntraEncodeBatch.out HybridEncoderBatch.out.

### Deliverable 6

The results can be found in the report, but a shell script "ResultBatch.sh" is provided to replicate the inputs of the tests automatically.
The requirements are the compiled versions of the updated code (CMake/make should only compile this as the previous deliverables are commented out to avoid confusion.) and the requirements of the shell script that it warns about on startup.