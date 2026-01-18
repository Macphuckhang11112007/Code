#include <bits/stdc++.h>

using namespace std;

#define fi first
#define se second

using ll  = long long;
using pll = pair<ll, ll>;

const int N = 2e5;

int n, q, a[N + 5];
pll seg[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l == r) {
        seg[p] = {a[m] + m, a[m] - m};
    } else {
        build(u, l, m);
        build(v, m + 1, r);
        seg[p] = {min(seg[u].fi, seg[v].fi), min(seg[u].se, seg[v].se)};
    }
}

void update(int p, int l, int r, int i, ll x) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l == r) {
        seg[p] = {x + m, x - m};
    } else {
        if (i <= m) {
            update(u, l, m, i, x);
        } else {
            update(v, m + 1, r, i, x);
        }
        seg[p] = {min(seg[u].fi, seg[v].fi), min(seg[u].se, seg[v].se)};
    }
}

ll get(int p, int l, int r, int s, int t, int flag) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (s <= l && r <= t) {
        return (!flag ? seg[p].fi : seg[p].se);
    } else {
        ll res = LLONG_MAX / 2;
        if (s <= m) {
            res = min(res, get(u, l, m, s, t, flag));
        }
        if (m < t) {
            res = min(res, get(v, m + 1, r, s, t, flag));
        }
        return res;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    build(1, 1, n);
    while (q--) {
        int flag;
        cin >> flag;
        if (flag == 1) {
            int i;
            ll  x;
            cin >> i >> x;
            update(1, 1, n, i, x);
        } else {
            int i;
            cin >> i;
            cout << min(get(1, 1, n, i, n, 0) - i, get(1, 1, n, 1, i, 1) + i) << "\n";
        }
    }
}
