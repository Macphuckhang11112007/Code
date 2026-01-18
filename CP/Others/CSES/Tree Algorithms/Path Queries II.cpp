#include <bits/stdc++.h>

using namespace std;

const int N = 2e5;

int         n, m, q, cnt, height;
int         sz[N + 5], h[N + 5], pos[N + 5], par[N + 5], head[N + 5], heavy[N + 5], a[N + 5];
int         tree[4 * N + 5], has_lz[4 * N + 5], lz[4 * N + 5];
vector<int> adj[N + 5];

void dfs(int u, int p) {
    sz[u] = 1;
    h[u]  = h[p] + 1;
    for (int v : adj[u]) {
        if (v == p) { continue; }
        dfs(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[heavy[u]]) { heavy[u] = v; }
    }
}

void hld(int u, int p, int hv) {
    pos[u]  = ++cnt;
    par[u]  = p;
    head[u] = hv;
    if (heavy[u]) { hld(heavy[u], u, hv); }
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) { hld(v, u, v); }
    }
}

void build() {
    height = 32 - __builtin_clz(n);
    m      = 1 << height;
    for (int i = 1; i <= n; i++) { tree[pos[i] + m - 1] = a[i]; }
    for (int i = m - 1; i; i--) { tree[i] = max(tree[i << 1], tree[i << 1 | 1]); }
}

void apply(int p, int x) {
    tree[p] = x;
    if (p < m) {
        lz[p]     = x;
        has_lz[p] = 1;
    }
}

void pull(int p) {
    for (p >>= 1; p; p >>= 1) {
        if (has_lz[p]) {
            tree[p] = lz[p];
        } else {
            tree[p] = max(tree[p << 1], tree[p << 1 | 1]);
        }
    }
}

void push(int p) {
    int p0 = p;
    for (int j = height; j; j--) {
        p = p0 >> j;
        if (!p) { continue; }
        if (has_lz[p]) {
            apply(p << 1, lz[p]);
            apply(p << 1 | 1, lz[p]);
            has_lz[p] = 0;
            lz[p]     = 0;
        }
    }
}

void update(int l, int r, int x) {
    l += m - 1;
    r += m - 1;
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

int query(int l, int r) {
    l += m - 1;
    r += m - 1;
    push(l);
    push(r);
    int res = 0;
    for (; l <= r; l >>= 1, r >>= 1) {
        if (l & 1) { res = max(res, tree[l++]); }
        if (~r & 1) { res = max(res, tree[r--]); }
    }
    return res;
}

int get(int u, int v) {
    int res = 0;
    while (head[u] != head[v]) {
        if (h[head[u]] < h[head[v]]) swap(u, v);
        res = max(res, query(pos[head[u]], pos[u]));
        u   = par[head[u]];
    }
    if (h[u] > h[v]) swap(u, v);
    res = max(res, query(pos[u], pos[v]));
    return res;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);
    hld(1, 0, 1);
    build();

    while (q--) {
        int flag;
        cin >> flag;
        if (flag == 1) {
            int u, x;
            cin >> u >> x;
            update(pos[u], pos[u], x);
        } else {
            int u, v;
            cin >> u >> v;
            cout << get(u, v) << " ";
        }
    }
}
