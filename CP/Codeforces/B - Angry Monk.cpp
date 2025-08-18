// MPK
// Finish Time: 8/18/2025, 9:49:11 AM AC
// Start Time: 8/18/2025, 9:23:11 AM

#include <bits/stdc++.h>

using namespace std;

int       t, n, k;
long long ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        priority_queue<int> pq;
        ans = 0;
        cin >> n >> k;
        for (int i = 1; i <= k; i++)
        {
            int x;
            cin >> x;
            pq.push(x);
        }
        int p = pq.top();
        pq.pop();
        while (!pq.empty())
        {
            int x = pq.top();
            pq.pop();
            ans += 2 * x - 1;
        }
        cout << ans << "\n";
    }
    return 0;
}