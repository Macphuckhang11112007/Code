#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e5, LG = 20;

int         n, q, hg, a[N + 5];
vector<int> adj[N + 5];
int         sz[N + 5], h[N + 5], pos[N + 5], pos_e[N + 5], e[2 * N + 5], par[N + 5];
ll          dist[N + 5];
int         m_e, cnt, cnt_e;
int         lg[2 * N + 5], st[2 * N + 5][LG + 5];
ll          tree[4 * N + 5], lz[4 * N + 5];
int         node[N + 5], len[4 * N + 5];

void dfs(int u, int p) {
    sz[u]     = 1;
    dist[u]   = dist[p] + abs(a[p]) + abs(a[u]);
    h[u]      = h[p] + 1;
    pos[u]    = ++cnt;
    pos_e[u]  = ++cnt_e;
    e[cnt_e]  = u;
    node[cnt] = u;
    par[u]    = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
            e[++cnt_e] = u;
            sz[u] += sz[v];
        }
    }
}

void build_lca() {
    lg[1] = 0;
    m_e   = cnt_e;
    for (int i = 2; i <= m_e; i++) { lg[i] = lg[i >> 1] + 1; }
    for (int i = 1; i <= m_e; i++) { st[i][0] = e[i]; }
    for (int j = 1; (1 << j) <= m_e; j++) {
        for (int i = 1; i + (1 << j) - 1 <= m_e; i++) { st[i][j] = (h[st[i][j - 1]] < h[st[i + (1 << j - 1)][j - 1]] ? st[i][j - 1] : st[i + (1 << j - 1)][j - 1]); }
    }
}

int query_lca(int u, int v) {
    if (pos_e[u] > pos_e[v]) { swap(u, v); }
    int k = lg[pos_e[v] - pos_e[u] + 1];
    int x = st[pos_e[u]][k], y = st[pos_e[v] - (1 << k) + 1][k];
    return (h[x] < h[y] ? x : y);
}

void build_tree() {
    hg = 32 - __builtin_clz(n);
    for (int i = 1; i <= n; i++) {
        tree[pos[i] + n - 1] = dist[i];
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
    if (l > r) { return; }
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
    if (l > r) { return 0; }
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

ll get(int u, int v) {
    int p = query_lca(u, v);
    return query(pos[u], pos[u]) + query(pos[v], pos[v]) - 2 * query(pos[p], pos[p]);
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
    build_lca();
    build_tree();
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int u;
            ll  x;
            cin >> u >> x;
            ll y = -abs(a[u]) + abs(x);
            update(pos[u], pos[u], y);
            y <<= 1;
            update(pos[u] + 1, pos[u] + sz[u] - 1, y);
            a[u] = x;
        } else {
            int u, v;
            cin >> u >> v;
            cout << get(u, v) << "\n";
        }
    }
}
