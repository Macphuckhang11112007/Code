// MPK
// Start Time: 8/20/2025, 6:20:44 AM

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int N = 1000;

int                           n, a[N + 5];
long long                     s;
unordered_map<long long, pii> mp;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> s;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    mp[a[1] + a[2]] = {1, 2};
    for (int i = 3; i <= n; i++)
    {
        for (int j = i + 1; j <= n; j++)
        {
            long long sum = a[i] + a[j];
            if (mp[s - sum] != make_pair(0, 0))
            {
                pii p = mp[s - sum];
                cout << p.fi << " " << p.se << " " << i << " " << j << "\n";
                return 0;
            }
        }
        for (int j = 1; j < i; j++)
        {
            long long sum = a[j] + a[i];
            mp[sum]       = {j, i};
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}