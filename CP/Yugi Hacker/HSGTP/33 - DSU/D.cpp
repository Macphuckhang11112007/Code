#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e5;

int           n, m, cnt, par[N + 5], sz[N + 5];
ll            ans;
bitset<N + 5> vis;
vector<int>   adj[N + 5];

int find_par(int u) {
    return (u == par[u] ? u : par[u] = find_par(par[u]));
}

void dsu(int u, int v) {
    int pu = find_par(u), pv = find_par(v);
    if (pu == pv) { return; }
    if (sz[pu] < sz[pv]) { swap(pu, pv); }
    par[pv] = pu;
    sz[pu] += pv;
}

void dfs(int u) {
    vis[u] = 1;
    for (int v : adj[u]) {
        if (!vis[v]) { dfs(v); }
    }
}

int main() {
    if (fopen("ads.inp", "r")) {
        freopen("ads.inp", "r", stdin);
        freopen("ads.out", "w", stdout);
    }
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    // for (int u = 1; u <= n; u++) {
    //     par[u] = u;
    //     sz[u]  = 1;
    // }
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        // ans += (find_par(u) == find_par(v));
        // dsu(u, v);
    }
    ans = m - n;
    for (int u = 1; u <= n; u++) {
        if (!vis[u]) {
            ++ans;
            dfs(u);
        }
    }
    cout << ans;
}
