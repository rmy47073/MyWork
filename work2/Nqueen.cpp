#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Solution {
private:
    vector<vector<string>> result; // 存储所有解
    vector<string> board;          // 当前棋盘状态
    vector<bool> colUsed;          // 标记列是否被占用
    vector<bool> diag1;            // 标记主斜线（左上→右下）是否被占用
    vector<bool> diag2;            // 标记副斜线（右上→左下）是否被占用

    // 回溯函数：尝试在第row行放置皇后
    void backtrack(int n, int row) {
        // 终止条件：所有行都放置了皇后，记录当前方案
        if (row == n) {
            result.push_back(board);
            return;
        }

        // 尝试当前行的每一列
        for (int col = 0; col < n; col++) {
            // 计算当前位置所在的斜线索引
            int d1 = row - col + n - 1; // 主斜线：row - col 可能为负，加偏移量转正
            int d2 = row + col;         // 副斜线：row + col 非负

            // 检查冲突：列或斜线已被占用
            if (colUsed[col] || diag1[d1] || diag2[d2]) {
                continue; // 冲突，跳过当前列
            }

            // 放置皇后（标记状态）
            board[row][col] = 'Q';
            colUsed[col] = true;
            diag1[d1] = true;
            diag2[d2] = true;

            // 递归处理下一行
            backtrack(n, row + 1);

            // 回溯（撤销选择，恢复状态）
            board[row][col] = '.';
            colUsed[col] = false;
            diag1[d1] = false;
            diag2[d2] = false;
        }
    }

public:
    vector<vector<string>> solveNQueens(int n) {
        // 初始化棋盘：n行，每行n个'.'
        board = vector<string>(n, string(n, '.'));
        // 初始化标记数组（大小确保覆盖所有可能索引）
        colUsed = vector<bool>(n, false);
        diag1 = vector<bool>(2 * n - 1, false); // 主斜线共2n-1条
        diag2 = vector<bool>(2 * n - 1, false); // 副斜线共2n-1条

        backtrack(n, 0); // 从第0行开始放置
        return result;
    }
};

// 打印结果
void printResult(const vector<vector<string>>& res) {
    for (const auto& solution : res) {
        cout << "解法：" << endl;
        for (const string& row : solution) {
            cout << row << endl;
        }
        cout << endl;
    }
}

int main() {
    int n;
    cout << "请输入n：";
    cin >> n;

    Solution sol;
    vector<vector<string>> result = sol.solveNQueens(n);

    cout << "共" << result.size() << "种解法：" << endl;
    printResult(result);
    return 0;
}