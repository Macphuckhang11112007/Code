#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 3000 + 5;

int n;
long long ans = 0;
bitset<kMaxN> adj[kMaxN];

signed main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            char in_char;
            cin >> in_char;
            adj[i][j] = in_char - '0';
        }
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = i + 1; j <= n; ++j)
        {
            if (adj[i][j])
            {
                bitset<kMaxN> tmp_adj = adj[i] & adj[j];
                ans += tmp_adj.count();
            }
        }
    }
    cout << ans / 3 << "\n";
    return 0;
}
