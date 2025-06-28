#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

int main()
{
    freopen("SDB.inp", "r", stdin);
    freopen("SDB.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    unordered_map<int, int> mp;
    int n, a[1000005];
    vector<int> ans;
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
        mp[a[i]]++;
    }
    for (int i = 1; i <= n; i++)
    {
        if (mp[a[i]] == 1)
        {
            ans.push_back(a[i]);
        }
    }
    cout << ans.size() << "\n";
    for (int i : ans)
    {
        cout << i << "\n";
    }
    return 0;
}
