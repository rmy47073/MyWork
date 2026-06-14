/* 在一个N*N的棋盘中，每个格子均有一个1到99的数字。
    你必须将N个皇后放置到棋盘中，且各皇后彼此不可互相攻击，并且放置皇后的N个位置中的数的和为最大。

输入：    
    第一行是整数N(4<=N<=20)
    之后有N行，每行有N个整数（介于0到99之间）
输出：
    可以放置N个皇后的最大的和
*/
#include<iostream>
#include<vector>

using namespace std;

int N;
vector<vector<int>> board;
vector<bool> col;
vector<bool> diag1;
vector<bool> diag2;
int best=0;

void gainSize(){//获取网格中各个格子的权值
    board.assign(N,vector<int>(N));
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            cin >> board[i][j];
}
//函数思路：剩下每一行和前面已选的
//列、主对角线、副对角线不冲突的最大权值，
//但剩下的行之间的选择没有考虑，没有考虑影响
//因此是乐观估计

int bound(int curRow,int curSum){//界函数，乐观估计选择当前格子的话全局可能会有的最大权值
    int total=curSum;
    for(int i=curRow;i<N;i++){
        int bestIncurRow=0;
        for(int j=0;j<N;j++)
            if(!col[j] && !diag1[i-j+N-1] && !diag2[i+j])
                bestIncurRow=max(bestIncurRow,board[i][j]);
        total+=bestIncurRow;
    }
    return total;
}

void solution(int curRow,int curSum){
    if(bound(curRow,curSum)<=best)
        return;//估计最大权值小于当前最大权值不考虑，剪枝
    if(curRow==N){
        if(curSum>best)
            best=curSum;
        return;//边界情况，若新权值大于当前最好情况，作更新并返回
    }
    for(int curCol=0;curCol<N;curCol++){              // 遍历当前行的每一列
        if(!col[curCol]&&!diag1[curRow-curCol+N-1]&&!diag2[curRow+curCol]){  // 列和两条对角线都没被占
            col[curCol]=true;                          // 占住当前列
            diag1[curRow-curCol+N-1]=true;                // 占住主对角线
            diag2[curRow+curCol]=true;                    // 占住副对角线

            solution(curRow+1,curSum+board[curRow][curCol]); // 递归放下一行

            col[curCol]=false;                         // 回溯：释放列
            diag1[curRow-curCol+N-1]=false;               // 回溯：释放主对角线
            diag2[curRow+curCol]=false;                   // 回溯：释放副对角线
        }
    }
}

int main(){
    cin >> N;
    col.assign(N,false);
    diag1.assign(2*N-1,false);
    diag2.assign(2*N-1,false);//初始化

    gainSize();
    solution(0,0);//从第零行开始
    cout<< best <<endl;
    return 0;
}
