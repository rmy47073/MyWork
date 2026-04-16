#include<iostream>
#include<vector>
using namespace std;

int findIndex(vector<int>& A,int target){
    int left=0;
    int right=A.size();
    while(left<right){
        int mid=(left+right)/2;
        if(target>A[mid]){// 如果目标值 > 中间值，说明目标在右半区间
            left=mid+1;
        }else{
            right=mid;//否则目标在左半区间，包含 mid，此法可以保证找到第一个大于等于target的元素的索引
        }
    }
    return left; //返回下标
}


vector<int> findQuantity(vector<int>& A,vector<int>& B){
    vector<int> results;
    for(int i=0;i<B.size();i+=2){//使用循环解决不同的问题
         int left=B[i];
         int right=B[i+1];
          // 找到第一个 >= left 的位置
         int index1=findIndex(A,left); 
          // 找到第一个 > right 的位置，等价于找 right+1
         int index2=findIndex(A,right+1);//
         int count=index2-index1;
         results.push_back(count);
    }
    return results;
}

int main(){
    
    int m,n;
    cin >> m;
    cin >> n;
    vector<int> A(m);
    vector<int> B(2*n);
    for(int i=0;i<m;i++){
        cin >>A[i];
    }
    for(int i=0;i<2*n;i++){
        cin >>B[i];
    }
    vector<int> results = findQuantity(A,B);//计算符合条件的数目
    for(int i=0;i<results.size();i++){
        cout << results[i] << endl;
    }
    return 0;
}


