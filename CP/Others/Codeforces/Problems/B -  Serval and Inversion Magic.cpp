// MPK
// Finish Time: 8/17/2025, 5:23:36 PM AC
// Finish Time: 8/17/2025, 5:23:23 PM WA
// Start Time: 8/17/2025, 5:06:06 PM

#include <bits/stdc++.h>

using namespace std;

int    t, n, prev_id;
string a;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        prev_id = 0;
        cin >> n >> a;
        a = " " + a;
        for (int i = 1; 2 * i <= n && prev_id != -1; i++)
        {
            if (a[i] != a[n - i + 1])
            {
                prev_id = (!prev_id || i == prev_id + 1 ? i : -1);
            }
        }
        cout << (prev_id != -1 ? "Yes\n" : "No\n");
    }
    return 0;
}