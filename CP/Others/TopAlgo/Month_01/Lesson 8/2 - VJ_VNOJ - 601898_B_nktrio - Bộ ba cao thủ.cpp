#undef _GLIBCXX_DEBUG

#include <bits/stdc++.h>

#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("arch=tigerlake")

using namespace std;

const int kMaxN = 1000 + 5;

int           n;
bitset<kMaxN> win[kMaxN];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            int val;
            cin >> val;
            win[i][j] = bool(val);
        }
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = i + 1; j <= n; ++j)
        {
            if (!win[i][j] && !win[j][i]) continue;
            int x = i, y = j, z = 0;
            if (!win[i][j]) swap(x, y);
            bitset<kMaxN> tmp_win = win[x] ^ win[y];
            tmp_win &= ~win[x];
            z = tmp_win._Find_first();
            if (1 <= z && z <= n)
            {
                cout << x << " " << y << " " << z << "\n";
                return 0;
            }
        }
    }
    cout << "-1 -1 -1\n";
    return 0;
}
