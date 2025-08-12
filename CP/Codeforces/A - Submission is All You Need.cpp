// MPK
// Finish Time: 8/12/2025, 10:51:35 AM AC

// Categories: Greedy
#include <bits/stdc++.h>

using namespace std;

const int N = 50;

int t, n, ans, a[N + 5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        ans = 0;
        cin >> n;
        for (int i = 1; i <= n; ++i)
        {
            cin >> a[i];
            ans += max(a[i], (a[i] ? a[i] - 1 : 1));
        }
        cout << ans << "\n";
    }
    return 0;
}
