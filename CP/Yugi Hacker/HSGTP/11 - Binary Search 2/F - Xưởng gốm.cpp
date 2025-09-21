#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 1e5;

int n, m, t;
ll a[N + 5], b[N + 5];
ll ans;

ll calc(ll x, int sz, int flag) {
    ll res = 0;
    if (x <= 0) {
        return 0;
    }
    for (int i = 1; i <= sz; i++) {
        if (flag == 1) {
            res += x / a[i];
        } else if (flag == 2) {
            res += x / b[i];
        }
    }
    return res;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> b[i];
    }    
    ll l = 1, r = t;
    while (l <= r) {
        ll mid = l + (r - l) / 2;
        ll c1 = calc(mid, n, 1), c2 = calc(t - mid, m, 2);
        if (c1 <= c2) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
        ans = max(ans, min(c1, c2));
        if (c1 == c2) {
            break;
        }
    }
    cout << ans << "\n";
    return 0;
}