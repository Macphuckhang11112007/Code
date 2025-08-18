// MPK
// Finish Time: 8/17/2025, 1:03:02 PM AC
// Start Time: 8/17/2025, 12:20:54 PM

// Categories: *800 (*674), Greedy, Implementation, Math
#include <bits/stdc++.h>

using namespace std;

int         t, n, k, ans;
vector<int> a;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> n >> k;
        cout << (n + k - 1) / k + ((n - 1) % k != 0) << "\n";
    }
    return 0;
}