#include "FileCompressor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

using namespace std;

FileCompressor::FileCompressor() {}

FileCompressor::~FileCompressor() {}

map<unsigned char, unsigned long long> FileCompressor::countFrequencies(const string& filePath) {
    map<unsigned char, unsigned long long> frequencies;
    ifstream file(filePath, ios::binary);
    
    if (!file.is_open()) {
        return frequencies;
    }

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1)) {
        frequencies[byte]++;
    }

    file.close();
    return frequencies;
}

vector<bool> FileCompressor::stringToBits(const string& code) {
    vector<bool> bits;
    for (char c : code) {
        bits.push_back(c == '1');
    }
    return bits;
}

vector<unsigned char> FileCompressor::bitsToBytes(const vector<bool>& bits) {
    vector<unsigned char> bytes;
    for (size_t i = 0; i < bits.size(); i += 8) {
        unsigned char byte = 0;
        for (size_t j = 0; j < 8 && (i + j) < bits.size(); j++) {
            if (bits[i + j]) {
                byte |= (1 << (7 - j));
            }
        }
        bytes.push_back(byte);
    }
    return bytes;
}

vector<bool> FileCompressor::bytesToBits(const vector<unsigned char>& bytes, size_t bitCount) {
    vector<bool> bits;
    for (size_t i = 0; i < bytes.size() && bits.size() < bitCount; i++) {
        for (int j = 7; j >= 0 && bits.size() < bitCount; j--) {
            bits.push_back((bytes[i] >> j) & 1);
        }
    }
    return bits;
}

void FileCompressor::writeHeader(ofstream& out,
                                 const map<unsigned char, string>& codes,
                                 size_t originalSize) {
    // 写入原始文件大小（8字节）
    out.write(reinterpret_cast<const char*>(&originalSize), sizeof(size_t));
    
    // 写入编码表大小（4字节）
    uint32_t codeTableSize = static_cast<uint32_t>(codes.size());
    out.write(reinterpret_cast<const char*>(&codeTableSize), sizeof(uint32_t));
    
    // 写入编码表
    for (const auto& pair : codes) {
        // 写入字符（1字节）
        out.write(reinterpret_cast<const char*>(&pair.first), 1);
        
        // 写入编码长度（1字节）
        uint8_t codeLength = static_cast<uint8_t>(pair.second.length());
        out.write(reinterpret_cast<const char*>(&codeLength), 1);
        
        // 写入编码字符串
        out.write(pair.second.c_str(), codeLength);
    }
}

bool FileCompressor::readHeader(ifstream& in,
                               map<unsigned char, string>& codes,
                               size_t& originalSize,
                               size_t& compressedBitCount) {
    // 读取原始文件大小
    if (!in.read(reinterpret_cast<char*>(&originalSize), sizeof(size_t))) {
        return false;
    }
    
    // 读取编码表大小
    uint32_t codeTableSize;
    if (!in.read(reinterpret_cast<char*>(&codeTableSize), sizeof(uint32_t))) {
        return false;
    }
    
    // 读取编码表
    codes.clear();
    for (uint32_t i = 0; i < codeTableSize; i++) {
        unsigned char byte;
        if (!in.read(reinterpret_cast<char*>(&byte), 1)) {
            return false;
        }
        
        uint8_t codeLength;
        if (!in.read(reinterpret_cast<char*>(&codeLength), 1)) {
            return false;
        }
        
        string code(codeLength, '\0');
        if (!in.read(&code[0], codeLength)) {
            return false;
        }
        
        codes[byte] = code;
    }
    
    // 计算压缩数据的位数量
    size_t headerSize = sizeof(size_t) + sizeof(uint32_t) + 
                       codeTableSize * (1 + 1) + 
                       [&codes]() {
                           size_t total = 0;
                           for (const auto& pair : codes) {
                               total += pair.second.length();
                           }
                           return total;
                       }();
    
    // 获取文件总大小
    size_t currentPos = in.tellg();
    in.seekg(0, ios::end);
    size_t fileSize = in.tellg();
    in.seekg(currentPos);
    
    compressedBitCount = (fileSize - headerSize) * 8;
    
    return true;
}

