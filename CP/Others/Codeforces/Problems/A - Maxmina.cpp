// MPK
// Finish Time: 8/17/2025, 6:18:40 PM AC
// Start Time: 8/17/2025, 6:10:03 PM

#include <bits/stdc++.h>

using namespace std;

int t, n, k;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        bool ok = 0;
        cin >> n >> k;
        for (int i = 1; i <= n; i++)
        {
            int x;
            cin >> x;
            ok |= x;
        }
        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}