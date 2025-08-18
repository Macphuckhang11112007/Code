// MPK
// Finish Time: 8/17/2025, 3:56:26 PM AC
// Start Time: 8/17/2025, 3:38:28 PM
// Start Time: 8/17/2025, 1:20:50 PM

#include <bits/stdc++.h>

using namespace std;

const int N = 30;

int       t, n, c, ans;
long long w;
int       a[N + 5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        ans = INT_MAX / 2;
        cin >> n >> c;
        for (int i = 1; i <= n; i++) { cin >> a[i]; }
        sort(a + 1, a + n + 1);
        for (int i = 1; i <= n; i++)
        {
            int tmp_ans = 0;
            w           = 1;
            for (int j = i; j >= 1; j--)
            {
                long long s = a[j] * w;
                tmp_ans += s > c;
                w *= 2;
            }
            for (int j = i + 1; j <= n; j++)
            {
                long long s = a[j] * w;
                tmp_ans += s > c;
                w *= 2;
            }
            ans = min(ans, tmp_ans);
        }
        cout << ans << "\n";
    }
    return 0;
}