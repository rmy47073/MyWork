/**
 * 拔河比赛公平分配问题之动态规划解法
 * 
 * 问题描述：
 *   将n个队员分成两队进行拔河比赛，使两队总重量差最小
 * 
 * 核心思路：
 *   找到一个子集，其和尽可能接近总重量的一半
 */
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 输入队员数量
    int n;
    cin >> n;
    // 存储队员体重
    vector<int> w(n);
    // 计算总体重
    int sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> w[i];
        sum += w[i];
    }
    // dp[j]: 是否能组成重量j
    vector<bool> dp(sum / 2 + 1, false);
    // 初始状态：可以组成重量0
    dp[0] = true;

    // 核心：0-1背包问题，逆序遍历避免重复选择
    for (int i = 0; i<n; i++) {           // 遍历每个队员
        for (int j = sum/2; j >= w[i]; j--) { // 逆序遍历重量
            if (dp[j-w[i]]) {             // 如果可组成j-w[i]的重量，则j的重量也可组成
                dp[j]=true;
            }
        }
    }
    // 寻找最接近sum/2的可组成重量
    int a, b;
    for (int j = sum / 2; j >= 0; j--) {
        if (dp[j]) {                      // 找到第一个可组成的重量
            a = j;
            b = sum - j;
            break;
        }
    }
    // 输出两队重量（先大后小）
    cout << max(a, b) << " " << min(a, b);
    return 0;
}