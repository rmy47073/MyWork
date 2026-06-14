// BAT得知有一块富饶的土地里埋了很多金子，因为得到消息的同行很多，
// 因此他希望第一时间挖光所有的地，于是他招兵买马招聘了很多的矿工来帮他挖这批金子。
// 这块地由m*n个排列成矩形阵列的格子组成。
// 每个矿工只能挖他所在的格子与他所在位置相邻的上、下、左、右4 个格子。
// 设计一个确定矿工位置的算法，使得可以挖到所有的格子，且所使用的矿工人数最少。

// 输入：
//     2个正整数m和n (1≤m,n≤12)。
// 输出:
//     计算出的最少可能矿工人数。

#include<iostream>
#include<vector>

using namespace std;

int M,N;
int best;

bool inBound(int r,int c){
    return r>=0 && r<M && c>=0 && c<N;
}

// 贪心法求初始可行解
int greedyInit(){
    vector<vector<bool>> vis(M,vector<bool>(N,false));
    int cnt=0;
    for(int r=0;r<M;r++){
        for(int c=0;c<N;c++){
            if(vis[r][c]) continue;
            cnt++;
            vis[r][c]=true;
            if(inBound(r-1,c)) vis[r-1][c]=true;
            if(inBound(r+1,c)) vis[r+1][c]=true;
            if(inBound(r,c-1)) vis[r][c-1]=true;
            if(inBound(r,c+1)) vis[r][c+1]=true;
        }
    }
    return cnt;
}

int bound(int remain){
    return (remain+4)/5;  // 还需要的矿工数（向下取）
}

