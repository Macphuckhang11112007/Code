// MPK

#include <bits/stdc++.h>

using namespace std;

string str;
int    ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    for (char &ch : str)
    {
        if (isdigit(ch)) { ans += ch - '0'; }
    }
    cout << ans << "\n";
    return 0;
}
