// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e5 + 5, kInf = INT_MAX / 2;

int       n, ans = kInf;
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
        while (sum - a[l] >= s) { sum -= a[l++]; }
        if (sum >= s) { ans = min(ans, r - l + 1); }
    }
    cout << (ans != kInf ? ans : -1) << '\n';
    return 0;
}
