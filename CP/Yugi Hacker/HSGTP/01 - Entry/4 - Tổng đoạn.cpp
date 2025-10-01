#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e6 + 5;

int       n, t;
long long arr[kMaxN];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        cin >> arr[i];
        arr[i] += arr[i - 1];
    }
    cin >> t;
    while (t--)
    {
        int l, r;
        cin >> l >> r;
        cout << arr[r] - arr[l - 1] << '\n';
    }
    return 0;
}
