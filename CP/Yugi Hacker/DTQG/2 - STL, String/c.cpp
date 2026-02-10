#include <bits/stdc++.h>

using namespace std;

int n;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--) {
        stringstream ss;
        string       s, str;
        getline(cin >> ws, str);
        ss << str;
        while (ss >> s) {
            s[0] = toupper(s[0]);
            for (int i = 1; i < int(s.size()); i++) {
                s[i] = tolower(s[i]);
            }
            cout << s << " ";
        }
        cout << "\n";
    }
}
