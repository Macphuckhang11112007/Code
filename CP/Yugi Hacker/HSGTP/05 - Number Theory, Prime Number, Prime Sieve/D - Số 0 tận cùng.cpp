// MPK

#include <bits/stdc++.h>

using namespace std;

int n, ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n)
    {
        ans += n / 5;
        n /= 5;
    }
    cout << ans << '\n';
    return 0;
}
