// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e5 + 5;

int       n, ans = 0;
long long s, sum = 0;
int       a[kMaxN];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> s;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    for (int l = 1, r = 1; r <= n; ++r)
    {
        sum += a[r];
        while (sum > s) { sum -= a[l++]; }
        ans = max(ans, r - l + 1);
    }
    cout << ans << '\n';
    return 0;
}
