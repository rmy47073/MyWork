#include <iostream>
#include <vector>
#include<queue>
#include <algorithm>
using namespace std;

/**
 * 获取输入数据并构建优先队列
 * 背景：三项全能训练：游泳、自行车、长跑
 * 约束条件：泳池独占，只能一人游泳；自行车和长跑可以多人同时进行
 * 
 * 关键观察：
 * 1. 所有队员必须按顺序进行活动
 * 2. 自行车和长跑可以并行进行
 * 3. 总时间由游泳串行时间和后续活动并行时间决定
 * 
 */
priority_queue<pair<int,int>> gainGrid(int n){
    priority_queue<pair<int, int>> pq;  // 最大堆，按bike+run降序排序
    int a,b,c;
    for(int i=0;i<n;i++){
        cin >> a >> b >> c;  // a:游泳时间, b:自行车时间, c:长跑时间
        // 将(bike+run, swim)存入优先队列
        // 按(bike+run)降序排列，优先处理后续活动时间长的队员
        pq.push({b+c,a});
    }
    return pq;
}

/**
 * 核心贪心算法：计算最小总训练时间
 * 1. 按(bike+run)时间降序排列队员
 * 2. 让后续活动时间长的队员先游泳，减少后续等待时间
 * 3. 动态调整游泳序列和并行活动的时间安排
 * 
 */
int solve(priority_queue<pair<int,int>> A,int n){
    int result=0;
    int longest=0;  // 记录当前时间最靠后的后续活动时间（bike+run）
    
    // 取出第一个队员，其后续活动时间最长的
    pair<int,int> best = A.top();
    int first=best.first;    // bike+run时间
    int second=best.second;  // swim时间
    int need=second;         // 累计游泳时间
    longest=first;           // 当前时间最靠后的后续活动时间
    A.pop();
    
    int temp=0;  // 临时存储游泳时间的累积值

    // 处理剩余队员
    for(int i=1;i<n;i++){
        pair<int,int> p=A.top();
        A.pop();
        
        // 如果当前队员的游泳时间加上临时累积时间，再加上其后续活动时间
        // 大于等于当前时间最靠后的后续活动时间，则更新累计游泳时间和当前时间最靠后的后续活动时间
        if(p.first+p.second+temp>=longest){
            // 更新前面的累计游泳时间
            need+=(temp+p.second);
            // 更新当前时间最靠后的后续活动时间
            longest=p.first;
            temp=0;  // 重置临时累积
        } else {
            // 临时累积游泳时间
            temp+=p.second;
        }
    }

    // 总时间 = 当前时间最靠后的后续活动时间 + 前面的累计游泳时间
    return longest+need;
}

/**
 * 主函数：程序入口点
 * 输入格式：
 * 第一行：队员数量n
 * 接下来n行：每行三个整数:游泳时间,自行车时间,长跑时间
 * 
 * 输出：完成所有训练的最短时间
 */
int main(){
    int n;
    cin >> n;
    
    // 读取队员数据并构建优先队列
    priority_queue<pair<int,int>> A=gainGrid(n);
    
    // 使用贪心算法计算最小总时间
    int result=solve(A,n);
    
    // 输出结果
    cout << result << endl;
}