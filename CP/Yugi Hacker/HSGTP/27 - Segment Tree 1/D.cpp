#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 2e5;

int n, q, a[N + 5];
ll  seg[4 * N + 5], lazy[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    if (l == r) {
        seg[p] = a[m];
    } else {
        int u = 2 * p, v = 2 * p + 1;
        build(u, l, m);
        build(v, m + 1, r);
        seg[p] = min(seg[u], seg[v]);
    }
}

void Lazy(int p, int l, int r) {
    int u = 2 * p, v = 2 * p + 1;
    if (l != r) {
        seg[u] += lazy[p];
        seg[v] += lazy[p];
        lazy[u] += lazy[p];
        lazy[v] += lazy[p];
    }
    lazy[p] = 0;
}

void update(int p, int l, int r, int s, int t, ll x) {
    Lazy(p, l, r);
    if (s <= l && r <= t) {
        seg[p] += x;
        lazy[p] += x;
    } else {
        int m = (l + r) / 2;
        int u = 2 * p, v = 2 * p + 1;
        if (s <= m) {
            update(u, l, m, s, t, x);
        }
        if (m < t) {
            update(v, m + 1, r, s, t, x);
        }
        seg[p] = min(seg[u], seg[v]);
    }
}

ll get(int p, int l, int r, int s, int t) {
    Lazy(p, l, r);
    if (s <= l && r <= t) {
        return seg[p];
    } else {
        int m = (l + r) / 2;
        int u = 2 * p, v = 2 * p + 1;
        ll  res = LLONG_MAX / 2;
        if (s <= m) {
            res = min(res, get(u, l, m, s, t));
        }
        if (m < t) {
            res = min(res, get(v, m + 1, r, s, t));
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
            int l, r;
            ll  x;
            cin >> l >> r >> x;
            update(1, 1, n, l, r, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << get(1, 1, n, l, r) << "\n";
        }
    }
}
