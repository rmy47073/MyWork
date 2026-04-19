#include <iostream>
#include <vector>
using namespace std;

vector<int> gainSize(){
    int m;
    cin >> m;
    vector<int> prices(m);
    for(int i=0;i<m;i++){
        cin >> prices[i];
    }
    return prices;
}
 int solve(vector<int> A, int left, int right){
    if(left>=right){
        return 0;
    }
    int mid=(left+right)/2;
    int leftMax=solve(A,left,mid);
    int rightMax=solve(A,mid+1,right);
    int minPrice=A[left];
    for(int i=left;i<=mid;i++){
        minPrice=min(minPrice,A[i]);
    }
    int maxPrice=A[mid];
    for(int i=mid+1;i<=right;i++){
        maxPrice=max(maxPrice,A[i]);
    }
    int crossMax=maxPrice-minPrice;
    if(crossMax<0){
        crossMax=0;
    }
    return max(leftMax,max(rightMax,crossMax));
    
 }
 int main(){
    vector<int> prices=gainSize();
    int max=solve(prices,0,prices.size()-1);
    if(max == 0){
        cout << "no" << endl;
    } else {
        cout << max << endl;
    }
    return 0;
 }