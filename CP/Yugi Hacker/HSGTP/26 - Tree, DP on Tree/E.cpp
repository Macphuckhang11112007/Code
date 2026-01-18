#include <bits/stdc++.h>

using namespace std;

#define epb emplace_back

typedef long long ll;

const int N = 2e5;

int           n;
ll            ans, sz[N + 5], dp[N + 5];
vector<int>   adj[N + 5];
bitset<N + 1> vis;

void DFS(int u) {
    ll res = 0;
    vis[u] = 1;
    for (int v : adj[u]) {
        if (!vis[v]) {
            DFS(v);
            res += 1LL * sz[v] * sz[v];
            sz[u] += sz[v];
        }
    }
    dp[u] = (res = 2 * (n - 1) * sz[u] - (sz[u] * sz[u] + res)) /= 2;
    sz[u]++;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].epb(v);
        adj[v].epb(u);
    }
    DFS(1);
    for (int u = 1; u <= n; u++) {
        ans += dp[u];
    }
    ans = 1LL * n * (n - 1) * (n - 2) / 6 - ans;
    cout << ans;
}
