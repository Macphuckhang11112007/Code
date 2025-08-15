// MPK

#include <bits/stdc++.h>

using namespace std;

string str;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    for (char &ch : str) { ch = tolower(ch); }
    cout << str << "\n";
    return 0;
}
