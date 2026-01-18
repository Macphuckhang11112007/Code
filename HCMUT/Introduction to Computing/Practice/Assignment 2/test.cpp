#include <iostream>
#include <string>
#define sz(x) (int)(x.size())
using namespace std;
string a[24] = {"ai", "ae", "ao", "au", "ei", "eu", "iu", "oi", "ou", "ui", "a", "e", "i", "o", "u", "p", "k", "h", "l", "m", "n", "w", " ", "\'"}, a_rep[24] = {"eye", "eye", "ow", "ow", "ay", "eh-oo", "ew", "oy", "ow", "ooey", "ah", "eh", "ee", "oh", "oo", "p", "k", "h", "l", "m", "n", "w", " ", "\'"}, s, str;

int main() {
    while (getline(cin, s)) {
        str     = s;
        bool ok = 1;
        for (int i = 0; ok && i < sz(s); i++) {
            string c = {char(tolower(s[i]))};
            if (string(" \'").find(c) == string::npos) {
                ok = 0;
                for (string ch : a)
                    if (c == ch)
                        ok = 1;
            }
        }
        if (!ok)
            cout << str << " contains invalid characters.";
        else
            for (int i = 0, p = -1; i < sz(s); i++, p = -1) {
                string g = s.substr(i, 2), c = {char(tolower(s[i]))};
                g[0] = tolower(g[0]);
                for (int id = 0; p == -1 && id < 24; id++)
                    if (g == a[id] || c == a[id])
                        p = id, i += (id <= 9);
                cout << (c == "w" && i && string("ie").find(s[i - 1]) != string::npos ? "v" : a_rep[p]) + (p <= 14 && i < sz(s) - 1 && string(" \'").find(s[i + 1]) == string::npos ? "-" : "");
            }
        cout << "\n";
    }
}