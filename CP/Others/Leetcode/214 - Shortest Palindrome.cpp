// MPK

#include <bits/stdc++.h>

using namespace std;

const int N = 5e4;

string s, rev_s, str, sub_str;
int    n, len, lps[2 * N + 5];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> s;
    s     = " " + s;
    rev_s = s;
    reverse(rev_s.begin(), rev_s.end());
    str = s + "#" + rev_s;
    n   = str.size() - 2;
    for (int i = 2; i <= n; ++i)
    {
        char c = str[i];
        while (len && c != str[len + 1]) { len = lps[len]; }
        if (c == str[len + 1]) { ++len; }
        lps[i] = len;
    }
    if (s.size() - 1 >= len + 1)
    {
        sub_str = s.substr(len + 1);
        reverse(sub_str.begin(), sub_str.end());
    }
    s.erase(s.begin());
    cout << sub_str << s << "\n";
    return 0;
}
