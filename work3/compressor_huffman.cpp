#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <cstdint>

using namespace std;
namespace fs = filesystem;

// Huffman树节点结构
struct HuffmanNode {
    unsigned char data;       // 存储的字节（仅叶子节点有效）
    uint64_t freq;            // 频率
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(unsigned char d, uint64_t f) : data(d), freq(f), left(nullptr), right(nullptr) {}
    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

// 优先队列比较函数（最小堆）
struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// 构建Huffman树
HuffmanNode* buildHuffmanTree(const unordered_map<unsigned char, uint64_t>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNode> minHeap;

    // 所有字节入堆
    for (const auto& pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // 构建树
    while (minHeap.size() > 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        // 中间节点数据设为0xFF（无效值），频率为左右子节点之和
        HuffmanNode* parent = new HuffmanNode(0xFF, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        minHeap.push(parent);
    }

    return minHeap.empty() ? nullptr : minHeap.top();
}

// 生成Huffman编码表（递归）
void generateHuffmanCodes(HuffmanNode* root, string code, unordered_map<unsigned char, string>& codeMap) {
    if (!root) return;

    // 叶子节点（有效数据）
    if (!root->left && !root->right) {
        codeMap[root->data] = code;
        return;
    }

    generateHuffmanCodes(root->left, code + "0", codeMap);
    generateHuffmanCodes(root->right, code + "1", codeMap);
}

// 读取文件并统计字节频率
bool readFileAndCountFreq(const string& inputPath, vector<unsigned char>& fileData, unordered_map<unsigned char, uint64_t>& freqMap) {
    // 检查文件是否存在
    if (!fs::exists(inputPath)) {
        cerr << "错误：文件不存在: " << inputPath << endl;
        cerr << "提示：请检查文件路径是否正确" << endl;
        return false;
    }
    
    ifstream inFile(inputPath, ios::binary | ios::ate);
    if (!inFile.is_open()) {
        cerr << "错误：无法打开文件 " << inputPath << endl;
        cerr << "提示：文件可能被其他程序占用或没有读取权限" << endl;
        return false;
    }

    // 获取文件大小并分配缓冲区
    streampos fileSize = inFile.tellg();
    fileData.resize(static_cast<size_t>(fileSize));
    inFile.seekg(0, ios::beg);
    inFile.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    inFile.close();

    // 统计频率
    freqMap.clear();
    for (unsigned char byte : fileData) {
        freqMap[byte]++;
    }

    return true;
}

// 压缩文件
bool compressFile(const string& inputPath, const string& outputPath) {
    vector<unsigned char> fileData;
    unordered_map<unsigned char, uint64_t> freqMap;

    // 读取文件并统计频率
    if (!readFileAndCountFreq(inputPath, fileData, freqMap)) {
        return false;
    }

    // 构建Huffman树和编码表
    HuffmanNode* root = buildHuffmanTree(freqMap);
    if (!root) {
        cerr << "错误：文件为空，无法压缩" << endl;
        return false;
    }

    unordered_map<unsigned char, string> codeMap;
    generateHuffmanCodes(root, "", codeMap);
    delete root; // 编码表生成后，树可释放

    // 写入压缩文件（格式：频率表大小 + 频率表 + 补零位数 + 压缩数据）
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputPath << endl;
        return false;
    }

    // 写入频率表大小（1字节，最多256种字节）
    uint8_t freqMapSize = static_cast<uint8_t>(freqMap.size());
    outFile.write(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));

    // 写入频率表（每个条目：字节(1B) + 频率(8B)）
    for (const auto& pair : freqMap) {
        outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));
        outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

    // 生成压缩比特流
    string bitStream;
    for (unsigned char byte : fileData) {
        bitStream += codeMap[byte];
    }

    // 计算补零位数（确保比特流长度是8的倍数）
    uint8_t paddingBits = (8 - (bitStream.size() % 8)) % 8;
    for (int i = 0; i < paddingBits; i++) {
        bitStream += "0";
    }

