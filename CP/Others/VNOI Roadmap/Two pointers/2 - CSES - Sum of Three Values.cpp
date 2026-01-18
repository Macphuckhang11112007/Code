// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 5000 + 5;

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
    for (int i = 1; i <= n; ++i)
    {
        int l = i + 1, r = n;
        while (l < r)
        {
            long long sum  = a[i].first + a[l].first + a[r].first;
            int       id_l = a[l].second, id_r = a[r].second;
            if (sum == x)
            {
                vector<int> ids = {a[i].second, id_l, id_r};
                sort(ids.begin(), ids.end());
                cout << ids[0] << ' ' << ids[1] << ' ' << ids[2] << '\n';
                return 0;
            }
            else if (sum < x) { ++l; }
            else if (sum > x) { --r; }
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}
