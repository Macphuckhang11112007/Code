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
    int i, j;
    i = j = 1;
    while (i <= n && j <= m)
    {
        if (a[i] <= b[j])
        {
            cout << a[i] << " ";
            ++i;
        }
        else
        {
            cout << b[j] << " ";
            ++j;
        }
    }
    while (i <= n) { cout << a[i++] << " "; }
    while (j <= m) { cout << b[j++] << " "; }
    cout << "\n";
    return 0;
}
