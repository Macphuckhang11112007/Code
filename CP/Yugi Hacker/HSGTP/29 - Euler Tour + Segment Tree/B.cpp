#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 2e5;

int         n, m, q, timer, a[N + 5], euler[2 * N + 5], tin[N + 5], tout[N + 5];
ll          fen[2 * N + 5];
vector<int> adj[N + 5];

void update(int i, ll x) {
    for (; i <= n; i += i & -i) { fen[i] += x; }
}

ll get(int i) {
    ll res = 0;
    for (; i; i -= i & -i) { res += fen[i]; }
    return res;
}

void rangeUpdate(int l, int r, ll x) {
    update(l, x);
    update(r + 1, -x);
}

void dfs(int u, int p) {
    tin[u]       = ++timer;
    euler[timer] = u;
    for (int v : adj[u]) {
        if (v != p) { dfs(v, u); }
    }
    tout[u] = timer;
}

int main() {
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
    m = timer;
    for (int i = 1; i <= n; i++) {
        fen[tin[i]] += a[i];
        fen[tout[i] + 1] += -a[i];
    }
    for (int i = 1; i <= m; i++) {
        int j = i + (i & -i);
        if (j <= m) { fen[j] += fen[i]; }
    }
    while (q--) {
        int flag;
        cin >> flag;
        if (flag == 1) {
            int u;
            ll  x, tmp;
            cin >> u >> x;
            tmp = x;
            x -= a[u];
            a[u] = tmp;
            rangeUpdate(tin[u], tout[u], x);
        } else {
            int u;
            cin >> u;
            cout << get(tin[u]) << "\n";
        }
    }
}
