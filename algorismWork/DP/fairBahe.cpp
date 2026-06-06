/*尽可能公平的拔河游戏 某科研团队共有 n 人，体重分别是w1, w2, …, wn（都是正整数）。

为增进成员感情，该团队决定组织一场友谊拔河赛。考虑到其中或有”横推八马倒，倒曳九牛回”的大力士，因此这场友谊拔河赛不要求双方人数相当，而是希望两边的总体重差达到最小。
输入
    第一行 ：n （不超过<20001）
    之后是 n 个人的体重（都是整数，且所有人体重和不超过20000）
输出
    非增序输出两个队伍的总体重，以一个空格分隔

核心思路：从 n 个人中选一个子集，使其总重量尽可能接近 sum/2。
转化为子集和问题（0-1 背包），用布尔 DP 求解。
*/
#include <iostream>
#include <bitset>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> w(n);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> w[i];
        sum += w[i];//算出总体重
    }

    // dp[s] = true 表示存在某个子集总重为 s
    // 只需要开到 sum/2（超过一半的不用考虑）
    vector<bool> dp(sum / 2 + 1, false);
    dp[0] = true;  // 空集的和为 0

    // 0-1 背包：每个人选或不选
    // 逆序更新保证每个人最多用一次
    for (int i = 0; i < n; i++) {
        for (int s = sum / 2; s >= w[i]; s--) {
            if (dp[s - w[i]])
                dp[s] = true;
        }
    }

    // 从 sum/2 往下找第一个可达的和，即最接近 sum/2 的子集
    int a, b;
    for (int s = sum / 2; s >= 0; s--) {
        if (dp[s]) {
            a = s;
            b = sum - s;
            break;
        }
    }
    // 非增序输出
    cout << max(a, b) << " " << min(a, b) << endl;
    return 0;
}
