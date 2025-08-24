// MPK

#include <bits/stdc++.h>

using namespace std;

long long                    n;
bitset<1'000'005>            not_prime;
vector<int>                  primes;
int                          lim;
vector<pair<long long, int>> nums;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    lim = sqrt(n);
    primes.push_back(2);
    for (int i = 3; i <= lim; i += 2)
    {
        if (!not_prime[i])
        {
            primes.push_back(i);
            if (1LL * i * i <= lim)
            {
                for (int j = i * i; j <= lim; j += 2 * i) { not_prime[j] = 1; }
            }
        }
    }
    for (const int &p : primes)
    {
        int cnt = 0;
        while (n > 1 && n % p == 0)
        {
            n /= p;
            ++cnt;
        }
        if (cnt) { nums.push_back({p, cnt}); }
    }
    if (n > 1) { nums.push_back({n, 1}); }
    cout << nums.size() << '\n';
    for (const auto &[p, cnt] : nums) { cout << p << ' ' << cnt << '\n'; }
    return 0;
}
