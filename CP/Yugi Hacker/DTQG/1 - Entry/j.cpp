#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 1e5, H = 17;

int         n, q, cnt, a[N + 5], in[N + 5], out[N + 5], par[N + 5][H + 5], h[N + 5];
vector<int> adj[N + 5];
ll          d[N + 5];
ll          tree[4 * N + 5], lazy[4 * N + 5];

void dfs(int u, int p) {
    in[u]     = ++cnt;
    par[u][0] = p;
    h[u]      = h[p] + 1;
    for (int i = 1; i <= H; i++) {
        par[u][i] = par[par[u][i - 1]][i - 1];
    }
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
        }
    }
    out[u] = cnt;
}

int lca(int u, int v) {
    if (h[u] < h[v]) {
        swap(u, v);
    }
    for (int k = h[u] - h[v]; k; k -= k & -k) {
        u = par[u][__builtin_ctz(k & -k)];
    }
    if (u == v) {
        return u;
    }
    for (int i = H; i >= 0; i--) {
        if (par[u][i] != par[v][i]) {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}

void pointUpdate(int i, ll x) {
    for (; i <= n; i += i & -i) {
        tree[i] += x;
    }
}

void rangeUpdate(int l, int r, ll x) {
    pointUpdate(l, x);
    pointUpdate(r + 1, -x);
}

ll pointQuery(int i) {
    ll res = 0;
    for (; i; i -= i & -i) {
        res += tree[i];
    }
    return res;
}

int main() {
    freopen("maze.inp", "r", stdin);
    freopen("maze.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        a[i] = abs(a[i]);
    }
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++) {
        rangeUpdate(in[i], out[i], a[i]);
    }
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int u;
            ll  x;
            cin >> u >> x;
            x = abs(x);
            rangeUpdate(in[u], out[u], x - a[u]);
            a[u] = x;
        } else {
            int u, v;
            cin >> u >> v;
            int k   = lca(u, v);
            ll  res = 2 * (pointQuery(in[u]) + pointQuery(in[v]) - 2 * pointQuery(in[k]) + a[k]) - a[u] - a[v];
            cout << res << "\n";
        }
    }
}
