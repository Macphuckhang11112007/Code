#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int maxn = 2e6 + 5;

int n, m, id, sz = 0;
char ans[maxn];
string str;
set<pii> sett;
set<pii>::iterator p;

int low(int l = 0)
{
    if (sett.empty())
        return -1;
    p = sett.lower_bound({l, 0});
    if (p == sett.begin())
        return 1;
    else if (p != sett.end())
        return 2;
    return 0;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> str;
        cin >> m;
        for (int j = 1; j <= m; j++)
        {
            id = 0;
            int l, r, c;
            cin >> l;
            c = l;
            r = l + int(str.size()) - 1;
            sz = max(sz, r);
            pii p1, p2;
            p1 = p2 = {0, 0};
            int chk = low(c + 1), chk1 = 0, chk2 = 0;
            if (chk == 0 || chk == 2)
            {
                p--;
                if (c <= p->se + 1)
                {
                    id += p->se + 1 - c;
                    c = p->se + 1;
                    p1 = *p;
                    chk1 = 1;
                    sett.erase(p);
                }
            }
            while (c <= r)
            {
                chk = low(c);
                if (chk <= 0 || r <= p->se)
                    break;
                for (; c < p->fi; c++, id++)
                    ans[c] = str[id];
                id += p->se + 1 - c;
                c = p->se + 1;
                sett.erase(p);
            }
            chk = low(c);
            if (chk > 0 && p->fi - 1 <= r)
            {
                p2 = *p;
                chk2 = 1;
                for (; c < p->fi; c++, id++)
                    ans[c] = str[id];
                sett.erase(p);
            }
            else
                for (; c <= r; c++, id++)
                    ans[c] = str[id];
            if (chk1 && chk2)
                sett.insert({p1.fi, p2.se});
            else if (chk1)
                sett.insert({p1.fi, r});
            else if (chk2)
                sett.insert({l, p2.se});
            else
                sett.insert({l, r});
        }
    }
    for (int i = 1; i <= sz; i++)
        cout << (ans[i] ? ans[i] : 'a');
    return 0;
}
