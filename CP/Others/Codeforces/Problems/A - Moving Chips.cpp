// MPK
// Finish Time: 8/18/2025, 10:59:03 AM AC
// Start Time: 8/18/2025, 9:53:10 AM

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int N = 50;

int         t, n, m, ans;
int         a[N + 5];
vector<pii> arr;

pii DFS(int i)
{
    if (i <= 0 || i > m) { return {0, 0}; }
    pii x = DFS(i + 1);
    if (x.se) { x.fi += x.se - arr[i].se - 1; }
    x.se = arr[i].fi;
    return x;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        arr = {{0, 0}};
        cin >> n;
        for (int i = 1; i <= n; i++)
        {
            cin >> a[i];
            if (!a[i - 1] && a[i]) { arr.push_back({i, i}); }
            else if (a[i - 1] && a[i]) { arr.back().se = i; }
        }
        m   = arr.size() - 1;
        ans = DFS(1).fi;
        cout << ans << "\n";
    }
    return 0;
}