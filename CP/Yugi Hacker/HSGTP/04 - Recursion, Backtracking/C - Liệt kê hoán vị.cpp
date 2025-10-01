// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 10 + 5;

int  n, a[kMaxN];
bool vst[kMaxN];

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
        for (int j = 1; j <= n; ++j)
        {
            if (!vst[j])
            {
                vst[j] = 1;
                a[i]   = j;
                BT(i + 1);
                vst[j] = 0;
            }
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
