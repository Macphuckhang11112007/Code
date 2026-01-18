#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

using ll   = long long;
using pii  = pair<ll, ll>;
using tiii = tuple<ll, ll, ll>;

const int N = 4000, LG = 20;

int          n, m, q, par[N + 5], sz[N + 5], lg[N + 5], h[N + 5];
ll           ans, dist[N + 5][N + 5];
pii          up[N + 5][LG + 5];
vector<pii>  g[N + 5];
vector<tiii> edges;

int find_par(int u) {
    return (u == par[u] ? u : par[u] = find_par(par[u]));
}

bool dsu(int u, int v) {
    int pu = find_par(u), pv = find_par(v);
    if (pu == pv) { return 0; }
    if (sz[pu] < sz[pv]) { swap(pu, pv); }
    par[pv] = pu;
    sz[pu] += sz[pv];
    return 1;
}

void dfs_lca(int u, int p) {
    h[u] = h[p] + 1;
    for (auto &[v, w] : g[u]) {
        if (v != p) {
            up[v][0] = {u, w};
            for (int i = 1; i <= lg[n]; i++) {
                up[v][i].fi = up[up[v][i - 1].fi][i - 1].fi;
                up[v][i].se = max(up[v][i - 1].se, up[up[v][i - 1].fi][i - 1].se);
            }
            dfs_lca(v, u);
        }
    }
}

pii lca(int u, int v) {
    pii res;
    if (u == v) { return {u, 0}; }
    if (h[u] < h[v]) { swap(u, v); }
    for (int i = lg[h[u] - h[v] + 1]; h[u] != h[v] && i >= 0; i--) {
        if (h[up[u][i].fi] >= h[v]) {
            res.se = max(res.se, up[u][i].se);
            u      = up[u][i].fi;
        }
    }
    res.fi = u;
    if (u == v) { return res; }
    for (int i = lg[h[u]]; up[u][0].fi != up[v][0].fi && i >= 0; i--) {
        if (up[u][i].fi != up[v][i].fi) {
            res.se = max({res.se, up[u][i].se, up[v][i].se});
            u      = up[u][i].fi;
            v      = up[v][i].fi;
        }
    }
    res.fi = up[u][0].fi;
    res.se = max({res.se, up[u][0].se, up[v][0].se});
    return res;
}

void dfs(int u, int p, int hv) {
    for (auto &[v, w] : g[u]) {
        if (v != p) {
            dist[hv][v] = max(dist[hv][u], w);
            dfs(v, u, hv);
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        par[i] = i;
        sz[i]  = 1;
    }
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }
    sort(edges.begin(), edges.end());
    for (auto &[w, u, v] : edges) {
        if (dsu(u, v)) {
            ans += w;
            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }
    }
    lg[1] = 0;
    for (int i = 2; i <= n; i++) { lg[i] = lg[i >> 1] + 1; }
    dfs_lca(1, 0);
    // for (int i = 1; i <= n; i++) { dfs(i, 0, i); }
    cin >> q;
    for (int i = 1; i <= q; i++) {
        int x, y;
        cin >> x >> y;
        // cout << ans - dist[x][y] << "\n";
        pii res = lca(x, y);
        cout << ans - res.se << "\n";
    }
}
