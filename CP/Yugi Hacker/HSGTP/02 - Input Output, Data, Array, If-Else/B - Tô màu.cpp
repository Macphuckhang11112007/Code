// MPK

#include <bits/stdc++.h>

using namespace std;

int a, b, c, d;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> a >> b >> c >> d;
    if (a > b) { swap(a, b); }
    if (c > d) { swap(c, d); }
    cout << (b - a) + (d - c) - max(0, min(b, d) - max(a, c)) << '\n';
    return 0;
}
