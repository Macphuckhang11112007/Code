#include <bits/stdc++.h>

using namespace std;

const int N = 1e5;

int           n;
map<int, int> mp;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        string str;
        cin >> str;
        if (str == "add") {
            int x;
            cin >> x;
            mp[x]++;
        } else if (str == "del") {
            int x;
            cin >> x;
            if (mp.count(x)) {
                mp[x]--;
                if (mp[x] == 0) {
                    mp.erase(x);
                }
            }
        } else if (str == "count") {
            int x;
            cin >> x;
            cout << (mp.count(x) ? mp[x] : 0) << "\n";
        } else if (str == "size") {
            cout << mp.size() << "\n";
        }
    }
}
