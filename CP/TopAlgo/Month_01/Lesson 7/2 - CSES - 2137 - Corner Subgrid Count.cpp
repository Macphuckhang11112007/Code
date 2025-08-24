
#pragma GCC optimize("O3,unroll-loops,no-stack-protector,omit-frame-pointer")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt,fma,sse3,sse4.1,sse4.2,tzcnt")
#include <bits/stdc++.h>
#undef _GLIBCXX_DEBUG

using namespace std;

const int kMaxN = 3000 + 5;

int           n;
long long     ans = 0;
bitset<kMaxN> color[kMaxN];

signed main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            char in_char;
            cin >> in_char;
            color[i][j] = in_char - '0';
        }
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = i + 1; j <= n; ++j)
        {
            long long cnt = (color[i] & color[j]).count();
            ans += cnt * (cnt - 1) / 2;
        }
    }
    cout << ans << "\n";
    return 0;
}
