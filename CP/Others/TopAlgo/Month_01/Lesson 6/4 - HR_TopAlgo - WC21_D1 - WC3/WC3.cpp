#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 2e5 + 5, kMaxM = 10 + 5;

int n, m, tc;
long long ans = 0;
map<vector<int>, int> cnt_map;
int arr[kMaxN][kMaxM], pre_sum[kMaxN][kMaxM];

signed main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--)
    {
        cnt_map.clear();
        ans = 0;
        cin >> n >> m;
        cnt_map[vector<int>(m - 1, 0)] = 1;
        for (int i = 1; i <= n; ++i)
        {
            vector<int> tmp_arr(m - 1, 0);
            for (int j = 1; j <= m; ++j)
            {
                cin >> arr[i][j];
                pre_sum[i][j] = pre_sum[i - 1][j] + arr[i][j];
                if (j >= 2)
                {
                    tmp_arr[j - 2] = pre_sum[i][j] - pre_sum[i][1];
                }
            }
            ans += cnt_map[tmp_arr]++;
        }
        cout << ans << "\n";
    }
    return 0;
}
