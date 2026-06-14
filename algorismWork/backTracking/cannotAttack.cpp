/* 三色 N 皇后问题
 * 题目：n×n 棋盘，放 n 个红皇后、n 个蓝皇后、n 个绿皇后
 *       同一格只能放一个皇后
 *       同色皇后不能互相攻击（即每色独立满足 N 皇后约束）
 *       不同色皇后之间可以互相攻击（不约束）
 * 输入：n（1≤n≤7）
 * 输出：放置方法总数
 * 样例：n=6 → 24
 */

#include<iostream>
#include<vector>
using namespace std;
// cols[color][row] = 该颜色在第 row 行放置的列号
vector<vector<int>> cols; 
int count=0;                 // 解的个数
int N;                       // 棋盘大小

/* 判断同色皇后能否放置
 * 参数:
 *   row, col  待放置的坐标
 *   cols     当前的放置记录
 *   flag     颜色编号 (0/1/2)
 * 返回: true=可以放, false=会被同色攻击
 *
 * 三个判断条件:
 *   列冲突:  同一列已有该色皇后
 *   主对角线冲突:  i - cols[i] == row - col
 *   副对角线冲突:  i + cols[i] == row + col
 *
 * 注意: 只检查 i < row，即当前行以上的所有行，
 */
bool isValid(int row, int col, vector<vector<int>> cols, int flag){
    for(int i = 0; i < row; i++){
        // 1) 同列已有同色皇后
        if(cols[flag][i] == col)
            return false;
        // 2) 同一条主对角线 ，向右下斜
        //    i - cols[i] 是行号减列号，同一主对角线上该值相等
        if(i - cols[flag][i] == row - col)
            return false;
        // 3) 同一条副对角线 ，向左下斜
        //    i + cols[i] 是行号加列号，同一副对角线上该值相等
        if(i + cols[flag][i] == row + col)
            return false;
    }
    return true;
}
/*  回溯
 * 思路: 从第 0 行开始，每行依次放红、蓝、绿各一个皇后
 *       要求三个皇后放在不同的列 (i!=j 且 i!=k 且 j!=k)
 *       每种颜色独立满足 N 皇后约束
 * 三重循环枚举三种颜色的列号：
 *   i ： 红皇后列号, j ： 蓝皇后列号, k ：绿皇后列号
 *   用 continue 跳过不合法的情况：
 *     放不了该颜色 就 continue 跳过内层循环
 *     与已放颜色同列 就 continue 换一列
 */
void solution(int row){
    // 递归出口：所有行都放置完毕，找到一个解
    if(row == N){
        count++;
        return;
    }
    //枚举红皇后 ，flag=0 的列
    for(int i = 0; i < N; i++){
        if(!isValid(row, i, cols, 0)) continue;   // 红皇后不能放第 i 列
        cols[0][row] = i;                          // 放红皇后
        // 枚举蓝皇后 ,flag=1的列 
        for(int j = 0; j < N; j++){
            if(j == i || !isValid(row, j, cols, 1)) continue;  // 蓝皇后不能放第 j 列
            cols[1][row] = j;                                   // 放蓝皇后
            // 枚举绿皇后,flag=2的列
            for(int k = 0; k < N; k++){
                if(k == i || k == j || !isValid(row, k, cols, 2)) continue;
                cols[2][row] = k;     // 放绿皇后
                solution(row + 1);      // 递归处理下一行
                cols[2][row] = -1;     // 回溯：撤销绿皇后
            }
            cols[1][row] = -1;          // 回溯：撤销蓝皇后
        }
        cols[0][row] = -1;              // 回溯：撤销红皇后
    }
}
int main(){
    cin >> N;
    // 初始化: 3 种颜色, 每种 N 行, 初始全 0
    cols.assign(3, vector<int>(N));
    count = 0;
    solution(0);        // 从第 0 行开始搜索
    cout << count << endl;
    return 0;
}
