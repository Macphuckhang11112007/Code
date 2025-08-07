// MPK

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int kMaxN = 50000 + 5;

int n, k;
int a[kMaxN];
int max_l[kMaxN], max_r[kMaxN];
int ans = 0;

int main()
{
#ifndef CPH
    ifstream cin("diamond.in");
    ofstream cout("diamond.out");
#endif
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    sort(a + 1, a + n + 1);
    for (int l = 1, r = 1; r <= n; ++r)
    {
        while (a[r] - a[l] > k) { ++l; }
        max_l[r] = max(max_l[r - 1], r - l + 1);
    }
    for (int l = n, r = n; l >= 1; --l)
    {
        while (a[r] - a[l] > k) { --r; }
        max_r[l] = max(max_r[l + 1], r - l + 1);
    }
    for (int i = 1; i <= n; ++i) { ans = max(ans, max_l[i] + max_r[i + 1]); }
    cout << ans << '\n';
    return 0;
}
