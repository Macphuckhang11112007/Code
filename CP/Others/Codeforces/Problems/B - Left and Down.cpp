// MPK
// Finish Time: 8/12/2025, 12:02:46 PM AC
// Start Time: 8/12/2025, 11:08:38 AM AC

// Categories: Math
#include <bits/stdc++.h>

using namespace std;

int                t;
unsigned long long a, b, k;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> a >> b >> k;
        if (max(a, b) / gcd(a, b) <= k) { cout << "1\n"; }
        else { cout << "2\n"; }
    }
    return 0;
}
