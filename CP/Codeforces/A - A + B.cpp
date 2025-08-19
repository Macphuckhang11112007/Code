// MPK
// Finish Time: 8/19/2025, 10:28:25 AM AC
// Start Time: 8/19/2025, 10:21:57 AM

#include <bits/stdc++.h>

using namespace std;

int t;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        char a, b, _;
        cin >> a >> _ >> b;
        cout << (a - '0') + (b - '0') << "\n";
    }
    return 0;
}