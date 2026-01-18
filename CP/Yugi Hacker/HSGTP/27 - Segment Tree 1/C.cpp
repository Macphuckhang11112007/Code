#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 2e5;

int n, q, a[N + 5];

struct segTree {
    ll sum      = 0;
    ll sumLeft  = 0;
    ll sumRight = 0;
    ll sumMax   = 0;
} seg[4 * N + 5];

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    if (l == r) {
        seg[p].sum      = a[m];
        seg[p].sumLeft  = max(0LL, ll(a[m]));
        seg[p].sumRight = max(0LL, ll(a[m]));
        seg[p].sumMax   = max(0LL, ll(a[m]));
    } else {
        int u = 2 * p, v = 2 * p + 1;
        build(u, l, m);
        build(v, m + 1, r);
        seg[p].sum      = seg[u].sum + seg[v].sum;
        seg[p].sumLeft  = max(seg[u].sumLeft, seg[u].sum + seg[v].sumLeft);
        seg[p].sumRight = max(seg[v].sumRight, seg[v].sum + seg[u].sumRight);
        seg[p].sumMax   = max({seg[u].sumMax, seg[v].sumMax, seg[u].sumRight + seg[v].sumLeft});
    }
}

void update(int p, int l, int r, int i, ll x) {
    int m = (l + r) / 2;
    if (l == r) {
        seg[p].sum      = x;
        seg[p].sumLeft  = max(0LL, ll(x));
        seg[p].sumRight = max(0LL, ll(x));
        seg[p].sumMax   = max(0LL, ll(x));
    } else {
        int u = 2 * p, v = 2 * p + 1;
        if (i <= m) {
            update(u, l, m, i, x);
        }
        if (m < i) {
            update(v, m + 1, r, i, x);
        }
        seg[p].sum      = seg[u].sum + seg[v].sum;
        seg[p].sumLeft  = max(seg[u].sumLeft, seg[u].sum + seg[v].sumLeft);
        seg[p].sumRight = max(seg[v].sumRight, seg[v].sum + seg[u].sumRight);
        seg[p].sumMax   = max({seg[u].sumMax, seg[v].sumMax, seg[u].sumRight + seg[v].sumLeft});
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
        int i;
        ll  x;
        cin >> i >> x;
        update(1, 1, n, i, x);
        cout << seg[1].sumMax << "\n";
    }
}
