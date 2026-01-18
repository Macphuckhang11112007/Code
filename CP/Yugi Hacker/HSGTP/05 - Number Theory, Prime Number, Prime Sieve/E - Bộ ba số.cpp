// MPK

#include <bits/stdc++.h>

using namespace std;

int            n;
bitset<32'000> not_prime;
vector<int>    primes;
int            lim;
bool           ok;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    lim = sqrt(n - 2 * 2);
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
        if (p == 2) { continue; }
        int x = 2 * 2 + p * p;
        if (x <= lim)
        {
            if (!not_prime[x])
            {
                ok = 1;
                cout << "2 " << p << ' ' << x << '\n';
            }
        }
        else
        {
            for (const int &q : primes)
            {
                if (q * q > x) { break; }
                if (x % q == 0)
                {
                    x = 0;
                    break;
                }
            }
            if (x)
            {
                ok = 1;
                cout << "2 " << p << ' ' << x << '\n';
            }
        }
    }
    if (!ok) { cout << "-1\n"; }
    return 0;
}
