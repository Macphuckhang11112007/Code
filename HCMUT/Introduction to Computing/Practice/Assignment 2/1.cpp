#include <bits/stdc++.h>
using namespace std;
vector<string> a = {"ai", "ae", "ao", "au", "ei", "eu", "iu", "oi", "ou", "ui", "a", "e", "i", "o", "u", "p", "k", "h", "l", "m", "n", "w", " ", "\'"}, a_rep = {"eye", "eye", "ow", "ow", "ay", "eh-oo", "ew", "oy", "ow", "ooey", "ah", "eh", "ee", "oh", "oo", "p", "k", "h", "l", "m", "n", "w", " ", "\'"};
string         s, str;

int main() {
    getline(cin, s);
    str = s;
    if (all_of(s.begin(), s.end(), [&](char &c) { return a.find(string{c = tolower(c)}) != a.end(); }))
        cout << str << " contains invalid characters.";
    else
        for (int i = 0, p = -1; i < s.size(); i++, p = -1) {
            string g = s.substr(i, 2), c = {s[i]};
            for (int id = 0; p == -1 && id < 24; id++)
                if (g == a[id] || c == a[id])
                    p = id, i += (id <= 9);
            cout << (c == "w" && i && string("ie").find(s[i - 1]) != string::npos ? "v" : a_rep[p]) + (p <= 14 && i + 1 < s.size() && string(" \'").find(s[i + 1]) == string::npos ? "-" : "");
        }
}