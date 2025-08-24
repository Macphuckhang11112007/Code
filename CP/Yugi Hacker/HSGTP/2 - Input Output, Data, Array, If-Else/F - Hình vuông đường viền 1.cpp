// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxNM = 205, kInf = INT_MAX / 2;

int n, m, ans = -kInf;
int a[kMaxNM][kMaxNM], s[kMaxNM][kMaxNM];

int Sum(int i_low, int j_low, int i_high, int j_high)
{
    return s[i_high][j_high] - s[i_low - 1][j_high] - s[i_high][j_low - 1] +
           s[i_low - 1][j_low - 1];
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            cin >> a[i][j];
            s[i][j] = s[i - 1][j] + s[i][j - 1] - s[i - 1][j - 1] + a[i][j];
        }
    }
    for (int i_low = 1; i_low <= n; ++i_low)
    {
        for (int i_high = 1; i_high <= n; ++i_high)
        {
            int sz = i_high - i_low + 1;
            for (int j_low = 1, j_high = j_low + sz - 1; j_high <= m;
                 ++j_low, ++j_high)
            {
                if (Sum(i_low, j_low, i_high, j_low) == sz &&
                    Sum(i_low, j_high, i_high, j_high) == sz &&
                    Sum(i_low, j_low, i_low, j_high) == sz &&
                    Sum(i_high, j_low, i_high, j_high) == sz)
                {
                    ans = max(ans, sz * sz);
                }
            }
        }
    }
    cout << ans;
    return 0;
}
