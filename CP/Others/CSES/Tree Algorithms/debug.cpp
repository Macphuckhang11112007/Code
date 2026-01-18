#include <bits/stdc++.h>

using namespace std;

const int N = 2e5;

int         n, q, cnt, node[N + 5], sz[N + 5], h[N + 5], pos[N + 5], par[N + 5], head[N + 5], heavy[N + 5], a[N + 5], seg[4 * N + 5];
vector<int> adj[N + 5];

void dfs(int u, int p) {
    sz[u] = 1;
    h[u] = h[p] + 1;
    for (int v : adj[u]) {
        if (v == p) { continue; }
        dfs(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[heavy[u]]) { heavy[u] = v; }
    }
}

void hld(int u, int p, int hv) {
    pos[u]  = ++cnt;
    node[cnt] = u;
    par[u]  = p;
    head[u] = hv;
    if (heavy[u]) { hld(heavy[u], u, hv); }
    for (int v : adj[u]) {
        if (v == p || v == heavy[u]) { continue; }
        hld(v, u, v);
    }
}

void build(int p, int l, int r) {
    int m = (l + r) / 2;
    int u = 2 * p, v = u + 1;
    if (l == r) {
        seg[p] = a[node[m]];
        return;
    }
    build(u, l, m);
    build(v, m + 1, r);
    seg[p] = max(seg[u], seg[v]);
}

int query(int p, int l, int r, int i, int j) {
    int m = (l + r) / 2;
    int u = 2 * p, v = u + 1;
    if (j < l || r < i) { return 0; }
    if (i <= l && r <= j) { return seg[p]; }
    return max(query(u, l, m, i, j), query(v, m + 1, r, i, j));
}

void update(int p, int l, int r, int i, int x) {
    int m = (l + r) / 2;
    int u = 2 * p, v = u + 1;
    if (i < l || r < i) { return; }
    if (l == r) {
        seg[p] = x;
        return;
    }
    update(u, l, m, i, x);
    update(v, m + 1, r, i, x);
    seg[p] = max(seg[u], seg[v]);
}

int get(int u, int v) {
    int hu = head[u], hv = head[v];
    if (h[hu] < h[hv] || (hu == hv && h[u] > h[v])) { swap(u, v); }
    if (hu == hv) { return query(1, 1, n, pos[u], pos[v]); }
    return max(query(1, 1, n, pos[hu], pos[u]), get(par[hu], v));
}

int main() {
    // Hardcoded input for reproduction
    string input = R"(10 10
9 2 1 1 1 4 2 10 7 4
2 1
3 2
4 2
5 4
6 5
7 6
8 4
9 8
10 2
2 5 4
1 10 4
1 5 9
2 5 4
2 9 5
2 1 10
2 1 6
1 8 4
1 3 5
2 6 1)";
    stringstream ss(input);

    ss >> n >> q;
    for (int i = 1; i <= n; i++) ss >> a[i];
    for (int i = 1; i < n; i++) {
        int u, v;
        ss >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    hld(1, 0, 1);
    build(1, 1, n);
    while (q--) {
        int flag;
        ss >> flag;
        if (flag == 1) {
            int u, x;
            ss >> u >> x;
            update(1, 1, n, pos[u], x);
        } else {
            int u, v;
            ss >> u >> v;
            cout << get(u, v) << " ";
        }
    }
}
