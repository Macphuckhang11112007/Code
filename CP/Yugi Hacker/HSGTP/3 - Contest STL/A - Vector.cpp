// MPK

#include <bits/stdc++.h>

using namespace std;

int         n;
vector<int> arr;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--)
    {
        string opt;
        int    val;
        cin >> opt;
        if (opt == "push")
        {
            cin >> val;
            arr.push_back(val);
        }
        else if (opt == "pop")
        {
            if (!arr.empty()) { arr.pop_back(); }
        }
        else if (opt == "index")
        {
            cin >> val;
            --val;
            cout << (val < arr.size() ? arr[val] : -1) << '\n';
        }
        else if (opt == "size") { cout << arr.size() << '\n'; }
    }
    return 0;
}
