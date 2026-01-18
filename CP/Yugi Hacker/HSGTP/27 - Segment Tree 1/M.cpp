#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int    N = 2e5;
int          n, q, a[N + 5];
ll           d[N + 5];
array<ll, 3> seg[4 * N + 5][2][2];

void pull(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l == r) {
        ll x         = abs(d[m]);
        ll sgn       = !!d[m] | (d[m] >> 63);
        seg[p][1][1] = {x, sgn, sgn};
        return;
    }
    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 1; j++) {
            seg[p][i][j] = {0, 0, 0};
            for (int c = 0; c <= 1; c++) {
                for (int d = 0; d <= 1; d++) {
                    ll x = seg[u][i][c][0] + seg[v][d][j][0], y = seg[u][i][c][1], z = seg[v][d][j][2], sgn = seg[u][i][c][2] * seg[v][d][j][1];
                    if (c == 1 && d == 1 && sgn < 0) { continue; }
                    seg[p][i][j] = max(seg[p][i][j], {x, y, z});
                }
            }
        }
    }
}

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (l != r) {
        build(u, l, m);
        build(v, m + 1, r);
    }
    pull(p, l, r);
}

void update(int p, int l, int r, int i, ll x) {
    int m = (l + r) / 2;
    int u = 2 * p, v = 2 * p + 1;
    if (i < l || r < i) { return; }
    if (l == r) {
        ll val = seg[p][1][1][0] * seg[p][1][1][1], sgn = 0;
        val += (i != 1 ? x : 0);
        sgn          = !!val | (val >> 63);
        seg[p][1][1] = {abs(val), sgn, sgn};
        return;
    }
    update(u, l, m, i, x);
    update(v, m + 1, r, i, x);
    pull(p, l, r);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    for (int i = 2; i <= n; i++) { d[i] = a[i] - a[i - 1]; }
    build(1, 1, n);
    while (q--) {
        int l, r;
        ll  x;
        cin >> l >> r >> x;
        update(1, 1, n, l, x);
        update(1, 1, n, r + 1, -x);
        ll ans = 0;
        for (int i = 0; i <= 1; i++) {
            for (int j = 0; j <= 1; j++) { ans = max(ans, seg[1][i][j][0]); }
        }
        cout << ans << "\n";
    }
}
