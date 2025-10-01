#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e6 + 5, lim = 1e6;

int         t, n;
long long   ans            = 1;
int         spf[kMaxN / 2] = {0};
vector<int> primes;

int SPF(int x)
{
    if (x < 2) return 0;
    if (!(x & 1)) return 2;
    int id = (x - 1) / 2;
    return spf[id];
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 3; i <= lim; i += 2)
    {
        int id = (i - 1) / 2;
        if (!spf[id])
        {
            spf[id] = i;
            primes.push_back(i);
        }
        for (const auto& p : primes)
        {
            if (1LL * p * i > lim || p > spf[id]) break;
            int _id  = (i * p - 1) / 2;
            spf[_id] = p;
        }
    }
    cin >> t;
    while (t--)
    {
        int cnt = 0;
        ans     = 1;
        cin >> n;
        while (int _spf = SPF(n))
        {
            n /= _spf;
            ++cnt;
            if (SPF(n) != _spf)
            {
                ans *= ++cnt;
                cnt = 0;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
