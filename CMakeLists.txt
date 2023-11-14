cmake_minimum_required ( VERSION 3.22.1 )
project( main VERSION 0.0.1 )

include_directories(include)
#Find opencv package
find_package(OpenCV REQUIRED) # manual specification of the OpenCVConfig.cmake path is necessary since otherwise, cmake assigns /opt/local a higher priority and ocv2.4 will be used
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Deliverable 1
## 1.
add_executable( ImageExample.out tests/ImageExample.cpp src/Image.cpp)
target_link_libraries( ImageExample.out ${OpenCV_LIBS} )
set_target_properties( ImageExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_1")

## 2.
add_executable( Video_Ex2.out tests/Video_Ex2.cpp )
target_link_libraries( Video_Ex2.out ${OpenCV_LIBS} )
set_target_properties( Video_Ex2.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_1")

add_executable( Video_Ex3.out tests/Video_Ex3.cpp )
target_link_libraries( Video_Ex3.out ${OpenCV_LIBS} )
set_target_properties( Video_Ex3.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_1")

# Deliverable 2
## 1.
add_executable( BitStreamExample.out tests/BitStreamExample.cpp src/BitStream.cpp  )
set_target_properties( BitStreamExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_2")

# Deliverable 3
## 1.
add_executable( GolombExample.out tests/GolombExample.cpp src/Golomb.cpp src/BitStream.cpp  )
set_target_properties( GolombExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_3")

# Deliverable 4
## 2.
add_executable( EncoderExample.out tests/EncoderExample.cpp src/IntraEncoder.cpp src/Golomb.cpp src/BitStream.cpp )
target_link_libraries( EncoderExample.out ${OpenCV_LIBS} )
set_target_properties( EncoderExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_4")
