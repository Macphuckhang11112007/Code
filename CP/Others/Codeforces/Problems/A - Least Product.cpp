// MPK
// Finish Time: 8/17/2025, 5:02:10 PM AC
// Finish Time: 8/17/2025, 5:01:14 PM WA
// Finish Time: 8/17/2025, 4:57:33 PM WA
// Start Time: 8/17/2025, 4:26:47 PM

#include <bits/stdc++.h>

using namespace std;

const int N = 100;

int t, n, cnt_neg, a[N + 5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cnt_neg = 0;
        cin >> n;
        for (int i = 1; i <= n; i++)
        {
            cin >> a[i];
            cnt_neg += a[i] < 0;
        }
        if (find(a + 1, a + n + 1, 0) != a + n + 1) { cout << "0\n"; }
        else if (cnt_neg)
        {
            if (cnt_neg & 1) { cout << "0\n"; }
            else { cout << "1\n1 0\n"; }
        }
        else { cout << "1\n1 0\n"; }
    }
    return 0;
}