#include <bits/stdc++.h>

using namespace std;

int                   n;
map<int, vector<int>> id;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        int x;
        cin >> x;
        id[x].push_back(i);
    }
    for (const auto& [val, idx_vec] : id)
    {
        cout << val << '\n';
        for (const auto& idx : idx_vec) cout << idx << ' ';
        cout << '\n';
    }
    return 0;
}
