// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 20 + 5;

int       n, s, a[kMaxN];
long long ans;

void BT(int i, int sum, int chk)
{
    if (sum > s) { return; }
    else if (i > n)
    {
        ans += (chk && sum == s);
        return;
    }
    else
    {
        BT(i + 1, sum, chk);
        BT(i + 1, sum + a[i], 1);
    }
    return;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> s;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    BT(1, 0, 0);
    cout << ans << '\n';
    return 0;
}