bool FileCompressor::compressFile(const string& inputPath, const string& outputPath, double& compressionRatio) {
    // 获取原始文件大小
    ifstream testFile(inputPath, ios::binary | ios::ate);
    if (!testFile.is_open()) {
        cerr << "错误：无法打开输入文件: " << inputPath << endl;
        return false;
    }
    size_t originalSize = testFile.tellg();
    testFile.close();

    if (originalSize == 0) {
        cerr << "错误：输入文件为空" << endl;
        return false;
    }

    // 统计字符频率
    map<unsigned char, unsigned long long> frequencies = countFrequencies(inputPath);
    if (frequencies.empty()) {
        cerr << "错误：无法读取文件或文件为空" << endl;
        return false;
    }

    // 构建Huffman树
    HuffmanTree tree;
    tree.buildTree(frequencies);

    // 生成编码表
    map<unsigned char, string> codes = tree.generateCodes();

    // 读取文件并编码
    ifstream inputFile(inputPath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "错误：无法打开输入文件进行读取" << endl;
        return false;
    }

    vector<bool> encodedBits;
    unsigned char byte;
    while (inputFile.read(reinterpret_cast<char*>(&byte), 1)) {
        string code = codes[byte];
        for (char bit : code) {
            encodedBits.push_back(bit == '1');
        }
    }
    inputFile.close();

    // 写入压缩文件
    ofstream outputFile(outputPath, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "错误：无法创建输出文件: " << outputPath << endl;
        return false;
    }

    // 写入文件头
    writeHeader(outputFile, codes, originalSize);

    // 写入压缩数据
    vector<unsigned char> compressedBytes = bitsToBytes(encodedBits);
    outputFile.write(reinterpret_cast<const char*>(compressedBytes.data()), compressedBytes.size());
    outputFile.close();

    // 计算压缩比
    size_t compressedSize = compressedBytes.size() + 
                            sizeof(size_t) + sizeof(uint32_t) + 
                            codes.size() * (1 + 1) + 
                            [&codes]() {
                                size_t total = 0;
                                for (const auto& pair : codes) {
                                    total += pair.second.length();
                                }
                                return total;
                            }();
    
    compressionRatio = static_cast<double>(compressedSize) / originalSize;

    return true;
}

bool FileCompressor::decompressFile(const string& inputPath, const string& outputPath) {
    ifstream inputFile(inputPath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "错误：无法打开压缩文件: " << inputPath << endl;
        return false;
    }

    // 读取文件头
    map<unsigned char, string> codes;
    size_t originalSize;
    size_t compressedBitCount;
    
    if (!readHeader(inputFile, codes, originalSize, compressedBitCount)) {
        cerr << "错误：无法读取文件头" << endl;
        inputFile.close();
        return false;
    }

    // 读取压缩数据
    vector<unsigned char> compressedBytes;
    unsigned char byte;
    while (inputFile.read(reinterpret_cast<char*>(&byte), 1)) {
        compressedBytes.push_back(byte);
    }
    inputFile.close();

    // 将字节转换为位
    size_t actualBitCount = compressedBytes.size() * 8;
    vector<bool> encodedBits = bytesToBits(compressedBytes, actualBitCount);

    // 重建Huffman树
    HuffmanTree tree;
    tree.rebuildTreeFromCodes(codes);

    // 解码
    ofstream outputFile(outputPath, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "错误：无法创建输出文件: " << outputPath << endl;
        return false;
    }

    auto root = tree.getRoot();
    auto current = root;
    size_t decodedBytes = 0;

    for (size_t i = 0; i < encodedBits.size() && decodedBytes < originalSize; i++) {
        if (encodedBits[i]) {
            current = current->right;
        } else {
            current = current->left;
        }

        if (current && current->isLeaf) {
            outputFile.write(reinterpret_cast<const char*>(&current->data), 1);
            decodedBytes++;
            current = root;
        }
    }

    outputFile.close();

    if (decodedBytes != originalSize) {
        cerr << "警告：解码的字节数与原始文件大小不匹配" << endl;
        return false;
    }

    return true;
}

