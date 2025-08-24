// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 20 + 5;

int            n, s;
char           a[kMaxN];
vector<string> ans;

void BT(int i, int cnt)
{
    if (i > 2 * n)
    {
        if (!cnt)
        {
            string str;
            for (int j = 1; j <= 2 * n; ++j) { str += a[j]; }
            ans.push_back(str);
        }
        return;
    }
    else
    {
        a[i] = '(';
        BT(i + 1, cnt + 1);
        if (cnt > 0)
        {
            a[i] = ')';
            BT(i + 1, cnt - 1);
        }
    }
    return;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    BT(1, 0);
    cout << ans.size() << '\n';
    for (const string &str : ans) { cout << str << '\n'; }
    return 0;
}
