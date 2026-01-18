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
        ch = tolower(ch);
        if (ch == 'u' || ch == 'e' || ch == 'o' || ch == 'a' || ch == 'i')
        {
            ++ans;
        }
    }
    cout << ans << "\n";
    return 0;
}
