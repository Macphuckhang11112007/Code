// MPK

#include <bits/stdc++.h>

using namespace std;

string str;
bool   ok[5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    for (const char &ch : str)
    {
        ok[1] |= isdigit(ch);
        ok[2] |= isupper(ch);
        ok[3] |= islower(ch);
    }
    ok[4] |= str.size() >= 8;
    cout << (accumulate(ok + 1, ok + 4 + 1, 0) >= 4 ? "STRONG\n" : "WEAK\n");
    return 0;
}
