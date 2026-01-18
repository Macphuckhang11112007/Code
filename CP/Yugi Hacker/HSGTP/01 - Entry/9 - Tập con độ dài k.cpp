#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 2e5 + 5;

int                 t, n, k;
long long           ans, sum;
pair<int, int>      arr[kMaxN];
priority_queue<int> pq;

int main()
{
    freopen("minkseq.inp", "r", stdin);
    freopen("minkseq.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        ans = numeric_limits<long long>::max() / 2;
        sum = 0;
        pq  = priority_queue<int>();
        cin >> n >> k;
        for (int i = 1; i <= n; ++i) { cin >> arr[i].first; }
        for (int i = 1; i <= n; ++i) { cin >> arr[i].second; }
        sort(arr + 1, arr + n + 1,
             [](const pair<int, int>& i, const pair<int, int>& j)
             {
                 if (i.first != j.first) { return i.first < j.first; }
                 return i.second > j.second;
             });
        for (int i = 1; i <= n; ++i)
        {
            if (i < k)
            {
                pq.push(arr[i].second);
                sum += arr[i].second;
            }
            else if (i >= k)
            {
                bool chk = 0;
                if (i > k && pq.top() > arr[i].second)
                {
                    sum -= pq.top();
                    pq.pop();
                    chk = 1;
                }
                if (i == k || chk)
                {
                    sum += arr[i].second;
                    pq.push(arr[i].second);
                }
                ans = min(ans, arr[i].first * sum);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
