// MPK
// Finish Time: 8/19/2025, 10:49:53 AM AC
// Finish Time: 8/19/2025, 10:49:32 AM WA
// Start Time: 8/19/2025, 10:29:37 AM

#include <bits/stdc++.h>

using namespace std;

int         t, n, m, ans;
vector<int> a;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        bool ok = 0;
        ans     = 0;
        a.clear();
        cin >> n;
        char prev_c = '#';
        for (int i = 1; i <= n; i++)
        {
            char c;
            cin >> c;
            if (c == '.')
            {
                if (prev_c == '.') { a.back()++; }
                else { a.push_back(1); }
            }
            prev_c = c;
        }
        sort(a.begin(), a.end(), greater<int>());
        a.insert(a.begin(), 0);
        m = a.size() - 1;
        for (int i = 1; i <= m; i++)
        {
            if (!ok)
            {
                ok |= a[i] >= 3;
                ans += min(a[i], 2);
            }
        }
        cout << ans << "\n";
    }
    return 0;
}