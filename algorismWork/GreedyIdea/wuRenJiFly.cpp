#include <iostream>
#include <vector>
#include <queue>
using namespace std;

/**
 * 获取输入数据
 * 功能：读取n个无人机的k种飞行距离对应的飞行时间
 * n×k的二维数组，A[i][j]表示第i个无人机飞行第j种距离的飞行时间
 */
vector<vector<int>> gainSize(int n,int k){
    vector<vector<int>> A(n,vector<int>(k));
    for(int i=0;i<n;i++)
        for(int j=0;j<k;j++)
            cin >> A[i][j];
    return A;
}

/**
 * 算法思想：
 * 1. 初始状态：每个无人机先分配1次飞行
 * 2. 贪心策略：每次选择增加飞行时间最小的无人机来分配额外的飞行任务
 * 3. 比较每个无人机增加一次飞行的飞行时间增量
 * 
 */
long long solve(vector<vector<int>> A,int m){
    int n=A.size();      // 无人机数量
    int k=A[0].size();   // 每种无人机的飞行距离选项数量
    
    // length数组记录每个无人机当前使用的飞行距离选项索引
    // 初始时每个无人机都飞行1m
    vector<int> length(n,1);
    
    // remain表示还需要分配的额外飞行米数
    // 初始时每个无人机已经分配了1次1m飞行，所以剩余米数为m-n
    int remain=m-n;
    long long result=0;
    // 使用优先级队列，按飞行时间增量排序
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    // 初始化
    for(int i=0;i<n;i++){
        result+=A[i][0];
        if(length[i]<k){
            int add=A[i][length[i]]-A[i][length[i]-1];
            pq.push({add,i});
        }
    }
    
    // 贪心分配剩余的飞行米数
    while(remain>0){
        if(pq.empty())
            return -1;
        pair<int,int> best=pq.top();//取出当前增量最小值
        pq.pop();
        result+=best.first;//将最小增量累加到结果中
        length[best.second]++;//对应索引的无人机奉行长度加1
        remain--;
        if(length[best.second]<k){
            int add=A[best.second][length[best.second]]-A[best.second][length[best.second]-1];
            pq.push({add,best.second});//如果有的无人机还没有到最大飞行距离，就将其增量加入队列
        }
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,k,m;
    cin >> n >> k >> m;
    
    // 读取飞行时间矩阵数据
    vector<vector<int>> A=gainSize(n,k);
    
    // 使用贪心算法计算最小总飞行时间
    long long result=solve(A,m);
    
    // 输出结果
    cout << result << endl;
}