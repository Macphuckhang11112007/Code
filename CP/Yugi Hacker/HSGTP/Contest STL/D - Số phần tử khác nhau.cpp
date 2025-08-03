// MPK

#include <bits/stdc++.h>

using namespace std;

int      n;
set<int> num;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--)
    {
        int val;
        cin >> val;
        num.insert(val);
    }
    cout << num.size() << '\n';
    return 0;
}
