// MPK

#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 5;

struct Node
{
    int       par, val, cnt, maxx;
    long long sum;
} a[N];

int n, m, q;

int Par(int u)
{
    if (u == a[u].par) { return u; }
    a[u].par = Par(a[u].par);
    return a[u].par;
}

void DSU(int u, int v)
{
    int pu = Par(u);
    int pv = Par(v);
    if (pu == pv) { return; }
    if (a[pu].cnt < a[pv].cnt) { swap(pu, pv); }
    a[pv].par = pu;
    a[pu].cnt += a[pv].cnt;
    a[pu].sum += a[pv].sum;
    a[pu].maxx = max(a[pu].maxx, a[pv].maxx);
    return;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        int x;
        cin >> x;
        a[i].par  = i;
        a[i].val  = x;
        a[i].cnt  = 1;
        a[i].sum  = x;
        a[i].maxx = x;
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
        cout << a[pu].cnt << " " << a[pu].sum << " " << a[pu].maxx << "\n";
    }
    return 0;
}
