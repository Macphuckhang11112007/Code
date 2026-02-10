#include <bits/stdc++.h>

using namespace std;

int n, s;
long long ans;
map<int, int> mp;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> s;
    for (int i = 1; i <= n; i++) {
        int x; cin >> x;
        ans += mp[s - x];
        mp[x]++;
    }
    cout << ans << "\n";
}
