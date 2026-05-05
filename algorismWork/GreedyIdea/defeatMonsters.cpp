#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> gainGrid(int n){
    vector<vector<int>> A(n,vector<int>(2));
    for(int i=0;i<n;i++)
        cin >> A[i][0] >> A[i][1];
    return A;
}
//读取输入数据
long long solve(vector<vector<int>>& A){

    long long result=0;
    //依次选择最优的怪物，直到所有怪物都被击败
    while(!A.empty()){
        int bestIndex=0;
        //for循环实现找到当前最优的怪物
        for(int j=1;j<A.size();j++){
            auto& cur=A[j];
            auto& best=A[bestIndex];
            //当前最优的怪物依次和A中所有怪物相比，找到最击杀成本最少的怪物
            if(cur[0]*best[1]<best[0]*cur[1]){
                bestIndex=j;
            }
        }
        long long m = 0;
        for (auto& rest : A) m += rest[1];//计算剩余所有怪兽的总破坏力
        // 累加本次击杀造成的总破坏：时间 × 其他怪兽破坏力之和
        result += A[bestIndex][0] * (m - A[bestIndex][1]);
        A.erase(A.begin()+bestIndex);  // 从A中移除已击杀的怪兽
    }
    return result;
}

int main(){
    int n;
    cin >> n;
    vector<vector<int>> A=gainGrid(n);
    long long result=solve(A);
    cout << result << endl;
}
    