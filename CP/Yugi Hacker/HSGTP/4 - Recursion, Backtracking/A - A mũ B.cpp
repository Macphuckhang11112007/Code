// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMod = 1e9 + 7;

int       q;
long long a, b;

// long long binpow(long long a, long long b)
// {
//     if (b == 0) { return 1; }
//     if (a == 0) { return 0; }
//     long long res = 1;
//     while (b > 0)
//     {
//         if (b & 1) { (res *= a) %= kMod; }
//         (a *= a) %= kMod;
//         b >>= 1;
//     }
//     return res;
// }

long long binpow(long long x, long long y)
{
    if (y == 0) { return 1; }
    if (x == 0) { return 0; }
    long long res = binpow(x, y / 2);
    (res *= res) %= kMod;
    if (y & 1) { (res *= x) %= kMod; }
    return res;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> q;
    while (q--)
    {
        cin >> a >> b;
        cout << binpow(a, b) << '\n';
    }
    return 0;
}
