// MPK
// Finish Time: 8/17/2025, 12:19:52 PM AC
// Start Time: 8/17/2025, 12:12:53 PM

// Categories: *800 (*472), Brute Force, Constructive Algorithms, Greedy, Math
#include <bits/stdc++.h>

using namespace std;

int         t, n, m;
vector<int> a;

bool BT(int i, int sz, int sum)
{
    if (i > sz) { return sum == 0; }
    return BT(i + 1, sz, sum + a[i]) || BT(i + 1, sz, sum - a[i]);
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> n >> m;
        a.resize(n + m + 5);
        for (int i = 1; i <= n; i++) { a[i] = 1; }
        for (int i = n + 1; i <= n + m; i++) { a[i] = 2; }
        cout << (BT(1, n + m, 0) ? "YES\n" : "NO\n");
    }
    return 0;
}