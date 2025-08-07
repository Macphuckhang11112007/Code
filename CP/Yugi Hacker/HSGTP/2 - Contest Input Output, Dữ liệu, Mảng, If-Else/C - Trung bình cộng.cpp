// MPK

#include <bits/stdc++.h>

using namespace std;

int                cnt;
long long          x;
unsigned long long ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    while (cin >> x)
    {
        ans += x;
        ++cnt;
    }
    cout << fixed << setprecision(6) << 1.0L * ans / cnt;
    return 0;
}
