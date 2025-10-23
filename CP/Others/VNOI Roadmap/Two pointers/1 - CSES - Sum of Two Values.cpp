// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 2e5 + 5;

int            n, x;
pair<int, int> a[kMaxN];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> x;
    for (int i = 1; i <= n; ++i)
    {
        cin >> a[i].first;
        a[i].second = i;
    }
    sort(a + 1, a + n + 1);
    int l = 1, r = n;
    while (l < r)
    {
        long long sum  = a[l].first + a[r].first;
        int       id_l = a[l].second, id_r = a[r].second;
        if (sum == x)
        {
            cout << min(id_l, id_r) << " " << max(id_l, id_r) << "\n";
            return 0;
        }
        else if (sum < x) { ++l; }
        else if (sum > x) { --r; }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}
