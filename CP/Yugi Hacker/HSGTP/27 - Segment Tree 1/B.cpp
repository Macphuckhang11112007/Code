#include <bits/stdc++.h>

using namespace std;

const int N = 2e5;

int n, q, a[N + 5];
int seg[4 * N + 5];

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

void update(int p, int l, int r, int i, int x) {
    int m = (l + r) / 2;
    if (l == r) {
        seg[p] = x;
    } else {
        int u = 2 * p, v = 2 * p + 1;
        if (i <= m) {
            update(u, l, m, i, x);
        }
        if (m < i) {
            update(v, m + 1, r, i, x);
        }
        seg[p] = min(seg[u], seg[v]);
    }
}

int get(int p, int l, int r, int s, int t) {
    int m = (l + r) / 2;
    if (s <= l && r <= t) {
        return seg[p];
    } else {
        int res = INT_MAX / 2;
        int u = 2 * p, v = 2 * p + 1;
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
            int i, x;
            cin >> i >> x;
            update(1, 1, n, i, x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << get(1, 1, n, l, r) << "\n";
        }
    }
}
