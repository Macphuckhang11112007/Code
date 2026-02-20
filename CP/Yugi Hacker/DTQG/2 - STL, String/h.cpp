#include <bits/stdc++.h>

using namespace std;

int              tc, n;
string           s;
map<string, int> mp = {{"I", 1}, {"V", 5}, {"X", 10}, {"L", 50}, {"C", 100}, {"D", 500}, {"M", 1000}, {"IV", 4}, {"IX", 9}, {"XL", 40}, {"XC", 90}, {"CD", 400}, {"CM", 900}};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        int res = 0;
        cin >> n >> s;
        for (int i = 0; i < int(s.size()); i++) {
            string str;
            str = s.substr(i, 2);
            if (mp.count(str)) {
                res += mp[str];
                i++;
            } else {
                str = s.substr(i, 1);
                res += mp[str];
            }
        }
        cout << res << "\n";
    }
}
