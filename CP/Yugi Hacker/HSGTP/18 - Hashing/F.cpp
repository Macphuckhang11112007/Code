#include <bits/stdc++.h>

using namespace std;

using ll  = long long;
using lll = __int128_t;

const ll  BASE = stoll(string(19, '1')), MOD = (1ULL << 61) - 1;
const int N = 5e5;

int    n;
ll     ans;
ll     h[N + 5], rh[N + 5], p[N + 5] = {1};
string s;

ll mul(ll x, ll y) {
    lll res = (lll)x * y;
    res     = (res >> 61) + (res & MOD);
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

ll add(ll x, ll y) {
    ll res = x + y;
    if (res < 0) {
        res += MOD;
    }
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

ll getHash(int l, int r) {
    return add(h[r], -mul(h[l - 1], p[r - l + 1]));
}

ll getRHash(int l, int r) {
    return add(rh[l], -mul(rh[r + 1], p[r - l + 1]));
}

int main() {
    freopen("CntPalin.inp", "r", stdin);
    freopen("CntPalin.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> s;
    n = s.size();
    s = " " + s;
    for (int i = 1; i <= n; i++) {
        h[i] = add(mul(h[i - 1], BASE), s[i]);
        p[i] = mul(p[i - 1], BASE);
    }
    for (int i = n; i; i--) {
        rh[i] = add(mul(rh[i + 1], BASE), s[i]);
    }
    for (int i = 1; i <= n; i++) {
        int res = 0;
        int low = 1, high = n;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (i - mid + 1 >= 1 && i + mid - 1 <= n && getHash(i - mid + 1, i) == getRHash(i, i + mid - 1)) {
                res = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        ans += res;
        res = 0;
        low = 1, high = n;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (i - mid + 1 >= 1 && i + mid <= n && getHash(i - mid + 1, i) == getRHash(i + 1, i + mid)) {
                res = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        ans += res;
    }
    cout << ans;
}