void dfs(int placed,vector<vector<bool>>& vis){
    // 计算还剩几个格子未覆盖
    int remain=0;
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)
            if(!vis[i][j]) remain++;

    // 剪枝：当前矿工 + 至少还需要 >= 已知最优解
    if(placed+bound(remain) >= best) return;

    // 全部覆盖，更新最优解
    if(remain==0){
        best=placed;
        return;
    }

    // 找第一个未覆盖的格子
    int curRow=-1,curCol=-1;
    for(int i=0;i<M && curRow==-1;i++)
        for(int j=0;j<N && curRow==-1;j++)
            if(!vis[i][j]){curRow=i;curCol=j;}

    // 候选矿工位置：(curRow,curCol) —— 矿工在 curRow,curCol
    {
        int mr=curRow,mc=curCol;
        bool a1=!vis[mr][mc]; if(a1) vis[mr][mc]=true;
        bool a2=inBound(mr-1,mc)&&!vis[mr-1][mc]; if(a2) vis[mr-1][mc]=true;
        bool a3=inBound(mr+1,mc)&&!vis[mr+1][mc]; if(a3) vis[mr+1][mc]=true;
        bool a4=inBound(mr,mc-1)&&!vis[mr][mc-1]; if(a4) vis[mr][mc-1]=true;
        bool a5=inBound(mr,mc+1)&&!vis[mr][mc+1]; if(a5) vis[mr][mc+1]=true;
        dfs(placed+1,vis);
        if(a1) vis[mr][mc]=false;
        if(a2) vis[mr-1][mc]=false;
        if(a3) vis[mr+1][mc]=false;
        if(a4) vis[mr][mc-1]=false;
        if(a5) vis[mr][mc+1]=false;
    }
    // 候选矿工位置：(curRow-1,curCol)
    if(inBound(curRow-1,curCol)){
        int mr=curRow-1,mc=curCol;
        bool a1=!vis[mr][mc]; if(a1) vis[mr][mc]=true;
        bool a2=inBound(mr-1,mc)&&!vis[mr-1][mc]; if(a2) vis[mr-1][mc]=true;
        bool a3=inBound(mr+1,mc)&&!vis[mr+1][mc]; if(a3) vis[mr+1][mc]=true;
        bool a4=inBound(mr,mc-1)&&!vis[mr][mc-1]; if(a4) vis[mr][mc-1]=true;
        bool a5=inBound(mr,mc+1)&&!vis[mr][mc+1]; if(a5) vis[mr][mc+1]=true;
        dfs(placed+1,vis);
        if(a1) vis[mr][mc]=false;
        if(a2) vis[mr-1][mc]=false;
        if(a3) vis[mr+1][mc]=false;
        if(a4) vis[mr][mc-1]=false;
        if(a5) vis[mr][mc+1]=false;
    }
    // 候选矿工位置：(curRow+1,curCol)
    if(inBound(curRow+1,curCol)){
        int mr=curRow+1,mc=curCol;
        bool a1=!vis[mr][mc]; if(a1) vis[mr][mc]=true;
        bool a2=inBound(mr-1,mc)&&!vis[mr-1][mc]; if(a2) vis[mr-1][mc]=true;
        bool a3=inBound(mr+1,mc)&&!vis[mr+1][mc]; if(a3) vis[mr+1][mc]=true;
        bool a4=inBound(mr,mc-1)&&!vis[mr][mc-1]; if(a4) vis[mr][mc-1]=true;
        bool a5=inBound(mr,mc+1)&&!vis[mr][mc+1]; if(a5) vis[mr][mc+1]=true;
        dfs(placed+1,vis);
        if(a1) vis[mr][mc]=false;
        if(a2) vis[mr-1][mc]=false;
        if(a3) vis[mr+1][mc]=false;
        if(a4) vis[mr][mc-1]=false;
        if(a5) vis[mr][mc+1]=false;
    }
    // 候选矿工位置：(curRow,curCol-1)
    if(inBound(curRow,curCol-1)){
        int mr=curRow,mc=curCol-1;
        bool a1=!vis[mr][mc]; if(a1) vis[mr][mc]=true;
        bool a2=inBound(mr-1,mc)&&!vis[mr-1][mc]; if(a2) vis[mr-1][mc]=true;
        bool a3=inBound(mr+1,mc)&&!vis[mr+1][mc]; if(a3) vis[mr+1][mc]=true;
        bool a4=inBound(mr,mc-1)&&!vis[mr][mc-1]; if(a4) vis[mr][mc-1]=true;
        bool a5=inBound(mr,mc+1)&&!vis[mr][mc+1]; if(a5) vis[mr][mc+1]=true;
        dfs(placed+1,vis);
        if(a1) vis[mr][mc]=false;
        if(a2) vis[mr-1][mc]=false;
        if(a3) vis[mr+1][mc]=false;
        if(a4) vis[mr][mc-1]=false;
        if(a5) vis[mr][mc+1]=false;
    }
    // 候选矿工位置：(curRow,curCol+1)
    if(inBound(curRow,curCol+1)){
        int mr=curRow,mc=curCol+1;
        bool a1=!vis[mr][mc]; if(a1) vis[mr][mc]=true;
        bool a2=inBound(mr-1,mc)&&!vis[mr-1][mc]; if(a2) vis[mr-1][mc]=true;
        bool a3=inBound(mr+1,mc)&&!vis[mr+1][mc]; if(a3) vis[mr+1][mc]=true;
        bool a4=inBound(mr,mc-1)&&!vis[mr][mc-1]; if(a4) vis[mr][mc-1]=true;
        bool a5=inBound(mr,mc+1)&&!vis[mr][mc+1]; if(a5) vis[mr][mc+1]=true;
        dfs(placed+1,vis);
        if(a1) vis[mr][mc]=false;
        if(a2) vis[mr-1][mc]=false;
        if(a3) vis[mr+1][mc]=false;
        if(a4) vis[mr][mc-1]=false;
        if(a5) vis[mr][mc+1]=false;
    }
}

int main(){
    cin>>M>>N;
    vector<vector<bool>> vis(M,vector<bool>(N,false));
    best=greedyInit();   // 贪心求上界
    dfs(0,vis);          // 分支限界搜索
    cout<<best<<endl;
    return 0;
}
  