    // 写入补零位数
    outFile.write(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    // 写入压缩数据（将比特流转换为字节）
    for (size_t i = 0; i < bitStream.size(); i += 8) {
        string byteStr = bitStream.substr(i, 8);
        unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 2));
        outFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    outFile.close();

    // 计算并显示压缩比
    uint64_t originalSize = fs::file_size(inputPath);
    uint64_t compressedSize = fs::file_size(outputPath);
    double compressionRatio = static_cast<double>(compressedSize) / originalSize;

    cout << "压缩成功！" << endl;
    cout << "原始大小：" << originalSize << " 字节" << endl;
    cout << "压缩后大小：" << compressedSize << " 字节" << endl;
    cout << "压缩比：" << compressionRatio << "（越小越好）" << endl;

    return true;
}

// 从压缩文件读取频率表
bool readFreqMap(ifstream& inFile, unordered_map<unsigned char, uint64_t>& freqMap) {
    uint8_t freqMapSize;
    inFile.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));
    if (inFile.gcount() != sizeof(freqMapSize)) {
        return false;
    }

    freqMap.clear();
    for (int i = 0; i < freqMapSize; i++) {
        unsigned char byte;
        uint64_t freq;

        inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
        inFile.read(reinterpret_cast<char*>(&freq), sizeof(freq));

        if (inFile.gcount() != sizeof(freq)) {
            return false;
        }

        freqMap[byte] = freq;
    }

    return true;
}

// 解压文件
bool decompressFile(const string& inputPath, const string& outputPath) {
    // 检查文件是否存在
    if (!fs::exists(inputPath)) {
        cerr << "错误：压缩文件不存在: " << inputPath << endl;
        cerr << "提示：请检查文件路径是否正确" << endl;
        return false;
    }
    
    ifstream inFile(inputPath, ios::binary);
    if (!inFile.is_open()) {
        cerr << "错误：无法打开压缩文件 " << inputPath << endl;
        cerr << "提示：文件可能被其他程序占用或没有读取权限" << endl;
        return false;
    }

    // 读取频率表
    unordered_map<unsigned char, uint64_t> freqMap;
    if (!readFreqMap(inFile, freqMap)) {
        cerr << "错误：压缩文件格式损坏（频率表读取失败）" << endl;
        inFile.close();
        return false;
    }

    // 构建Huffman树
    HuffmanNode* root = buildHuffmanTree(freqMap);
    if (!root) {
        cerr << "错误：Huffman树构建失败" << endl;
        inFile.close();
        return false;
    }

    // 读取补零位数
    uint8_t paddingBits;
    inFile.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));
    if (inFile.gcount() != sizeof(paddingBits)) {
        cerr << "错误：压缩文件格式损坏（补零位数读取失败）" << endl;
        delete root;
        inFile.close();
        return false;
    }

    // 读取压缩数据并转换为比特流
    string bitStream;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        // 将字节转换为8位二进制字符串（补前导零）
        bitStream += bitset<8>(byte).to_string();
    }

    // 去除补零的比特
    if (paddingBits > 0) {
        bitStream.resize(bitStream.size() - paddingBits);
    }

    inFile.close();

    // 解码比特流
    ofstream outFile(outputPath, ios::binary);
    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputPath << endl;
        delete root;
        return false;
    }

    HuffmanNode* currentNode = root;
    uint64_t totalBytes = 0;
    uint64_t expectedTotal = 0;
    for (const auto& pair : freqMap) {
        expectedTotal += pair.second;
    }

    // 处理只有一个字符的特殊情况
    if (!root->left && !root->right) {
        // 只有一个字符，所有比特都代表这个字符
        for (uint64_t i = 0; i < expectedTotal; i++) {
            outFile.write(reinterpret_cast<char*>(&root->data), sizeof(root->data));
        }
    } else {
        // 正常情况：多个字符
        for (char bit : bitStream) {
            if (!currentNode) {
                cerr << "错误：解码过程中遇到空指针" << endl;
                delete root;
                outFile.close();
                return false;
            }
            
            if (bit == '0') {
                currentNode = currentNode->left;
            } else if (bit == '1') {
                currentNode = currentNode->right;
            } else {
                // 跳过无效字符
                continue;
            }

            // 检查是否到达叶子节点
            if (currentNode && !currentNode->left && !currentNode->right) {
                outFile.write(reinterpret_cast<char*>(&currentNode->data), sizeof(currentNode->data));
                currentNode = root;
                totalBytes++;

                // 提前退出（避免处理多余比特）
                if (totalBytes >= expectedTotal) {
                    break;
                }
            }
        }
    }

    outFile.close();
    delete root;

    if (totalBytes != expectedTotal) {
        cerr << "警告：解码字节数与预期不符，可能存在数据损坏" << endl;
        return false;
    }

    cout << "解压成功！" << endl;
    cout << "输出文件：" << outputPath << endl;
    cout << "解压后大小：" << fs::file_size(outputPath) << " 字节" << endl;

    return true;
}

