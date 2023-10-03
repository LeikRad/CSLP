cmake_minimum_required ( VERSION 3.22.1 )
project( main VERSION 0.0.1 )

#Find opencv package
find_package(OpenCV REQUIRED PATHS /usr/local/share/OpenCV NO_DEFAULT_PATH) # manual specification of the OpenCVConfig.cmake path is necessary since otherwise, cmake assigns /opt/local a higher priority and ocv2.4 will be used
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( video.out video.cpp  )
target_link_libraries( video.out ${OpenCV_LIBS} )