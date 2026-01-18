// MPK
// Finish Time: 8/17/2025, 6:25:59 PM AC
// Start Time: 8/17/2025, 6:21:11 PM

#include <bits/stdc++.h>

using namespace std;

int t, n;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        int minn = INT_MAX / 2, maxx = INT_MIN / 2;
        cin >> n;
        for (int i = 1; i <= n; i++)
        {
            int x;
            cin >> x;
            minn = min(minn, x);
            maxx = max(maxx, x);
        }
        cout << maxx - minn << "\n";
    }
    return 0;
}