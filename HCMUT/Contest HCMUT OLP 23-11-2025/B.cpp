#include <bits/stdc++.h>

#define int long long

using namespace std;

const int N = 1e6;

int tc, n, a[N + 1], ans = LLONG_MAX;

int solve() {
    int                  res = 1e18, val = 0;
    vector<int>          b(n + 1), id(N + 1), maxx(n + 1, -1e18), minn(n + 1, 1e18);
    map<int, queue<int>> mp;
    for (int i = 1; i <= n; i++) {
        b[i] = a[i];
    }
    sort(b.begin() + 1, b.end());
    for (int i = 1; i <= n; i++) {
        mp[b[i]].push(i);
    }
    for (int i = 1; i <= n; i++) {
        id[i] = mp[a[i]].front();
        mp[a[i]].pop();
    }
    for (int i = 1; i <= n; i++) {
        maxx[i] = maxx[i - 1];
        if (id[i] <= i) {
            maxx[i] = max(maxx[i], a[i]);
        }
    }
    for (int i = n; i >= 1; i--) {
        minn[i] = minn[i - 1];
        if (id[i] <= i) {
            minn[i] = min(minn[i], a[i]);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (id[i] <= i) {
            sett.insert(a[i]);
        }
    }
    for (int i = 1; i <= n; i++) {
        if (id[i] <= i) {
            
        }
    }
        int cnt = 0;
    for (int i = 0; i <= n + 1; i++) {
        val = i * i;
        cnt += (id[i] > i);
        val += pow(n - i + cnt, 2);
        if (maxx[i] > minn[i + 1]) {}
        res = min(res, val);
    }
    return res;
}

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        ans = 1e18;
        cin >> n;
        for (int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        ans = min(ans, solve());
        reverse(a + 1, a + n + 1);
        ans = min(ans, solve());
        cout << ans << "\n";
    }
}