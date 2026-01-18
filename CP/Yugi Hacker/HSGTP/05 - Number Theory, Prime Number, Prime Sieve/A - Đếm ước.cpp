// MPK

#include <bits/stdc++.h>

using namespace std;

long long         n;
bitset<1'000'005> not_prime;
vector<int>       primes;
int               lim, ans = 1;

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
        ans *= cnt + 1;
    }
    if (n > 1) { ans *= 2; }
    cout << ans << '\n';
    return 0;
}
