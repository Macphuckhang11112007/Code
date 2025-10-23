// MPK

#include <bits/stdc++.h>

using namespace std;

map<char, int> cnt;
string         str;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    getline(cin >> ws, str);
    for (char &ch : str) { ++cnt[ch]; }
    for (auto &[ch, count] : cnt) { cout << ch << ' ' << count << '\n'; }
    return 0;
}
