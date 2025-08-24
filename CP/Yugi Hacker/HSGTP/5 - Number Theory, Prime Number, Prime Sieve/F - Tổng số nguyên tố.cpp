// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e7 + 5;

int           n;
bitset<kMaxN> not_prime;
long long     ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    if (n >= 2) { ans = 2; }
    for (int i = 3; i <= n; i += 2)
    {
        if (!not_prime[i])
        {
            ans += i;
            if (1LL * i * i <= n)
            {
                for (int j = i * i; j <= n; j += 2 * i) { not_prime[j] = 1; }
            }
        }
    }
    cout << ans << '\n';
    return 0;
}
