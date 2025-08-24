// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e6 + 5, kInf = INT_MAX / 2;

int           n, a, ans = kInf;
int           spf[kMaxN];
vector<int>   primes;
map<int, int> nums_a, nums_n;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> a;
    primes.push_back(2);
    for (int i = 3; i <= max(n, a); i += 2)
    {
        if (!spf[i])
        {
            spf[i] = i;
            primes.push_back(i);
        }
        for (const auto &p : primes)
        {
            if (1LL * i * p > max(n, a) || p > spf[i]) { break; }
            spf[i * p] = p;
        }
    }
    while ((~a & 1) || spf[a])
    {
        int x = (~a & 1 ? 2 : spf[a]);
        ++nums_a[x];
        a /= x;
    }
    for (const int &p : primes)
    {
        int x = n;
        while (x > 1)
        {
            nums_n[p] += x / p;
            x /= p;
        }
    }
    for (const auto &[p, cnt_a] : nums_a)
    {
        int cnt_n = nums_n[p];
        ans       = min(ans, cnt_n / cnt_a);
    }
    cout << ans << '\n';
    return 0;
}
