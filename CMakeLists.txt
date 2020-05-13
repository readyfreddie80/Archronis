cmake_minimum_required(VERSION 3.13)
project(Archronis)

set(CMAKE_CXX_STANDARD 14)

add_executable(Archronis main.cpp HuffmanArchiver.cpp HuffmanArchiver.h
        Tree/Tree.cpp Tree/Tree.h Queue/Queue.cpp Queue/Queue.h)
