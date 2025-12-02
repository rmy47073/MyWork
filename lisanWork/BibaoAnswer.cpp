#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> printRelationMatrix(int n){
    vector<vector<int>> relationMatrix(n, vector<int>(n));

    cout << "请输入关系矩阵(" << n << "x" << n << "):" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> relationMatrix[i][j];
        }
    }
    cout << "关系矩阵为:" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << relationMatrix[i][j] << " ";
        }
        cout << endl;
    }
    return relationMatrix;
}
//判断自反性   
bool isReflexive(vector<vector<int>> relationMatrix, int n){
    for(int i=0; i<n; i++){
        if(relationMatrix[i][i] != 1){
                cout << "关系矩阵不是自反的" << endl;
                return false;
        }
    }
    cout << "关系矩阵是自反的" << endl;
    return true;
}
//判断对称性
bool isSymmetric(vector<vector<int>> relationMatrix, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(relationMatrix[i][j] != relationMatrix[j][i]){
                cout << "关系矩阵不是对称的" << endl;
                return false;
            }
        }
    }
    cout << "关系矩阵是对称的" << endl;
    return true;
}
//判断传递性
bool isTransitive(vector<vector<int>> relationMatrix, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(relationMatrix[i][j] == 1){
                for(int k=0; k<n; k++){
                    if(relationMatrix[j][k] == 1 && relationMatrix[i][k] != 1){
                        cout << "关系矩阵不是传递的" << endl;
                        return false;
                    }
                }
            }
        }
    }
    cout << "关系矩阵是传递的" << endl;
    return true;
}
//求自反闭包
void reflexiveClosure(vector<vector<int>> relationMatrix, int n){
    for(int i=0; i<n; i++){
        relationMatrix[i][i] = 1;
    }
    cout << "自反闭包为:" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << relationMatrix[i][j] << " ";
        }
        cout << endl;
    }
    return ;
}
//求对称闭包
void symmetricClosure(vector<vector<int>> relationMatrix, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(relationMatrix[i][j] != relationMatrix[j][i]){
                relationMatrix[j][i] = 1;//用沃舍尔算法求传递闭包
            }
        }
    }
    cout << "对称闭包为:" << endl;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << relationMatrix[i][j] << " ";
        }
        cout << endl;
    }
    return ;
}

//求传递闭包
void transitiveClosure(vector<vector<int>> relationMatrix, int n){
    for(int k=0; k<n; k++){
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                relationMatrix[i][j] = relationMatrix[i][j] || (relationMatrix[i][k] && relationMatrix[k][j]);
            }
        }
    }
    cout << "传递闭包为:" << endl;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << relationMatrix[i][j] << " ";
        }
        cout << endl;
    }
    return ;
}        
            
int main(){
    cout << "请输入集合A的元素个数n: " << endl;
    int n;
    cin >> n;
  //输出结果
    vector<vector<int>> relationMatrix =  printRelationMatrix(n);
    cout << "自反性" << (isReflexive(relationMatrix, n)? "是" : "否") << endl;
    cout << "对称性" << (isSymmetric(relationMatrix, n)? "是" : "否") << endl;
    cout << "传递性" << (isTransitive(relationMatrix, n)? "是" : "否") << endl;
    reflexiveClosure(relationMatrix, n);
    symmetricClosure(relationMatrix, n);
    transitiveClosure(relationMatrix, n);
  
    return 0;
}


