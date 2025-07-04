#include <bits/stdc++.h>

using namespace std;

int n, h, ans = 0;

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> n >> h;
    for (int i = 1; i <= n; i++)
    {
        int x; cin >> x;
        if (x > h) ans += 2; else ans++;
    }
    cout << ans;
    return 0;
}