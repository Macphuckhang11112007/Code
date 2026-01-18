// MPK

#include <bits/stdc++.h>

using namespace std;

int           n;
map<int, int> mapp;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--)
    {
        string opt;
        int    val;
        cin >> opt;
        if (opt == "add")
        {
            cin >> val;
            ++mapp[val];
        }
        else if (opt == "del")
        {
            cin >> val;
            auto p = mapp.find(val);
            if (p != mapp.end()) { mapp.erase(p); }
        }
        else if (opt == "count")
        {
            cin >> val;
            cout << (mapp.find(val) != mapp.end() ? mapp[val] : 0) << '\n';
        }
        else if (opt == "size") { cout << mapp.size() << '\n'; }
    }
    return 0;
}
