#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <filesystem>
#include <cstring>
#include <cstdint>
#include <climits>

using namespace std;
namespace fs = filesystem;

// 霍夫曼树节点结构（数组存储方式）
struct HTNode {
    uint64_t weight;  // 权重（频率）
    int parent;       // 父节点索引
    int lchild;       // 左孩子索引
    int rchild;       // 右孩子索引
    unsigned char data; // 存储的字节（仅叶子节点有效）
};

// 霍夫曼编码类型（字符指针数组）
using HuffmanCode = char**;

// 从数组中选择两个权重最小且parent为0的节点
void Select(HTNode* HT, int n, int& s1, int& s2) {
    uint64_t min1 = ULLONG_MAX, min2 = ULLONG_MAX;
    s1 = s2 = 0;
    
    for (int i = 1; i <= n; ++i) {
        if (HT[i].parent == 0) {  // 只考虑未被选中的节点
            if (HT[i].weight < min1) {
                min2 = min1;
                s2 = s1;
                min1 = HT[i].weight;
                s1 = i;
            } else if (HT[i].weight < min2) {
                min2 = HT[i].weight;
                s2 = i;
            }
        }
    }
}

// 构建霍夫曼树并生成编码（基于图片算法思想）
void HuffmanCoding(HTNode*& HT, HuffmanCode& HC, 
                  const unordered_map<unsigned char, uint64_t>& freqMap, int n) {
    if (n <= 0) return;
    
    int m = 2 * n - 1;  // 总节点数 = 2n-1
    HT = new HTNode[m + 1];  // 下标从1开始
    
    // 1. 初始化叶子节点（前n个节点）
    int i = 1;
    for (const auto& pair : freqMap) {
        HT[i].weight = pair.second;
        HT[i].data = pair.first;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
        ++i;
    }
    
    // 初始化非叶子节点
    for (i = n + 1; i <= m; ++i) {
        HT[i].weight = 0;
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    
    // 2. 构建霍夫曼树
    for (i = n + 1; i <= m; ++i) {
        int s1, s2;
        Select(HT, i - 1, s1, s2);  // 选择两个最小权重节点
        
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }
    
    // 3. 从叶子到根逆向求每个字符的霍夫曼编码
    HC = new char*[n + 1];  // 存储n个字符的编码
    char* cd = new char[n];  // 临时存放编码
    cd[n - 1] = '\0';       // 编码结束符
    
    // 建立字节到索引的映射（方便后续查找）
    unordered_map<unsigned char, int> dataToIndex;
    for (i = 1; i <= n; ++i) {
        dataToIndex[HT[i].data] = i;
    }
    
    for (i = 1; i <= n; ++i) {
        int start = n - 1;  // 编码起始位置
        int c = i;
        int f = HT[i].parent;
        
        // 从叶子节点向上遍历到根
        while (f != 0) {
            --start;
            if (HT[f].lchild == c) {
                cd[start] = '0';  // 左孩子为0
            } else {
                cd[start] = '1';  // 右孩子为1
            }
            c = f;
            f = HT[f].parent;
        }
        
        // 保存编码
        HC[i] = new char[n - start];
        strcpy(HC[i], &cd[start]);
    }
    
    delete[] cd;
}

// 读取文件并统计字节频率
bool readFileAndCountFreq(const string& inputPath, vector<unsigned char>& fileData, 
                         unordered_map<unsigned char, uint64_t>& freqMap) {
    if (!fs::exists(inputPath)) {
        cerr << "错误：文件不存在: " << inputPath << endl;
        return false;
    }
    
    ifstream inFile(inputPath, ios::binary | ios::ate);
    if (!inFile.is_open()) {
        cerr << "错误：无法打开文件 " << inputPath << endl;
        return false;
    }

    streampos fileSize = inFile.tellg();
    fileData.resize(static_cast<size_t>(fileSize));
    inFile.seekg(0, ios::beg);
    inFile.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    inFile.close();

    freqMap.clear();
    for (unsigned char byte : fileData) {
        freqMap[byte]++;
    }

    return true;
}

// 压缩文件（使用优化后的霍夫曼编码算法）
bool compressFile(const string& inputPath, const string& outputPath) {
    vector<unsigned char> fileData;
    unordered_map<unsigned char, uint64_t> freqMap;

    if (!readFileAndCountFreq(inputPath, fileData, freqMap)) {
        return false;
    }

    int n = freqMap.size();
    if (n == 0) {
        cerr << "错误：文件为空，无法压缩" << endl;
        return false;
    }

    // 构建霍夫曼树和编码表
    HTNode* HT = nullptr;
    HuffmanCode HC = nullptr;
    HuffmanCoding(HT, HC, freqMap, n);

    // 创建字节到编码的映射
    unordered_map<unsigned char, string> codeMap;
    unordered_map<unsigned char, int> dataToIndex;
    for (int i = 1; i <= n; ++i) {
        dataToIndex[HT[i].data] = i;
        codeMap[HT[i].data] = HC[i];
    }

    // 写入压缩文件
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputPath << endl;
        return false;
    }

    // 写入频率表大小和频率表
    uint8_t freqMapSize = static_cast<uint8_t>(n);
    outFile.write(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

    for (int i = 1; i <= n; ++i) {
        outFile.write(reinterpret_cast<const char*>(&HT[i].data), sizeof(HT[i].data));
        outFile.write(reinterpret_cast<const char*>(&HT[i].weight), sizeof(HT[i].weight));
    }

    // 生成压缩比特流
    string bitStream;
    for (unsigned char byte : fileData) {
        bitStream += codeMap[byte];
    }

    // 处理补零
    uint8_t paddingBits = (8 - (bitStream.size() % 8)) % 8;
    for (int i = 0; i < paddingBits; ++i) {
        bitStream += "0";
    }

    outFile.write(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    // 写入压缩数据
    for (size_t i = 0; i < bitStream.size(); i += 8) {
        string byteStr = bitStream.substr(i, 8);
        unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 2));
        outFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    outFile.close();

    // 释放内存
    for (int i = 1; i <= n; ++i) {
        delete[] HC[i];
    }
    delete[] HC;
    delete[] HT;

    // 显示压缩信息
    uint64_t originalSize = fs::file_size(inputPath);
    uint64_t compressedSize = fs::file_size(outputPath);
    double compressionRatio = static_cast<double>(compressedSize) / originalSize;

    cout << "压缩成功！" << endl;
    cout << "原始大小：" << originalSize << " 字节" << endl;
    cout << "压缩后大小：" << compressedSize << " 字节" << endl;
    cout << "压缩比：" << compressionRatio << endl;

    return true;
}

// 解压文件
bool decompressFile(const string& inputPath, const string& outputPath) {
    if (!fs::exists(inputPath)) {
        cerr << "错误：压缩文件不存在: " << inputPath << endl;
        return false;
    }
    
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) {
        cerr << "错误：无法打开压缩文件 " << inputPath << endl;
        return false;
    }

    // 读取频率表
    unordered_map<unsigned char, uint64_t> freqMap;
    uint8_t freqMapSize;
    inFile.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

    for (int i = 0; i < freqMapSize; ++i) {
        unsigned char byte;
        uint64_t freq;
        inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        freqMap[byte] = freq;
    }

    // 重建霍夫曼树
    int n = freqMap.size();
    HTNode* HT = nullptr;
    HuffmanCode HC = nullptr;
    HuffmanCoding(HT, HC, freqMap, n);

    // 读取补零位数
    uint8_t paddingBits;
    inFile.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    // 读取压缩数据并转换为比特流
    string bitStream;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        bitStream += bitset<8>(byte).to_string();
    }

    if (paddingBits > 0) {
        bitStream.resize(bitStream.size() - paddingBits);
    }

    inFile.close();

    // 解码比特流
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputPath << endl;
        return false;
    }

    int m = 2 * n - 1;
    int current = m;  // 从根节点开始
    uint64_t totalBytes = 0;
    uint64_t expectedTotal = 0;
    for (const auto& pair : freqMap) {
        expectedTotal += pair.second;
    }

    // 处理只有一个字符的情况
    if (n == 1) {
        for (uint64_t i = 0; i < expectedTotal; ++i) {
            outFile.write(reinterpret_cast<char*>(&HT[1].data), sizeof(HT[1].data));
        }
    } else {
        for (char bit : bitStream) {
            if (bit == '0') {
                current = HT[current].lchild;  // 左孩子
            } else {
                current = HT[current].rchild;  // 右孩子
            }

            // 到达叶子节点
            if (HT[current].lchild == 0 && HT[current].rchild == 0) {
                outFile.write(reinterpret_cast<char*>(&HT[current].data), sizeof(HT[current].data));
                current = m;  // 回到根节点
                totalBytes++;

                if (totalBytes >= expectedTotal) {
                    break;
                }
            }
        }
    }

    outFile.close();

    // 释放内存
    for (int i = 1; i <= n; ++i) {
        delete[] HC[i];
    }
    delete[] HC;
    delete[] HT;

    if (totalBytes != expectedTotal) {
        cerr << "警告：解码字节数与预期不符" << endl;
        return false;
    }

    cout << "解压成功！" << endl;
    cout << "输出文件：" << outputPath << endl;
    cout << "解压后大小：" << fs::file_size(outputPath) << " 字节" << endl;

    return true;
}

