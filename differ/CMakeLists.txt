cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project( comapreImage )
find_package( OpenCV REQUIRED )
add_executable( comapreImage comapreImage.cpp )
target_link_libraries( comapreImage ${OpenCV_LIBS} )
