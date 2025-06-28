#include <bits/stdc++.h>

#define int long long
#define pii pair<int, int>
#define fi first
#define se second
#define N (int)(1e6 + 10)

using namespace std;

int n, lim;
int a[N], spf[N];
vector<int> p, ans;

int pow(int x, int y)
{
    if (x == 0)
    {
        return 0;
    }
    if (y == 0)
    {
        return 1;
    }
    int z = pow(x, y / 2);
    return z * z * (y % 2 ? x : 1);
}

signed main()
{
    freopen("GHH.inp", "r", stdin);
    freopen("GHH.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i];
    }
    lim = *max_element(a + 1, a + n + 1);
    spf[0] = spf[1] = 0;
    for (int i = 2; i <= lim; i++)
    {
        if (!spf[i])
        {
            p.push_back(i);
            spf[i] = i;
        }
        for (int j : p)
        {
            if (j > spf[i] || lim / i < j)
            {
                break;
            }
            else
            {
                spf[i * j] = j;
            }
        }
    }
    for (int i = 1; i <= n; i++)
    {
        int s = 1, x = a[i];
        map<int, int> mp;
        while (spf[x])
        {
            mp[spf[x]]++;
            x /= spf[x];
        }
        for (pii j : mp)
        {
            cerr << "..." << j.fi << " " << j.se << "\n";
            s *= (pow(j.fi, j.se + 1) - 1) / (j.fi - 1);
        }
        cerr << a[i] << " " << s << "\n";
        if (2 * a[i] <= s)
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
