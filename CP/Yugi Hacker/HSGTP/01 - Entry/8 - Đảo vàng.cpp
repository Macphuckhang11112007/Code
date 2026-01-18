#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e5 + 5;

struct Node
{
    int            par;
    int            sz;
    long long      sum;
    pair<int, int> max;
};

int  n, m, q;
Node node[kMaxN];

int Par(int u)
{
    if (node[u].par == u) { return u; }
    return node[u].par = Par(node[u].par);
}

bool DSU(int u, int v)
{
    int pu = Par(u);
    int pv = Par(v);
    if (pu == pv) { return false; }
    if (node[pu].sz < node[pv].sz) swap(pu, pv);
    node[pv].par = pu;
    node[pu].sz += node[pv].sz;
    node[pu].sum += node[pv].sum;
    node[pu].max = max(node[pu].max, node[pv].max);
    return true;
}
int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int u = 1; u <= n; ++u)
    {
        int val;
        cin >> val;
        node[u].par = u;
        node[u].sz  = 1;
        node[u].sum = val;
        node[u].max = {val, u};
    }
    while (m--)
    {
        int u, v;
        cin >> u >> v;
        DSU(u, v);
    }
    cin >> q;
    while (q--)
    {
        int u;
        cin >> u;
        int pu = Par(u);
        cout << node[pu].sz << ' ' << node[pu].sum << ' ' << node[pu].max.first
             << '\n';
    }
    return 0;
}
