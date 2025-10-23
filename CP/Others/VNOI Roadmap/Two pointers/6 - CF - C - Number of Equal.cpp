// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxNM = 1e5 + 5;

int       n, m;
long long ans = 0;
int       a[kMaxNM], b[kMaxNM];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    for (int j = 1; j <= m; ++j) { cin >> b[j]; }
    int i = 1, j = 1;
    while (i <= n && j <= m)
    {
        int cnt_a = 1, cnt_b = 1;
        if (a[i] == b[j])
        {
            while (i < n && a[i] == a[i + 1])
            {
                ++i;
                ++cnt_a;
            }
            while (j < m && b[j] == b[j + 1])
            {
                ++j;
                ++cnt_b;
            }
            ans += 1LL * cnt_a * cnt_b;
            ++i;
            ++j;
        }
        else if (a[i] < b[j]) { ++i; }
        else if (a[i] > b[j]) { ++j; }
    }
    cout << ans << '\n';
    return 0;
}
