#include "HuffmanTree.h"
#include <queue>
#include <algorithm>

using namespace std;

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    deleteTree(root);
}

void HuffmanTree::buildTree(const map<unsigned char, unsigned long long>& frequencies) {
    if (frequencies.empty()) {
        root = nullptr;
        return;
    }

    // 如果只有一个字符，创建特殊树
    if (frequencies.size() == 1) {
        auto it = frequencies.begin();
        root = make_shared<HuffmanNode>(it->first, it->second, true);
        return;
    }

    // 使用优先队列构建Huffman树
    priority_queue<shared_ptr<HuffmanNode>, 
                   vector<shared_ptr<HuffmanNode>>, 
                   CompareNodes> pq;

    // 创建叶子节点并加入优先队列
    for (const auto& pair : frequencies) {
        auto node = make_shared<HuffmanNode>(pair.first, pair.second, true);
        pq.push(node);
    }

    // 构建树
    while (pq.size() > 1) {
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        auto parent = make_shared<HuffmanNode>(0, left->frequency + right->frequency, false);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root = pq.top();
}

map<unsigned char, string> HuffmanTree::generateCodes() {
    map<unsigned char, string> codes;
    if (root) {
        generateCodesRecursive(root, "", codes);
    }
    return codes;
}

void HuffmanTree::generateCodesRecursive(shared_ptr<HuffmanNode> node,
                                         const string& code,
                                         map<unsigned char, string>& codes) {
    if (!node) return;

    if (node->isLeaf) {
        codes[node->data] = code.empty() ? "0" : code;  // 处理单字符情况
    } else {
        if (node->left) {
            generateCodesRecursive(node->left, code + "0", codes);
        }
        if (node->right) {
            generateCodesRecursive(node->right, code + "1", codes);
        }
    }
}

void HuffmanTree::rebuildTreeFromCodes(const map<unsigned char, string>& codes) {
    root = make_shared<HuffmanNode>(0, 0, false);

    for (const auto& pair : codes) {
        auto current = root;
        const string& code = pair.second;

        for (char bit : code) {
            if (bit == '0') {
                if (!current->left) {
                    current->left = make_shared<HuffmanNode>(0, 0, false);
                }
                current = current->left;
            } else if (bit == '1') {
                if (!current->right) {
                    current->right = make_shared<HuffmanNode>(0, 0, false);
                }
                current = current->right;
            }
        }
        current->isLeaf = true;
        current->data = pair.first;
    }
}

void HuffmanTree::deleteTree(std::shared_ptr<HuffmanNode> node) {
    // 使用智能指针，自动管理内存，这里可以留空或用于其他清理工作
}

