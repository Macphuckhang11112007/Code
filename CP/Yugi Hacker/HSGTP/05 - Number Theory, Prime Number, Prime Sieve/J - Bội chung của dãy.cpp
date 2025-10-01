// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e5 + 5, kLim = 1e5, kMaxVal = 1e6 + 5, kMod = 1e9 + 7;

int           n, q, a[kMaxN], spf[kMaxVal];
long long     ans = 1;
vector<int>   primes;
map<int, int> nums, fact[kMaxN];

int SPF(int x) { return (~x & 1 ? 2 : spf[x]); }

long long binpow(long long x, long long y)
{
    long long res = 1;
    while (y)
    {
        if (y & 1) { (res *= x) %= kMod; }
        (x *= x) %= kMod;
        y >>= 1;
    }
    return res;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 3; i <= kLim; i += 2)
    {
        if (!spf[i])
        {
            spf[i] = i;
            primes.push_back(i);
        }
        for (const auto &p : primes)
        {
            if (1LL * i * p > kLim || p > spf[i]) { break; }
            spf[i * p] = p;
        }
    }
    cin >> n >> q;
    for (int i = 1; i <= n; ++i)
    {
        cin >> a[i];
        int x = a[i];
        while (SPF(x))
        {
            int maxx = max(nums[SPF(x)], ++fact[i][SPF(x)]);
            if (maxx > nums[SPF(x)])
            {
                (ans *= binpow(SPF(x), maxx - nums[SPF(x)])) %= kMod;
                nums[SPF(x)] = maxx;
            }
            x /= SPF(x);
        }
    }
    while (q--)
    {
        int id, x;
        cin >> id >> x;
        while (SPF(x))
        {
            int maxx = max(nums[SPF(x)], ++fact[id][SPF(x)]);
            if (maxx > nums[SPF(x)])
            {
                (ans *= binpow(SPF(x), maxx - nums[SPF(x)])) %= kMod;
                nums[SPF(x)] = maxx;
            }
            x /= SPF(x);
        }
        cout << ans << '\n';
    }
    return 0;
}
