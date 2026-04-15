#include<iostream>
#include<stack>
using namespace std;
#include<vector>

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
int countPath(vector<vector<int>> grid){//二维数组传入
//栈里存元素的位置x，y和已经尝试过的次数
    stack<vector<int>> q;
    int count=0;

    grid[0][0]=0;
    q.push({0,0,0});//为1则入栈，这是深度优先遍历起点
    while(!q.empty()){//以下就是深度优先遍历的常规操作
    vector<int> a = q.top();
    q.pop();       
          
    int x=a[0];
    int y=a[1];
    int step=a[2];

    if(x==grid.size()-1 && y==grid[x].size()-1){
        grid[x][y]=1;
        count++;
        continue;//到达终点，计数加1
    }
    if(step==4){
        grid[x][y]=1;
        continue;
    }
    q.push({x,y,step+1}); 
    if(step==0){
        if(y-1>=0 && grid[x][y-1]==1 ){//左边的
        q.push({x,y-1,0});
        grid[x][y-1]=0;//标记已访问
        }     
    }
    if(step==1){
        if(x-1>=0 && grid[x-1][y]==1 ){//上边的
        q.push({x-1,y,0});
        grid[x-1][y]=0;
        }    
    }
    if(step==2){
        if(y+1<grid[x].size() && grid[x][y+1]==1 ){//右边的
        q.push({x,y+1,0}); 
        grid[x][y+1]=0;
        }
    }
    if(step==3){
        if(x+1<grid.size() && grid[x+1][y]==1 ){//下边的
        q.push({x+1,y,0}); 
        grid[x+1][y]=0;
        }
    }
}

    return count;
}



int main(){
    vector<vector<int>> grid=gainSize();
    int count=countPath(grid);
    cout << count << endl;
    return 0;
}





    
