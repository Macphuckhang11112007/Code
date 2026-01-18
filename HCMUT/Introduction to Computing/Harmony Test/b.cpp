#include <bits/stdc++.h>
using namespace std;
string s, c, ch, t, f, g, a[] = {"p", "k", "h", "l", "m", "n", "w", "ai", "ae", "ao", "au", "ei", "eu", "iu", "oi", "ou", "ui", "a", "e", "i", "o", "u"};
int    ans;
int main() {
    getline(cin, s);
    for (size_t i = 0, ok = 1; (ok ? !(ok = 0) : !(ans = -1)) && i < s.size() && (c{s[i] = tolower(s[i])}); i++) for (ch : a) ok |= (c == ch || c == " " || c == "\'");
    for (size_t i = 0; ans != -1 && i < s.size() && (c = s.substr(i, 1)) && (t = s.substr(i, 2)) && (f = s.substr(i + 1, 1)) && (g = s.substr(i + 1, 2)); i++) if (c != " " && c != "\'" && ++ans) for (int cnt = 0, id = 0; id < 22 && (ch = a[id]); cnt += (id <= 6 && c == ch), id++) if (cnt == 1 && g.size() == 2 && g == ch) i += 2, cnt++; else if (cnt == 1 && f.size() == 1 && f == ch) i++, cnt++; else if (cnt == 0 && id > 6 && t.size() == 2 && t == ch) i++, cnt++;
    cout << ans;
}