// MPK
// Start Time: 8/17/2025, 12:20:54 PM

#include <bits/stdc++.h>

using namespace std;

int t, n, k;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> n >> k;
        cout << n / k + 1 << "\n";
    }
    return 0;
}