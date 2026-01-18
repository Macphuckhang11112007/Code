#include <bits/stdc++.h>

using namespace std;

const int N = 1e4;

int q, par[N + 5], sz[N + 5];

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
    for (int u = 1; u <= N; u++) {
        par[u] = u;
        sz[u]  = 1;
    }
    cin >> q;
    while (q--) {
        int u, v;
        int type;
        cin >> u >> v >> type;
        if (type == 1) {
            dsu(u, v);
        } else {
            cout << (find_par(u) == find_par(v)) << "\n";
        }
    }
}
