#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

using pii = pair<int, int>;
using ll  = long long;

const int N = 2e5;

int         n, q, par[N + 5], sz[N + 5];
ll          ans;
vector<pii> qr;
vector<ll>  qr_ans;

int find_par(int u) {
    return (u == par[u] ? u : par[u] = find_par(par[u]));
}

void dsu(int u, int v) {
    int pu = find_par(u), pv = find_par(v);
    if (pu == pv) { return; }
    if (sz[pu] < sz[pv]) { swap(pu, pv); }
    ans += 1LL * sz[pu] * sz[pu];
    ans += 1LL * sz[pv] * sz[pv];
    par[pv] = pu;
    sz[pu] += sz[pv];
    ans -= 1LL * sz[pu] * sz[pu];
}

int main() {
    if (fopen("phacau.inp", "r")) {
        freopen("phacau.inp", "r", stdin);
        freopen("phacau.out", "w", stdout);
    }
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        par[i] = i;
        sz[i]  = 1;
    }
    for (int i = 1; i <= q; i++) {
        int u, v;
        cin >> u >> v;
        qr.push_back({u, v});
    }
    reverse(qr.begin(), qr.end());
    ans = 1LL * n * (n - 1);
    for (auto &[u, v] : qr) {
        qr_ans.push_back(ans);
        dsu(u, v);
    }
    reverse(qr_ans.begin(), qr_ans.end());
    for (auto &x : qr_ans) { cout << x / 2 << "\n"; }
}
