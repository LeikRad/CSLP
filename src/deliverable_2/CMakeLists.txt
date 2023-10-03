cmake_minimum_required ( VERSION 3.22.1 )
project( main VERSION 0.0.1 )

#Find opencv package
# find_package( OpenCV REQUIRED )  

# include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( main.out main.cpp BitStream.cpp  )
target_link_libraries( main.out )