#include <iostream>
#include <vector>
using namespace std;


vector<int> gainSize(){//获得棒棒糖的价格序列
    int m;
    cin >> m;
    vector<int> prices(m);
    for(int i=0;i<m;i++){
        cin >> prices[i];
    }
    return prices;
}
/**算法思想
 * 递归求解左半部分的最大利润
 * 递归求解右半部分的最大利润  
 * 跨越中点的最大利润
 */
int solve(vector<int> A, int left, int right){
    // 递归终止条件：区间只有一个元素或为空
    if(left>=right){
        return 0;  // 单天无法买卖，利润为0
    }
    
    // 计算中点，将问题分解为左右两部分
    int mid=(left+right)/2;
    
    // 递归调用，求解左半部分的最大利润
    int leftMax=solve(A,left,mid);
    // 递归求解右半部分的最大利润
    int rightMax=solve(A,mid+1,right);
    
    // 计算左半部分的最小买入价格
    int minPrice=A[left];
    for(int i=left;i<=mid;i++){
        minPrice=min(minPrice,A[i]);
    }
    
    // 计算右半部分的最大卖出价格
    int maxPrice=A[mid];
    for(int i=mid+1;i<=right;i++){
        maxPrice=max(maxPrice,A[i]);
    }
    
    // 计算跨越中点的最大利润
    int crossMax=maxPrice-minPrice;
    
    // 如果跨越中点的利润为负，利润为0
    if(crossMax<0){
        crossMax=0;
    }
    
    // 返回三种情况中的最大值：左半部分、右半部分、跨越中点
    return max(leftMax,max(rightMax,crossMax));
}

 //当最大利润为0时输出"no"，表示无利可图
int main(){
    // 读取棒棒糖的价格
    vector<int> prices=gainSize();
    int max=solve(prices,0,prices.size()-1);
    if(max == 0){
        cout << "no" << endl;  // 利润为0时输出"no"
    } else {
        cout << max << endl;    //输出实际利润值
    }
    
    return 0;
}