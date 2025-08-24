// MPK

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int kMaxN = 1e6 + 5, lim = 1e6;

int         n, a[kMaxN], spf[kMaxN];
vector<int> primes = {2};
pii         ans;

int main()
{
#ifndef CPH
    ifstream cin("Prime.inp");
    ofstream cout("Prime.out");
#endif
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 3; i <= lim; i += 2)
    {
        if (!spf[i])
        {
            spf[i] = i;
            primes.push_back(i);
        }
        for (const auto &p : primes)
        {
            if (1LL * i * p > lim || p > spf[i]) { break; }
            spf[i * p] = p;
        }
    }
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        cin >> a[i];
        int cnt      = 0;
        int x        = a[i];
        int prev_spf = 0;
        while ((~x & 1) || spf[x])
        {
            int _spf = (~x & 1 ? 2 : spf[x]);
            if (_spf != prev_spf)
            {
                ++cnt;
                prev_spf = _spf;
            }
            x /= _spf;
        }
        if (cnt > ans.fi)
        {
            ans.fi = cnt;
            ans.se = a[i];
        }
    }
    cout << ans.se << '\n';
    return 0;
}
