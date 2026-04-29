#include<iostream>
#include<vector>

using namespace std;
//这道题核心在于明白如果是全部人的男神，那么这个男神
//不是左一半的男神就是右一半的男神，
//进而可以用递归的思想
int solve(vector<int>& A,int left,int right){
    if(left==right){
        return A[left];
    }//最小集合，男神既定
    int mid = (left+right)/2;
    int leftMale = solve(A,left,mid);//递归地去左边寻找男神
    int rightMale = solve(A,mid+1,right);//递归地从右边寻找男神
    if(leftMale != -1){
        int count = 0;
        for(int i=left;i<=right;i++){
            if(A[i] == leftMale){
                count++;
            }
        }
    
        if(count > (right-left+1)/2){
            return leftMale;
        }
    }
    if(rightMale != -1){
        int count=0;
        for(int i=left;i<=right;i++){
            if(A[i] == rightMale){
                count++;
            }
        }
    
        if(count > (right-left+1)/2){
            return rightMale;
        }
    }
return -1;
}


int main(){
    int n;
    cin >>n;
    vector<int> A(n);
    for(int i=0;i<n;i++){
        cin >>A[i];
    }
    int allMale=solve(A,0,n-1);
    if(allMale != -1){
        cout << allMale << endl;
    }
    else{
        cout << "None" << endl;
    }
    return 0;
}
    
   