// 显示帮助信息
void showHelp() {
    cout << "\n霍夫曼文件压缩工具" << endl;
    cout << "用法：" << endl;
    cout << "  压缩：-c <输入文件路径> <输出压缩文件路径>" << endl;
    cout << "  解压：-d <输入压缩文件路径> <输出文件路径>" << endl;
    cout << "  退出：quit 或 exit" << endl;
}

// 处理命令
bool processCommand(const string& mode, const string& inputPath, const string& outputPath) {
    if (mode == "-c" || mode == "c") {
        return compressFile(inputPath, outputPath);
    } else if (mode == "-d" || mode == "d") {
        return decompressFile(inputPath, outputPath);
    } else {
        cerr << "错误：无效的模式参数 " << mode << endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        string mode = argv[1];
        string inputPath = argv[2];
        string outputPath = argv[3];
        return processCommand(mode, inputPath, outputPath) ? 0 : 1;
    }
    
    showHelp();
    
    string line;
    while (true) {
        cout << "\n> ";
        getline(cin, line);
        
        // 处理退出和帮助命令
        if (line == "quit" || line == "exit") {
            cout << "再见！" << endl;
            break;
        }
        if (line == "help") {
            showHelp();
            continue;
        }
        
        // 解析命令
        vector<string> tokens;
        string token;
        for (char c : line) {
            if (c == ' ' || c == '\t') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(token);
        }
        
        if (tokens.size() == 3) {
            processCommand(tokens[0], tokens[1], tokens[2]);
        } else {
            cerr << "命令格式错误，正确格式：<模式> <输入文件> <输出文件>" << endl;
        }
    }
    
    return 0;
}