#include <iostream>
using namespace std;

// 定义集合的最大容量
#define MAX_SIZE 100

// 检查元素是否在集合中
bool isInSet(int set[], int size, int element) {
    for (int i = 0; i < size; i++) {
        if (set[i] == element) {
            return true;
        }
    }
    return false;
}

// 向集合中添加元素（确保不重复）
void addElement(int set[], int &size, int element) {
    // 检查集合是否已满
    if (size >= MAX_SIZE) {
        cout << "集合已满，无法添加元素！" << endl;
        return;
    }
    
    // 检查元素是否已存在，不存在则添加
    if (!isInSet(set, size, element)) {
        set[size] = element;
        size++;
    }
}

// 从键盘输入集合元素
void inputSet(int set[], int &size, const char *setName) {
    int n, element;
    size = 0; // 初始化集合大小为0
    
    while(true) {
        cout << "请输入" << setName << "的元素个数（1到" << MAX_SIZE << "之间的整数）: ";
     cin >> n;
    
       // 检查输入是否合法
        if (n <= 0 || n > MAX_SIZE) {
            cout << "无效的元素个数，必须是1到" << MAX_SIZE << "之间的整数" << endl;
            continue;
    }
        break;
}

    
    cout << "请输入" << setName << "的" << n << "个整数元素（用空格分隔）: ";
    for (int i = 0; i < n; i++) {
        cin >> element;
        addElement(set, size, element);
    }
    
    // 提示实际存储的元素数量（可能因去重而减少）
    if (size < n) {
        cout << setName << "中存在" << (n - size) << "个重复元素，已去重" << endl;
        cout << setName << "实际包含" << size << "个元素" << endl;
    }
}

// 打印集合
void printSet(int set[], int size, const char *setName) {
    cout << setName << " = { ";
    for (int i = 0; i < size; i++) {
        cout << set[i];
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << " }" << endl;
}

// 计算两个集合的交集
void intersection(int setA[], int sizeA, int setB[], int sizeB, int result[], int &sizeResult) {
    sizeResult = 0;
    // 遍历集合A的每个元素，检查是否也在集合B中
    for (int i = 0; i < sizeA; i++) {
        if (isInSet(setB, sizeB, setA[i])) {
            result[sizeResult] = setA[i];
            sizeResult++;
        }
    }
}

// 计算两个集合的并集
void unionSet(int setA[], int sizeA, int setB[], int sizeB, int result[], int &sizeResult) {
    sizeResult = 0;
    // 先添加集合A的所有元素
    for (int i = 0; i < sizeA; i++) {
        result[sizeResult] = setA[i];
        sizeResult++;
    }
    // 再添加集合B中不在集合A中的元素
    for (int i = 0; i < sizeB; i++) {
        if (!isInSet(setA, sizeA, setB[i])) {
            result[sizeResult] = setB[i];
            sizeResult++;
        }
    }
}

// 计算两个集合的差集 (A - B)
void difference(int setA[], int sizeA, int setB[], int sizeB, int result[], int &sizeResult) {
    sizeResult = 0;
    // 遍历集合A的每个元素，只保留不在集合B中的元素
    for (int i = 0; i < sizeA; i++) {
        if (!isInSet(setB, sizeB, setA[i])) {
            result[sizeResult] = setA[i];
            sizeResult++;
        }
    }
}

int main() {
    int setA[MAX_SIZE], setB[MAX_SIZE];
    int sizeA, sizeB;
    int result[MAX_SIZE], sizeResult;
    
    // 输入两个集合
    inputSet(setA, sizeA, "集合A");
    inputSet(setB, sizeB, "集合B");
    
    // 打印输入的集合
    cout << endl << "你输入的集合为：" << endl;
    printSet(setA, sizeA, "集合A");
    printSet(setB, sizeB, "集合B");
    
    // 计算并打印交集
    intersection(setA, sizeA, setB, sizeB, result, sizeResult);
    printSet(result, sizeResult, "A与B的交集");
    
    // 计算并打印并集
    unionSet(setA, sizeA, setB, sizeB, result, sizeResult);
    printSet(result, sizeResult, "A与B的并集");
    
    // 计算并打印差集 A-B
    difference(setA, sizeA, setB, sizeB, result, sizeResult);
    printSet(result, sizeResult, "A与B的差集 (A-B)");
    
    // 计算并打印差集 B-A
    difference(setB, sizeB, setA, sizeA, result, sizeResult);
    printSet(result, sizeResult, "B与A的差集 (B-A)");
    
    return 0;
}
