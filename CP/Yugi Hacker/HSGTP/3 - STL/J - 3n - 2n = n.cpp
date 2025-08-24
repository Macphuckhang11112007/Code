// MPK

#include <bits/stdc++.h>

using namespace std;

const int       kMaxN = 1e5 + 5, kMax3N = 3 * kMaxN + 5;
const long long kInf = LLONG_MAX / 2;

int       n, a[kMax3N];
long long ans = -kInf, pre[kMax3N], suf[kMax3N];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= 3 * n; ++i) { cin >> a[i]; }
    priority_queue<int, vector<int>, greater<int>> pq_min;
    long long                                      sum_max = 0;
    for (int i = 1; i <= n; ++i)
    {
        sum_max += a[i];
        pq_min.push(a[i]);
    }
    pre[n] = sum_max;
    for (int i = 2 * n; i <= 3 * n; ++i)
    {
        pq_min.push(a[i]);
        sum_max += a[i];
        sum_max -= pq_min.top();
        pq_min.pop();
        pre[i] = sum_max;
    }
    priority_queue<int> pq_max;
    long long           sum_min = 0;
    for (int i = 3 * n; i >= 1; --i)
    {
        if (i > 2 * n)
        {
            sum_min += a[i];
            pq_max.push(a[i]);
        }
        else if (i <= 2 * n && pq_max.top() > a[i])
        {
            sum_min -= pq_max.top();
            pq_max.pop();
            sum_min += a[i];
            pq_max.push(a[i]);
        }
        suf[i] = sum_min;
    }
    for (int i = n; i <= 2 * n; ++i) { ans = max(ans, pre[i] - suf[i + 1]); }
    cout << ans << '\n';
    return 0;
}
