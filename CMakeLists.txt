cmake_minimum_required ( VERSION 3.22.1 )
project( main VERSION 0.0.1 )

include_directories(include)

#Find opencv package
find_package(OpenCV REQUIRED) # manual specification of the OpenCVConfig.cmake path is necessary since otherwise, cmake assigns /opt/local a higher priority and ocv2.4 will be used
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Deliverable 1
## 1.
add_executable( ImageExample.out src/ImageExample.cpp src/image.cpp)
set_target_properties( ImageExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_1")

## 2.
add_executable( VideoExample.out src/VideoExample.cpp )
target_link_libraries( VideoExample.out ${OpenCV_LIBS} )
set_target_properties( VideoExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_1")

# Deliverable 2
## 1.
add_executable( BitStreamExample.out src/BitStreamExample.cpp src/BitStream.cpp  )
set_target_properties( BitStreamExample.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Deliverable_2")
