#include <bits/stdc++.h>

using namespace std;

int n;
map<string, int> cnt_map;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        string in_str;
        cin >> in_str;
        if (cnt_map[in_str])
        {
            cout << in_str << cnt_map[in_str] << "\n";
        }
        else
        {
            cout << "OK\n";
        }
        ++cnt_map[in_str];
    }
}
