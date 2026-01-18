#include <bits/stdc++.h>

using namespace std;

const int N = 1e5;

int                      n, cnt, ok;
int                      id[N + 1], deg[N + 1];
vector<int>              a[N + 1];
map<pair<int, int>, int> vst, f;

int DFS(int u, int par, int root) {
    if (!id[u]) {
        id[u] = ++cnt;
    }
    int  res = 0;
    bool chk = 0;
    for (int v : a[u]) {
        if (v == par) {
            continue;
        }
        if (vst.find({u, v}) == vst.end()) {
            if (!id[v]) {
                chk         = 1;
                vst[{u, v}] = 1;
                res         = max(res, DFS(v, u, root));
            } else {
                if (!ok) {
                    ok = v;
                }
                if (ok == u || (ok == -1 && v != root)) {
                    if (ok != -1) {
                        ok   = -1;
                        root = u;
                    }
                    chk         = 1;
                    vst[{u, v}] = 1;
                    res         = max(res, DFS(v, u, root));
                }
            }
        } else {
            chk = 1;
            res = max(res, f[{u, v}]);
        }
    }
    if (!chk) {
        res++;
    }
    f[{par, u}] = max(f[{par, u}], res);
    if (u != par) {
        f[{par, u}]++;
    }
    return f[{par, u}];
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int u, v;
        cin >> u >> v;
        deg[u] += 2;
        deg[v] += 2;
        a[u].push_back(v);
        a[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) {
        if (deg[i] == 2) {
            cout << DFS(i, i, 0);
            return 0;
        }
    }
    cout << n;
    return 0;
}