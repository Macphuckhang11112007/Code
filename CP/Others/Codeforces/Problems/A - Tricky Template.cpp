// MPK
// Start Time: 8/18/2025, 8:47:54 PM

#include <bits/stdc++.h>

using namespace std;

const int N = 20;

int  t, n;
char a[N + 5], b[N + 5], c[N + 5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        bool ok = 0;
        cin >> n;
        for (int i = 1; i <= n; i++) { cin >> a[i]; }
        for (int i = 1; i <= n; i++) { cin >> b[i]; }
        for (int i = 1; i <= n; i++) { cin >> c[i]; }
        for (int i = 1; i <= n; i++) { ok |= (c[i] != a[i] && c[i] != b[i]); }
        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}