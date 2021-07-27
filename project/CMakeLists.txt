cmake_minimum_required(VERSION 3.10)
project(mvobjreader)

include_directories(./include)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -s -O2")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

add_executable(${CMAKE_PROJECT_NAME}
    ./source/main.cpp    
    ./source/CObjReader.cpp
    ./source/CObjReaderWriteJS.cpp
    ./lib/CFileIO.cpp
    ./lib/NStringTool.cpp
)
