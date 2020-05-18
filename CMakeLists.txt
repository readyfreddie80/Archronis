cmake_minimum_required(VERSION 3.13)
project(Archronis)

set(CMAKE_CXX_STANDARD 14)

add_executable(Archronis main.cpp
        Tree/Tree.h Queue/Queue.h Vector/Vector.h Archive/Debug.h ProgramInfo/ProgramInfo.cpp ProgramInfo/ProgramInfo.h Archive/Huffman.cpp Archive/Huffman.h Archive/Archiver.cpp Archive/Archiver.h Archive/File.cpp Archive/File.h)
