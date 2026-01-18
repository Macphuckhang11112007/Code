// MPK

#include <bits/stdc++.h>

using namespace std;

int           n, x;
map<int, int> cnt;
long long     ans = 0;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> x;
    while (n--)
    {
        int val;
        cin >> val;
        ans += cnt[x - val];
        ++cnt[val];
    }
    cout << ans << '\n';
    return 0;
}
