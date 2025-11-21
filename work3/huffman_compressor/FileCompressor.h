#ifndef FILE_COMPRESSOR_H
#define FILE_COMPRESSOR_H

#include "HuffmanTree.h"
#include <string>
#include <map>
#include <fstream>

using namespace std;

class FileCompressor {
public:
    FileCompressor();
    ~FileCompressor();

    // 压缩文件
    bool compressFile(const string& inputPath, const string& outputPath, double& compressionRatio);
    
    // 解压文件
    bool decompressFile(const string& inputPath, const string& outputPath);

private:
    // 统计字符频率
    map<unsigned char, unsigned long long> countFrequencies(const string& filePath);
    
    // 将字符串转换为位流
    vector<bool> stringToBits(const string& code);
    
    // 将位流转换为字节
    vector<unsigned char> bitsToBytes(const vector<bool>& bits);
    
    // 从字节恢复位流
    vector<bool> bytesToBits(const vector<unsigned char>& bytes, size_t bitCount);
    
    // 写入压缩文件头（包含编码表和文件信息）
    void writeHeader(ofstream& out, 
                    const map<unsigned char, string>& codes,
                    size_t originalSize);
    
    // 读取压缩文件头
    bool readHeader(ifstream& in,
                   map<unsigned char, string>& codes,
                   size_t& originalSize,
                   size_t& compressedBitCount);
};

#endif // FILE_COMPRESSOR_H

