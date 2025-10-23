// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 2e5 + 5, kMaxB = 2e6 + 5, buffer = 1e6;

int           n, a[kMaxN];
bitset<kMaxB> nums;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        cin >> a[i];
        if (!nums[a[i]]) { nums[a[i]] = 1; }
        else { nums[buffer + a[i]] = 1; }
    }
    int maxx = *max_element(a + 1, a + n + 1);
    for (int i = maxx; i >= 1; --i)
    {
        int cnt = 0;
        for (int j = i; j <= maxx; j += i)
        {
            cnt += nums[j] + nums[buffer + j];
        }
        if (cnt >= 2)
        {
            cout << i << '\n';
            return 0;
        }
    }
    return 0;
}
