#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e5;

int n, q;
ll  seg[4 * N + 5], lazy[4 * N + 5];

void push(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l != r) {
        seg[u] |= lazy[p];
        seg[v] |= lazy[p];
        lazy[u] |= lazy[p];
        lazy[v] |= lazy[p];
    }
    lazy[p] = 0;
}

void update(int p, int l, int r, int s, int t, ll x) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    push(p, l, r);
    if (s <= l && r <= t) {
        seg[p] |= x;
        lazy[p] |= x;
    } else {
        if (s <= m) {
            update(u, l, m, s, t, x);
        }
        if (m < t) {
            update(v, m + 1, r, s, t, x);
        }
        seg[p] = seg[u] & seg[v];
    }
}

ll get(int p, int l, int r, int s, int t) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    push(p, l, r);
    if (s <= l && r <= t) {
        return seg[p];
    } else {
        ll res = (1 << 30) - 1;
        if (s <= m) {
            res &= get(u, l, m, s, t);
        }
        if (m < t) {
            res &= get(v, m + 1, r, s, t);
        }
        return res;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
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
