#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll  BASE = 311, MOD = (1LL << 61) - 1;
const int N = 1e6;
int       n, m, ans;
string    s, t;
ll        h[N + 2], p[N + 2] = {1}, g;

ll mul(ll x, ll y) {
    return (__int128_t)x * y % MOD;
}

ll getHash(int l, int r) {
    return (h[r] - mul(h[l], p[r - l]) + MOD) % MOD;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> s >> t;
    n = s.size();
    m = t.size();
    s = " " + s;
    t = " " + t;

    for (int i = 1; i <= max(n, m); i++) {
        p[i] = mul(p[i - 1], BASE);
    }
    for (int i = 1; i <= n; i++) {
        (h[i] = mul(h[i - 1], BASE) + s[i] - 'A' + 1) %= MOD;
    }
    for (int i = 1; i <= m; i++) {
        (g = mul(g, BASE) + t[i] - 'A' + 1) %= MOD;
    }
    for (int i = m; i <= n; i++) {
        if (getHash(i - m, i) == g) {
            ans++;
            // cout << i - m + 1 << " ";
        }
    }
    cout << ans;
    return 0;
}