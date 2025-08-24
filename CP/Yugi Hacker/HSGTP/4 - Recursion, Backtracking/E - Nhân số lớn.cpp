// MPK

#include <bits/stdc++.h>

using namespace std;

// unsigned long long x, y, m, ans;
// __uint128_t        a, b;

// int main()
// {
//     cin.tie(0)->sync_with_stdio(0);
//     cin >> x >> y >> m;
//     a   = x;
//     b   = y;
//     ans = (a * b) % m;
//     cout << ans << '\n';
//     return 0;
// }

unsigned long long a, b, m;

unsigned long long binadd(unsigned long long x, unsigned long long y)
{
    if (x == 0 || y == 0) { return 0; }
    unsigned long long res = binadd(x, y / 2);
    (res += res) %= m;
    if (y & 1) { (res += x) %= m; }
    return res;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> a >> b >> m;
    cout << binadd(a, b) << '\n';
    return 0;
}
