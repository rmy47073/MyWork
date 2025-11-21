# Huffman文件压缩工具

基于Huffman编码算法的文件压缩/解压工具，支持多种文档格式。

## 功能特性

- ? 支持多种文件格式：
  - 图像：JPG, PNG, BMP, GIF, TIFF
  - Word文档：DOC, DOCX
  - PDF文档
  - PPT文档：PPT, PPTX
  - 视频：MP4, AVI, MOV, MKV, WMV
  - 音频：MP3, WAV, FLAC, AAC, OGG
  - 其他任意二进制文件
- ? 使用Huffman编码算法进行压缩
- ? 显示压缩比和节省空间
- ? 命令行界面，简单易用

## 编译方法

### 使用CMake（推荐）

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 使用Makefile（Linux/Mac）

```bash
make
```

### 手动编译（Windows）

```bash
g++ -std=c++17 -O2 -o huffman_compressor.exe main.cpp FileCompressor.cpp HuffmanTree.cpp
```

## 使用方法

### 压缩文件

```bash
huffman_compressor -c <输入文件> <输出文件>
```

示例：
```bash
huffman_compressor -c image.jpg image.huff
huffman_compressor -c document.pdf document.huff
huffman_compressor -c video.mp4 video.huff
```

### 解压文件

```bash
huffman_compressor -d <压缩文件> <输出文件>
```

示例：
```bash
huffman_compressor -d image.huff image_decompressed.jpg
huffman_compressor -d document.huff document_decompressed.pdf
```

## 输出示例

```
[压缩] 图像文件: image.jpg
原始文件大小: 2.50 MB
正在压缩...

压缩完成！
压缩后文件大小: 1.85 MB
压缩比: 74.00%
节省空间: 650.00 KB (26.00%)
```

## 注意事项

1. Huffman编码对已经压缩过的文件（如JPG、MP3、MP4等）效果有限，因为这些文件本身已经是压缩格式
2. 对于文本文件和未压缩的二进制文件，压缩效果会更好
3. 压缩文件会包含编码表，对于小文件可能反而增大
4. 确保有足够的磁盘空间存储压缩文件

## 文件结构

```
huffman_compressor/
├── main.cpp              # 主程序入口
├── FileCompressor.h      # 文件压缩类头文件
├── FileCompressor.cpp    # 文件压缩类实现
├── HuffmanTree.h         # Huffman树头文件
├── HuffmanTree.cpp       # Huffman树实现
├── CMakeLists.txt        # CMake构建文件
├── Makefile              # Make构建文件
└── README.md             # 说明文档
```

