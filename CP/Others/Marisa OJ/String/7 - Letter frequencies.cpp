// MPK

#include <bits/stdc++.h>

using namespace std;

string str;
int    cnt['z' - 'a' + 1];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    for (const char &ch : str)
    {
        if (isalpha(ch)) { ++cnt[ch - 'a']; }
    }
    for (const int &x : cnt) { cout << x << " "; }
    return 0;
}
