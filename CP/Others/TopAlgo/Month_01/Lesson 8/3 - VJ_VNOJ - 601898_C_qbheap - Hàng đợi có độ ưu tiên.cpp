#include <bits/stdc++.h>

using namespace std;

struct comp
{
    bool operator()(int _i, int _j) const { return _i > _j; }
};

string              str;
int                 sz = 0;
map<int, int, comp> mapp;

bool is_num(char ch) { return ('0' <= ch && ch <= '9'); }

int ctoi(char ch) { return ch - '0'; }

int strtoi(string _str)
{
    int _id = 0;
    int _x  = 0;
    while (_id < _str.size() && !is_num(_str[_id])) { ++_id; }
    while (_id < _str.size() && is_num(_str[_id]))
    {
        (_x *= 10) += ctoi(_str[_id]);
        ++_id;
    }
    return _x;
}

int main()
{
    // cin.tie(0)->sync_with_stdio(0);
    while (getline(cin >> ws, str))
    {
        char ch = str[0];
        // cout << ch;
        if (ch == '+' && sz < 15'000)
        {
            int x = strtoi(str);
            ++sz;
            ++mapp[x];
            // cout << " " << x << "\n";
        }
        else if (ch == '-' && sz)
        {
            auto p = mapp.begin();
            sz -= p->second;
            mapp.erase(p);
        }
    }
    cout << mapp.size() << "\n";
    for (auto [key, val] : mapp) { cout << key << "\n"; }
    return 0;
}
