#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N   = 1e5;
const ll  MOD = 1e9 + 7;

int n, q;
ll  seg[4 * N + 5], lazy[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l == r) {
        seg[p] = 1;
    } else {
        build(u, l, m);
        build(v, m + 1, r);
        (seg[p] = seg[u] + seg[v]) %= MOD;
    }
    lazy[p] = 1;
}

void push(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l != r) {
        (seg[u] *= lazy[p]) %= MOD;
        (seg[v] *= lazy[p]) %= MOD;
        (lazy[u] *= lazy[p]) %= MOD;
        (lazy[v] *= lazy[p]) %= MOD;
    }
    lazy[p] = 1;
}

void update(int p, int l, int r, int s, int t, ll x) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    push(p, l, r);
    if (s <= l && r <= t) {
        (seg[p] *= x) %= MOD;
        (lazy[p] *= x) %= MOD;
    } else {
        if (s <= m) {
            update(u, l, m, s, t, x);
        }
        if (m < t) {
            update(v, m + 1, r, s, t, x);
        }
        (seg[p] = seg[u] + seg[v]) %= MOD;
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
        return res %= MOD;
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
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
