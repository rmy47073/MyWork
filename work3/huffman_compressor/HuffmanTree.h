#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <memory>

using namespace std;

// Huffman树节点
struct HuffmanNode {
    unsigned char data;           // 字符数据（叶子节点有效）
    unsigned long long frequency;  // 频率
    bool isLeaf;                   // 是否为叶子节点
    shared_ptr<HuffmanNode> left;
    shared_ptr<HuffmanNode> right;

    HuffmanNode(unsigned char d, unsigned long long freq, bool leaf = false)
        : data(d), frequency(freq), isLeaf(leaf), left(nullptr), right(nullptr) {}
};

// 用于优先队列的比较器
struct CompareNodes {
    bool operator()(const shared_ptr<HuffmanNode>& a, 
                   const shared_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    }
};

// Huffman树类
class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree();

    // 构建Huffman树
    void buildTree(const map<unsigned char, unsigned long long>& frequencies);
    
    // 生成编码表
    map<unsigned char, string> generateCodes();
    
    // 获取根节点
    shared_ptr<HuffmanNode> getRoot() const { return root; }
    
    // 从编码表重建树（用于解码）
    void rebuildTreeFromCodes(const map<unsigned char, string>& codes);

private:
    shared_ptr<HuffmanNode> root;
    
    // 递归生成编码
    void generateCodesRecursive(shared_ptr<HuffmanNode> node, 
                                const string& code,
                                map<unsigned char, string>& codes);
    
    // 递归删除节点
    void deleteTree(shared_ptr<HuffmanNode> node);
};

#endif // HUFFMAN_TREE_H

