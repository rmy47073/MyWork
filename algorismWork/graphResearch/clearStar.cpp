#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

/*上一次提交答案错误的原因是countSame应该初始化为1，我误初始化为0了，起点也要算进去*/
vector<vector<int>> gainSize(){//获得尺寸和数字
    int m,n;
    cin >> m;
    cin >> n;
    vector<vector<int>> grid(m,vector<int>(n));
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cin >> grid[i][j];
        }
    }
    return grid;
}
int countMax(vector<vector<int>>& grid){
    queue<vector<int>> q;
    vector<int> maxCount;

    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[i].size();j++){
           if(grid[i][j]!=0){//没有被访问的数字，就是开始了新一轮的统计
              int countSame=1;//一定初始化1，起点也要算进去
              int c=grid[i][j];//保存起点的数字，以便后续数字的比较
              q.push({i,j});
              grid[i][j]=0;//相当于访问标记

              while(!q.empty()){//常规广度优先遍历操作
                  vector<int> a = q.front();
                  q.pop();
                  int x=a[0];
                  int y=a[1];
                  if(x-1>=0 && grid[x-1][y]==c){
                      q.push({x-1,y});
                      grid[x-1][y]=0;//访问标记，下同
                      countSame++;//统计相同数字的个数，下同
                  }
                  if(x+1<grid.size() && grid[x+1][y]==c){
                      q.push({x+1,y});
                      grid[x+1][y]=0;
                      countSame++;
                  }
                  if(y-1>=0 && grid[x][y-1]==c){
                      q.push({x,y-1});
                      grid[x][y-1]=0;
                      countSame++;
                  }
                  if(y+1<grid[x].size() && grid[x][y+1]==c){
                      q.push({x,y+1});
                      grid[x][y+1]=0;
                      countSame++;
                  }
                  maxCount.push_back(c*c*countSame);//将每一轮相邻数字的统计结果保存在一维动态数组里
              }
            }
        }
    }
    // 对maxCount排序，返回最大的得分
    sort(maxCount.begin(),maxCount.end());
    return maxCount[maxCount.size()-1];

}

int main(){
    vector<vector<int>> grid=gainSize();
    cout << countMax(grid) << endl;
}


        
    

