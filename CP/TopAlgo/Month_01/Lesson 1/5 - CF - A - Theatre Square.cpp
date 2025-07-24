#include <bits/stdc++.h>

using namespace std;

int n, m, a;

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> n >> m >> a;
    long long x = 1LL * (n + a - 1) / a,
        y = 1LL * (m + a - 1) / a;
    cout << x * y;
    return 0;
}