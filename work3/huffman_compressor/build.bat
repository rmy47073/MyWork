@echo off
echo 正在编译Huffman压缩工具...
g++ -std=c++17 -O2 -o huffman_compressor.exe main.cpp FileCompressor.cpp HuffmanTree.cpp
if %errorlevel% == 0 (
    echo 编译成功！
    echo 可执行文件: huffman_compressor.exe
) else (
    echo 编译失败！
    pause
)