// 显示帮助信息
void showHelp() {
    cout << "\n========================================" << endl;
    cout << "Huffman文件压缩工具" << endl;
    cout << "支持图像、WORD、PDF、PPT、视频、音频等所有文件格式" << endl;
    cout << "========================================\n" << endl;
    cout << "用法：" << endl;
    cout << "  压缩：-c <输入文件路径> <输出压缩文件路径>" << endl;
    cout << "  解压：-d <输入压缩文件路径> <输出文件路径>" << endl;
    cout << "  退出：quit 或 exit" << endl;
    cout << "\n示例：" << endl;
    cout << "  压缩PDF：-c document.pdf document.huf" << endl;
    cout << "  解压视频：-d video.huf video.mp4" << endl;
    cout << "\n请输入命令（或输入 quit 退出）：" << endl;
}

// 处理命令
bool processCommand(const string& mode, const string& inputPath, const string& outputPath) {
    if (mode == "-c" || mode == "c") {
        // 压缩模式
        return compressFile(inputPath, outputPath);
    } else if (mode == "-d" || mode == "d") {
        // 解压模式
        return decompressFile(inputPath, outputPath);
    } else {
        cerr << "错误：无效的模式参数 " << mode << endl;
        cerr << "请使用 -c 压缩 或 -d 解压" << endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    // 如果有命令行参数，直接执行
    if (argc == 4) {
        string mode = argv[1];
        string inputPath = argv[2];
        string outputPath = argv[3];
        
        if (!processCommand(mode, inputPath, outputPath)) {
            return 1;
        }
        return 0;
    }
    
    // 交互式模式
    showHelp();
    
    string line;
    while (true) {
        cout << "\n> ";
        getline(cin, line);
        
        // 去除首尾空格
        while (!line.empty() && (line[0] == ' ' || line[0] == '\t')) {
            line.erase(0, 1);
        }
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t')) {
            line.pop_back();
        }
        
        // 检查退出命令
        if (line == "quit" || line == "exit" || line == "q") {
            cout << "再见！" << endl;
            break;
        }
        
        // 检查帮助命令
        if (line == "help" || line == "h" || line == "-h" || line == "--help") {
            showHelp();
            continue;
        }
        
        // 解析命令
        if (line.empty()) {
            continue;
        }
        
        // 分割字符串
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
        
        // 检查参数数量
        if (tokens.size() != 3) {
            cerr << "错误：命令格式不正确" << endl;
            cerr << "正确格式：<模式> <输入文件> <输出文件>" << endl;
            cerr << "例如：-c input.txt output.huf" << endl;
            continue;
        }
        
        string mode = tokens[0];
        string inputPath = tokens[1];
        string outputPath = tokens[2];
        
        // 执行命令
        processCommand(mode, inputPath, outputPath);
    }
    
    return 0;
}