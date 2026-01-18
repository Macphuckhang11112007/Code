// MPK

#include <bits/stdc++.h>

using namespace std;

string str;
int    ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    stringstream ss(str);
    while (ss >> str) { ++ans; }
    cout << ans << "\n";
    return 0;
}
