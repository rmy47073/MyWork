cmake_minimum_required(VERSION 3.10)
project(HuffmanCompressor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
set(SOURCES
    main.cpp
    FileCompressor.cpp
    HuffmanTree.cpp
)

# 头文件
set(HEADERS
    FileCompressor.h
    HuffmanTree.h
)

# 创建可执行文件
add_executable(huffman_compressor ${SOURCES} ${HEADERS})

# Windows特定设置
if(WIN32)
    target_compile_definitions(huffman_compressor PRIVATE _WIN32)
endif()

