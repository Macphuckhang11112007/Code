// MPK

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int kMaxN = 1e5 + 5;

int                                 n, spf[kMaxN];
long long                           ans;
vector<int>                         primes;
unordered_map<long long, long long> cnt;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    spf[0] = spf[1] = 0;
    for (int i = 2; i <= n; ++i)
    {
        if (!spf[i])
        {
            spf[i] = i;
            primes.push_back(i);
        }
        for (const int &j : primes)
        {
            if (1LL * i * j > n || j > spf[i]) { break; }
            spf[i * j] = j;
        }
    }
    for (int i = 1; i <= n; ++i)
    {
        unordered_map<int, int> fact;
        int                     x = i;
        while (spf[x])
        {
            ++fact[spf[x]];
            x /= spf[x];
        }
        long long y = 1;
        for (const auto &[num, c] : fact)
        {
            if (c & 1) { y *= num; }
        }
        ans += 2 * cnt[y] + 1;
        ++cnt[y];
    }
    cout << ans;
    return 0;
}
