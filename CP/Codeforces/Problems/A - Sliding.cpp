// MPK
// Finish Time: 8/17/2025, 6:04:29 PM AC
// Start Time: 8/17/2025, 5:48:58 PM

#include <bits/stdc++.h>

using namespace std;

long long t, n, m, r, c;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> n >> m >> r >> c;
        cout << (n - r) * (m - 1) + (n * m - ((r - 1) * m + c)) << "\n";
    }
    return 0;
}