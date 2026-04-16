#include<iostream>
#include<queue>
using namespace std;
#include<vector>
/*相较于上一次提交，我取消了visited数组，
直接在grid数组上修改，将1改为0，即可实现访问标记，
缩短了运行时间和内存占用，通过全部测试用例*/
vector<vector<int>> gainSize(){
    int m,n;
    cin >> m;
    cin >> n;
    vector<vector<int>> grid(m,vector<int>(n));
    //用两个for循环获取矩阵grid的每个元素
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cin >> grid[i][j];
        }
    }
    return grid;
}
int countPool(vector<vector<int>>& grid){//二维数组传入

    queue<vector<int>> q;
    int count=0;

   
    for(int i=0;i<grid.size();i++){
        for(int j=0;j<grid[i].size();j++){//外层是两个for循环遍历矩阵grid的每个元素
            if(grid[i][j]==1){//

                grid[i][j]=0;
                q.push({i,j});//为1则入队，这是广度优先遍历起点
                while(!q.empty()){//以下就是广度优先遍历的常规操作
                    vector<int> a = q.front();
                    
                    q.pop();
                    //接下来就是判断a的上下左右是否有1
                    int x=a[0];
                    int y=a[1];
                    if(x-1>=0 && grid[x-1][y]==1 /*&& !visited[(x-1)*grid[x].size()+y]*/){//上边的
                        q.push({x-1,y});
                        grid[x-1][y]=0;//标记访问，避免重复访问，以下都是一样的
                    }
                    if(x+1<grid.size() && grid[x+1][y]==1 /*&& !visited[(x+1)*grid[x].size()+y]*/){//下边的
                        q.push({x+1,y});
                        grid[x+1][y]=0;
                    }
                    if(y-1>=0 && grid[x][y-1]==1 /*&& !visited[x*grid[x].size()+(y-1)]*/){//左边的
                        q.push({x,y-1});
                        grid[x][y-1]=0;
                    }
                    if(y+1<grid[x].size() && grid[x][y+1]==1 /*&& !visited[x*grid[x].size()+(y+1)]*/){//右边的
                        q.push({x,y+1}); 
                        grid[x][y+1]=0;
                    }
                    
                }
            count++;
            } 
        }
    }
    return count;
} 
int main(){
    vector<vector<int>> grid = gainSize();
    int count= countPool(grid);
    cout << count << endl;
    return 0;
}        
   

