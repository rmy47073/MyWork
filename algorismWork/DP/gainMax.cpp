#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*
4 列 n 行的方格表，每个格子有正整数奖金。放代币的规则：
每个格子最多一个代币
不能水平或垂直相邻（可以对角相邻）
最多放 2n 个代币（实际上由规则 2 限制，每行最多放 2 个）
目标：最大化被覆盖格子的奖金总和
*/
// 8 种合法行内放法（4bit对应 4 列，1=放代币，无水平相邻）
// 0:0000  1:0001  2:0010  4:0100  5:0101  8:1000  9:1001  10:1010
const int isLegal[] = {0,1,2,4,5,8,9,10};
vector<vector<int>> gainSize(int n) {
    vector<vector<int>> A(n, vector<int>(8));
    int a, b, c, d;
    for (int i = 0; i < n; i++){
        cin >> a >> b >> c >> d;   // a=左1列  b=左2列  c=左3列  d=左4列
        // 对每行预先算出 每一行的8 种放法的奖金和，避免 DP 中重复计算
        A[i][0]=0;          // 0000: 不放
        A[i][1]=d;          // 0001: 只放第 4 列
        A[i][2]=c;          // 0010: 只放第 3 列
        A[i][3]=b;          // 0100: 只放第 2 列
        A[i][4]=b+d;        // 0101: 放第 2、4 列
        A[i][5]=a;          // 1000: 只放第 1 列
        A[i][6]=a+d;        // 1001: 放第 1、4 列
        A[i][7]=a+c;        // 1010: 放第 1、3 列
    }
    return A;
}        


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<vector<int>> A = gainSize(n);
    int dp[n][8]={0};
    // 第 0 行 初始化：每种放法的奖金就是它本身
    for(int i=0;i<8;i++){
        dp[0][i]=A[0][i];
    }

    for (int i = 1; i < n; i++){
        for (int j = 0; j < 8; j++){ //遍历除了第一行的所有放法      
            for(int k=0;k<8;k++){ //每个元素都要遍历第i-1行的所有放法，选出第i-1行所有放法中最大的一个
                // 垂直相邻限制：
                //两行不能在同一列都有代币，使用二进制的与运算，两个数的相同位经过与&运算结果为0的才符合条件
                if((isLegal[j]&isLegal[k])==0){
                    dp[i][j]=max(dp[i][j],dp[i-1][k]+A[i][j]);//
                }
            }

        }
    }
    // 从最后一行 8 种放法中取最大值
    int maxResult=0;
    for(int i=0;i<8;i++){
        maxResult=max(maxResult,dp[n-1][i]);
    }
    cout<<maxResult<<endl;
}
