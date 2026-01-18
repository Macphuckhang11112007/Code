#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e5;

int                          n, m, cnt, par[N + 5], sz[N + 5];
ll                           ans;
vector<tuple<int, int, int>> edges;

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

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int u = 1; u <= n; u++) {
        par[u] = u;
        sz[u]  = 1;
    }
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }
    sort(edges.begin(), edges.end());
    for (auto &[w, u, v] : edges) {
        if (find_par(u) != find_par(v)) {
            ++cnt;
            ans += w;
            dsu(u, v);
        }
        if (cnt == n - 1) { break; }
    }
    cout << (cnt == n - 1 ? to_string(ans) : "IMPOSSIBLE");
}
