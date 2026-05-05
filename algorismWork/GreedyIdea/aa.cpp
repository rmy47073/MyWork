#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;

bool cmp(pair<int, int> a, pair<int, int> b) {
    return 1LL * a.first * b.second < 1LL * b.first * a.second;
}

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i].first >> v[i].second;
    }
    sort(v.begin(), v.end(), cmp);
    
    ll sum = 0, ans = 0;
    for (auto &p : v) {
        ans += sum * p.second;
        sum += p.first;
    }
    cout << ans << endl;
    return 0;
}