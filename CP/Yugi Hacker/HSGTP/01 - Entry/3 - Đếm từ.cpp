#include <bits/stdc++.h>

using namespace std;

int    n, ans;
string in_str;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--)
    {
        ans = 0;
        getline(cin >> ws, in_str);
        stringstream ss(in_str);
        while (ss >> in_str) { ++ans; }
        cout << ans << '\n';
    }
    return 0;
}
