// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxNM = 500 + 5;

int       n, m, mat[kMaxNM][kMaxNM];
long long a, b, q = LLONG_MAX, s[kMaxNM][kMaxNM];

int main()
{
#ifndef CPH
    ifstream cin("DauTu.inp");
    ofstream cout("DauTu.out");
#endif
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> a >> b;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j)
        {
            cin >> mat[i][j];
            s[i][j] = s[i - 1][j] + mat[i][j];
        }
    }
    for (int i1 = 1; i1 <= n; ++i1)
    {
        for (int i2 = i1; i2 <= n; ++i2)
        {
            int       l_low = 1, l_mid = 1, l_high = 1;
            long long sum_low = 0, sum_mid = 0, sum_high = 0;
            for (int r = 1; r <= m; ++r)
            {
                sum_low += s[i2][r] - s[i1 - 1][r];
                sum_mid += s[i2][r] - s[i1 - 1][r];
                sum_high += s[i2][r] - s[i1 - 1][r];
                while (l_low <= r && sum_low > a)
                {
                    sum_low -= s[i2][l_low] - s[i1 - 1][l_low];
                    ++l_low;
                }
                while (l_mid <= r && sum_mid > b)
                {
                    sum_mid -= s[i2][l_mid] - s[i1 - 1][l_mid];
                    ++l_mid;
                }
                ++l_high;
                // if (sum_low <= a) { q = min(q, a + b - 2 * sum_low); }
                // if (a <= sum_mid && sum_mid <= b) { q = min(q, b - a); }
                // if (sum_high >= b) { q = min(q, 2 * sum_high - (a + b)); }
                q = min({q, abs(sum_low - a) + abs(sum_low - b),
                         abs(sum_mid - a) + abs(sum_mid - b),
                         abs(sum_high - a) + abs(sum_high - b)});
            }
        }
    }
    cout << q << '\n';
    return 0;
}
