#include <bits/stdc++.h>

using namespace std;

#define fi first
#define se second

using ll  = long long;
using pll = pair<ll, ll>;

const int N = 2e5;

int n, q, a[N + 5];
ll  seg[4 * N + 5];
pll lazy[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l == r) {
        seg[p] = a[m];
    } else {
        build(u, l, m);
        build(v, m + 1, r);
        seg[p] = seg[u] + seg[v];
    }
}

void push(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l != r) {
        ll d1 = m - l + 1, d2 = r - m;
        seg[u] += d1 * (2 * lazy[p].fi + (d1 - 1) * lazy[p].se) / 2;
        seg[v] += d2 * (2 * (lazy[p].fi + d1 * lazy[p].se) + (d2 - 1) * lazy[p].se) / 2;
        lazy[u].fi += lazy[p].fi;
        lazy[u].se += lazy[p].se;
        lazy[v].fi += lazy[p].fi + d1 * lazy[p].se;
        lazy[v].se += lazy[p].se;
    }
    lazy[p] = {0, 0};
}

void update(int p, int l, int r, int s, int t) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    push(p, l, r);
    if (s <= l && r <= t) {
        ll d = (r - l + 1);
        seg[p] += d * (2 * (l - s + 1) + d - 1) / 2;
        lazy[p].fi += l - s + 1;
        lazy[p].se++;
    } else {
        if (s <= m) {
            update(u, l, m, s, t);
        }
        if (m < t) {
            update(v, m + 1, r, s, t);
        }
        seg[p] = seg[u] + seg[v];
    }
}

ll get(int p, int l, int r, int s, int t) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    push(p, l, r);
    if (s <= l && r <= t) {
        return seg[p];
    } else {
        ll res = 0;
        if (s <= m) {
            res += get(u, l, m, s, t);
        }
        if (m < t) {
            res += get(v, m + 1, r, s, t);
        }
        return res;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    build(1, 1, n);
    cin >> q;
    while (q--) {
        int flag;
        cin >> flag;
        if (flag == 1) {
            int l, r;
            cin >> l >> r;
            update(1, 1, n, l, r);
        } else {
            int l, r;
            cin >> l >> r;
            cout << get(1, 1, n, l, r) << "\n";
        }
    }
}
