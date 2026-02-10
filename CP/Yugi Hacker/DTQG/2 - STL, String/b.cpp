#include <bits/stdc++.h>

using namespace std;

string str;
map<char, int> mp;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> str;
    for (char ch : str) {
        mp[ch]++;
    }
    for (auto [ch, val] : mp) {
        cout << ch << " " << val << "\n";
    }
}
