#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e5, LG = 20;

int         n, q, hg, cnt, a[N + 5], sz[N + 5], h[N + 5], par[N + 5], heavy[N + 5], pos[N + 5], pos_e[2 * N + 5], lg[2 * N + 5], head[N + 5], len[2 * N + 5];
ll          dist[N + 5], tree[2 * N + 5], lz[2 * N + 5];
vector<int> adj[N + 5];

void dfs(int u, int p) {
    sz[u]   = 1;
    h[u]    = h[p] + 1;
    dist[u] = dist[p] + abs(a[u]) + abs(a[p]);
    par[u]  = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
            sz[u] += sz[v];
            if (sz[v] > sz[heavy[u]]) { heavy[u] = v; }
        }
    }
}

void build_hld(int u, int hv) {
    head[u] = hv;
    pos[u]  = ++cnt;
    if (heavy[u]) { build_hld(heavy[u], hv); }
    for (int v : adj[u]) {
        if (v != par[u] && v != heavy[u]) { build_hld(v, v); }
    }
}

void build_tree() {
    hg = 32 - __builtin_clz(n);
    for (int i = 1; i <= n; i++) {
        tree[pos[i] + n - 1] = abs(a[i]);
        len[pos[i] + n - 1]  = 1;
    }
    for (int i = n - 1; i; i--) {
        tree[i] = tree[i << 1] + tree[i << 1 | 1];
        len[i]  = len[i << 1] + len[i << 1 | 1];
    }
}

void apply(int p, ll x) {
    tree[p] += x * len[p];
    if (p < n) { lz[p] += x; }
}

void push(int p) {
    int p0 = p;
    for (int i = hg; i; i--) {
        p = p0 >> i;
        if (!p) { continue; }
        apply(p << 1, lz[p]);
        apply(p << 1 | 1, lz[p]);
        lz[p] = 0;
    }
}

void pull(int p) {
    for (p >>= 1; p; p >>= 1) { tree[p] = tree[p << 1] + tree[p << 1 | 1] + lz[p] * len[p]; }
}

void update(int l, int r, ll x) {
    l += n - 1;
    r += n - 1;
    int l0 = l, r0 = r;
    push(l0);
    push(r0);
    for (; l <= r; l >>= 1, r >>= 1) {
        if (l & 1) { apply(l++, x); }
        if (~r & 1) { apply(r--, x); }
    }
    pull(l0);
    pull(r0);
}

ll query(int l, int r) {
    ll res = 0;
    l += n - 1;
    r += n - 1;
    push(l);
    push(r);
    for (; l <= r; l >>= 1, r >>= 1) {
        if (l & 1) { res += tree[l++]; }
        if (~r & 1) { res += tree[r--]; }
    }
    return res;
}

ll hld(int u, int v) {
    ll res = 0;
    while (head[u] != head[v]) {
        if (h[head[u]] < h[head[v]]) { swap(u, v); }
        res += query(pos[head[u]], pos[u]);
        u = par[head[u]];
    }
    if (h[u] > h[v]) { swap(u, v); }
    res += query(pos[u], pos[v]);
    return res;
}

int main() {
    freopen("Maze.inp", "r", stdin);
    freopen("Maze.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    build_hld(1, 1);
    build_tree();
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int u;
            ll  x, y;
            cin >> u >> x;
            y = abs(x) - abs(a[u]);
            update(pos[u], pos[u], y);
            a[u] = x;
        } else {
            int u, v;
            cin >> u >> v;
            cout << 2 * hld(u, v) - abs(a[u]) - abs(a[v]) << "\n";
        }
    }
}
