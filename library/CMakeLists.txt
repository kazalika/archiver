cmake_minimum_required(VERSION 3.16)

set(SRC_FILES BitReader.cpp BitWriter.cpp Compress.cpp Decompress.cpp Node.cpp Symbol.cpp Trie.cpp)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)