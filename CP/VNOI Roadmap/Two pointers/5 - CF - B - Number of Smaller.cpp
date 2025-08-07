// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxNM = 1e5 + 5;

int n, m;
int a[kMaxNM], b[kMaxNM];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    for (int i = 1; i <= m; ++i) { cin >> b[i]; }
    for (int i = 1, j = 1; j <= m; ++j)
    {
        while (i <= n && a[i] < b[j]) { ++i; }
        cout << i - 1 << " ";
    }
    return 0;
}
