// MPK

#include <bits/stdc++.h>

using namespace std;

long long a, b, m;

long long binpow(long long x, long long y)
{
    if (y == 0) { return 1; }
    if (x == 0) { return 0; }
    long long res = binpow(x, y / 2);
    (res *= res) %= m;
    if (y & 1) { (res *= x) %= m; }
    return res;
}

long long binexp(long long x, long long y)
{
    if (y == 0) { return 1; }
    if (x == 0) { return 0; }
    long long res = binexp(x, y / 2);
    long long pow = binpow(x, (y + 1) / 2);
    (res += pow * res) %= m;
    if (~y & 1) { ((res -= pow) += m) %= m; }
    return res;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> a >> b >> m;
    cout << binexp(a, b) << '\n';
    return 0;
}
