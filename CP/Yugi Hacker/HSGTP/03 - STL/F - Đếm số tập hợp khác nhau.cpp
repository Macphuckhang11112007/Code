// MPK

#include <bits/stdc++.h>

using namespace std;

int           n, m;
set<set<int>> sett;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        long long tmp_ans = LLONG_MAX / 2;
        set<int>  num;
        for (int j = 1; j <= m; ++j)
        {
            int val;
            cin >> val;
            num.insert(val);
        }
        sett.insert(num);
    }
    cout << sett.size() << '\n';
    return 0;
}
