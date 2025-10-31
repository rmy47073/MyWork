#include <iostream>
using namespace std;

// 函数功能：计算n只猴子中最后当选猴王的编号
// 参数：n是猴子总数，m是报数退出的数字
// 返回值：当选猴王的原始编号(从1开始)，若输入无效返回-1
int findMonkeyKing(int n, int m) {
    // 检查输入有效性
    if (n < 1 || m < 1) {
        return -1; // 无效输入
    }
    
    int king = 0; // f(1) = 0，编号从0开始
    
    // 应用递推公式：f(n) = (f(n-1) + m) % n
    for (int i = 2; i <= n; i++) {
        king = (king + m) % i;
    }
    
    // 转换为从1开始的编号
    return king + 1;
}

int main() {
    int n, m;
    
    // 输入猴子数量
    cout << "请输入猴子的数量N: ";
    cin >> n;
    
    // 输入退出数字
    cout << "请输入报数退出的数字M: ";
    cin >> m;
    
    // 计算结果
    int result = findMonkeyKing(n, m);
    
    // 输出结果
    if (result == -1) {
        cout << "输入无效！猴子数量和退出数字必须为正整数。" << endl;
    } else {
        cout << "当每数到第" << m << "个猴子退出时，最后当选猴王的是原来的第" << result << "号猴子" << endl;
    }
    
    return 0;
}
    