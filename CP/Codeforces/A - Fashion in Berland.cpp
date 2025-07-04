#include <bits/stdc++.h>

using namespace std;

int n, cnt = 0;

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        int x; cin >> x;
        cnt += x;
    }
    if (n == 1) cnt--;
    cout << (cnt == n - 1 ? "YES" : "NO");
    return 0;
}