#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <filesystem>
#include <cstring>
#include <cstdint>
#include <climits>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

// 霍夫曼树节点结构（数组存储方式）
struct HTNode {
    uint64_t weight;  // 权重（频率）
    int parent;       // 双亲节点索引
    int lchild;       // 左孩子索引
    int rchild;       // 右孩子索引
    unsigned char data; // 存储的字节（仅叶子节点有效）
};

// 霍夫曼编码类型（字符指针数组）
using HuffmanCode = char**;

// 从数组中选择两个权重最小且parent为0的节点
void Select(HTNode* HT, int n, int& s1, int& s2) {
    uint64_t min1 = ULLONG_MAX;
    uint64_t min2 = ULLONG_MAX;
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

// 基于有序频率列表构建霍夫曼树并生成编码
void HuffmanCodingFromList(HTNode*& HT, HuffmanCode& HC,
                           const vector<pair<unsigned char, uint64_t>>& freqList, int n) {
    if (n <= 0) return;

    int m = 2 * n - 1;  // 总节点数 = 2n-1
    HT = new HTNode[m + 1];  // 下标从1开始

    // 1. 根据频率表初始化叶子节点，
    int i = 1;
    for (const auto& pair : freqList) {
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
        HT[i].data = 0;
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

    // 3. 从叶子到根求每个字符的哈夫曼编码
    HC = new char*[n + 1];  // 存储n个字符的编码
    char* cd = new char[n];  // 临时存放编码
    cd[n - 1] = '\0';       // 编码结束符

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

// 构建霍夫曼树并生成编码
void HuffmanCoding(HTNode*& HT, HuffmanCode& HC, 
                  const unordered_map<unsigned char, uint64_t>& freqMap, int n) {
    // 旧接口保留，但内部先把无序map转换成按字节值排序的有序列表，
    // 保证压缩和解压时构建完全相同的霍夫曼树
    vector<pair<unsigned char, uint64_t>> freqList(freqMap.begin(), freqMap.end());
    sort(freqList.begin(), freqList.end(),
         [](const auto& a, const auto& b) { return a.first < b.first; });
    HuffmanCodingFromList(HT, HC, freqList, n);
}

// 读取文件并统计字节频率
bool readFileAndCountFreq(const string& inputPath, vector<unsigned char>& fileData, 
                         unordered_map<unsigned char, uint64_t>& freqMap) {
    try {
        if (!fs::exists(inputPath)) {
            cerr << "错误：文件不存在: " << inputPath << endl;
            return false;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "错误：无法处理输入路径（可能是路径中包含特殊/非UTF-8字符）" << endl;
        cerr << "建议：先把文件拷贝到一个只包含英文和数字的路径下再试。" << endl;
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

    // 将频率表按字节值排序，保证顺序确定
    vector<pair<unsigned char, uint64_t>> freqList(freqMap.begin(), freqMap.end());
    sort(freqList.begin(), freqList.end(),
         [](const auto& a, const auto& b) { return a.first < b.first; });
    n = static_cast<int>(freqList.size());

    // 构建霍夫曼树和编码表（使用确定顺序的频率列表）
    HTNode* HT = nullptr;
    HuffmanCode HC = nullptr;
    HuffmanCodingFromList(HT, HC, freqList, n);

    // 创建字节到编码的映射
    unordered_map<unsigned char, string> codeMap;
    unordered_map<unsigned char, int> dataToIndex;
    for (int i = 1; i <= n; ++i) {
        dataToIndex[HT[i].data] = i;
        codeMap[HT[i].data] = HC[i];//从字节到哈夫曼编码的映射
    }

    // 写入压缩文件
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputPath << endl;
        return false;
    }

    // 写入频率表大小和频率表
    uint16_t freqMapSize = static_cast<uint16_t>(n);
    outFile.write(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

    // 频率表写出顺序与 freqList 完全一致，解压时按同样顺序重建
    for (const auto& p : freqList) {
        unsigned char byte = p.first;
        uint64_t freq = p.second;
        outFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte));//写入字节
        outFile.write(reinterpret_cast<const char*>(&freq), sizeof(freq));//写入频率
       
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
    try {
        uint64_t originalSize = fs::file_size(inputPath);
        uint64_t compressedSize = fs::file_size(outputPath);
        double compressionRatio = static_cast<double>(compressedSize) / originalSize;

        cout << "压缩成功！" << endl;
        cout << "原始大小：" << originalSize << " 字节" << endl;
        cout << "压缩后大小：" << compressedSize << " 字节" << endl;
        cout << "压缩比：" << compressionRatio << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "压缩成功！" << endl;
        cout << "提示：无法获取文件大小（路径可能包含特殊/非UTF-8字符）" << endl;
    }

    return true;
}

// 解压文件
bool decompressFile(const string& inputPath, const string& outputPath) {
    try {
        if (!fs::exists(inputPath)) {
            cerr << "错误：压缩文件不存在: " << inputPath << endl;
            return false;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "错误：无法处理压缩文件路径（可能是路径中包含特殊/非UTF-8字符）" << endl;
        cerr << "建议：先把压缩文件拷贝到一个只包含英文和数字的路径下再试。" << endl;
        return false;
    }
    
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) {
        cerr << "错误：无法打开压缩文件 " << inputPath << endl;
        return false;
    }

    // 读取频率表（同时保留读取顺序，确保与压缩时一致）
    unordered_map<unsigned char, uint64_t> freqMap;
    vector<pair<unsigned char, uint64_t>> freqList;
    uint16_t freqMapSize = 0;
    inFile.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

    if (!inFile || freqMapSize == 0) {
        cerr << "错误：压缩文件中的频率表无效" << endl;
        return false;
    }

    for (uint16_t i = 0; i < freqMapSize; ++i) {
        unsigned char byte;
        uint64_t freq;
        inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        if (!inFile) {
            cerr << "错误：读取频率表失败，压缩文件可能已损坏" << endl;
            return false;
        }
        freqMap[byte] = freq;
        freqList.emplace_back(byte, freq);
    }

    // 重建霍夫曼树（使用与压缩时完全相同的顺序）
    int n = static_cast<int>(freqList.size());
    HTNode* HT = nullptr;
    HuffmanCode HC = nullptr;
    HuffmanCodingFromList(HT, HC, freqList, n);

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
    uint64_t totalBytes = 0;//读取的总字节数
    uint64_t expectedTotal = 0;//读取的预期字节数
    for (const auto& pair : freqList) {
        expectedTotal += pair.second;
    }

    // 处理只有一个字符的情况
    if (n == 1) {
        for (uint64_t i = 0; i < expectedTotal; ++i) {
            outFile.write(reinterpret_cast<char*>(&HT[1].data), sizeof(HT[1].data));
        }
        // 单字符文件，写出的字节数等于预期
        totalBytes = expectedTotal;
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
    try {
        cout << "解压后大小：" << fs::file_size(outputPath) << " 字节" << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "解压后大小：无法获取（路径可能包含特殊/非UTF-8字符）" << endl;
    }

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