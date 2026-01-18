// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 13 + 5;

int n, a[kMaxN];

void BT(int i)
{
    if (i > n)
    {
        for (int j = 1; j <= n; ++j) { cout << a[j]; }
        cout << '\n';
        return;
    }
    else
    {
        for (int j = 0; j < 3; ++j)
        {
            a[i] = j;
            BT(i + 1);
        }
    }
    return;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    BT(1);
    return 0;
}
