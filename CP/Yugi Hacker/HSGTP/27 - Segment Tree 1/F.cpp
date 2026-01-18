#include <bits/stdc++.h>

using namespace std;

#define fi first
#define se second

using ll  = long long;
using pll = pair<ll, ll>;

const int N = 1e5;

int n, q, a[N + 5];
pll seg[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    if (l == r) {
        seg[p] = {a[m], (a[m] ? m : 0)};
    } else {
        int u = 2 * p, v = 2 * p + 1;
        build(u, l, m);
        build(v, m + 1, r);
        seg[p] = {seg[u].fi + seg[v].fi, max(seg[u].se, seg[v].se)};
    }
}

void update(int p, int l, int r, int i) {
    if (l == r) {
        seg[p].fi += (a[i] ^ 1) - a[i];
        seg[p].se = (a[i] ? 0 : i);
        a[i] ^= 1;
    } else {
        int m = (l + r) / 2;
        int u = 2 * p, v = 2 * p + 1;
        if (i <= m) {
            update(u, l, m, i);
        }
        if (m < i) {
            update(v, m + 1, r, i);
        }
        seg[p] = {seg[u].fi + seg[v].fi, max(seg[u].se, seg[v].se)};
    }
}

ll get(int p, int l, int r, int i) {
    if (seg[p].fi == i) {
        return seg[p].se;
    } else {
        int m = (l + r) / 2;
        int u = 2 * p, v = 2 * p + 1;
        ll  res = 0;
        if (i <= seg[u].fi) {
            res = get(u, l, m, i);
        } else {
            res = get(v, m + 1, r, i - seg[u].fi);
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
            cin >> i;
            update(1, 1, n, i);
        } else {
            int i;
            cin >> i;
            cout << get(1, 1, n, i) << "\n";
        }
    }
}
