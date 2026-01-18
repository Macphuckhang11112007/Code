#include <bits/stdc++.h>

using namespace std;

using ll  = long long;
using lll = __int128_t;

ll          n, ans;
vector<lll> a = {0};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (ll i = 1; i * i <= n; i++) {
        a.push_back(i);
        if (n / i != i) { a.push_back(n / i); }
    }
    sort(a.begin(), a.end());
    for (int i = 1; i < a.size(); i++) { (ans += (lll(1) * (n / a[i]) * (a[i] + a[i - 1] + 1) * (a[i] - a[i - 1]) / lll(2)) % lll(1e9 + 7)) %= lll(1e9 + 7); }
    cout << ans;
}
