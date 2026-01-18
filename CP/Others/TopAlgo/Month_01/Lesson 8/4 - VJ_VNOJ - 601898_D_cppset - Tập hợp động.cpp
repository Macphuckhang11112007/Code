#include <bits/stdc++.h>

using namespace std;

int      opt, x;
set<int> sett;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    while (cin >> opt && opt)
    {
        if (opt != 3 && opt != 4) { cin >> x; }
        if (opt == 1) { sett.insert(x); }
        else if (opt == 2)
        {
            auto p = sett.find(x);
            if (p != sett.end()) { sett.erase(x); }
        }
        else if (!sett.empty())
        {
            if (opt == 3) { cout << *sett.begin(); }
            else if (opt == 4) { cout << *sett.rbegin(); }
            else if (opt == 5)
            {
                auto p = sett.upper_bound(x);
                if (p != sett.end()) { cout << *p; }
                else { cout << "no"; }
            }
            else if (opt == 6)
            {
                auto p = sett.lower_bound(x);
                if (p != sett.end()) { cout << *p; }
                else { cout << "no"; }
            }
            else if (opt == 7)
            {
                auto p = sett.lower_bound(x);
                if (p != sett.begin())
                {
                    --p;
                    cout << *p;
                }
                else { cout << "no"; }
            }
            else if (opt == 8)
            {
                auto p = sett.upper_bound(x);
                if (p != sett.begin())
                {
                    --p;
                    cout << *p;
                }
                else { cout << "no"; }
            }
        }
        else { cout << "empty"; }
        if (opt != 1 && opt != 2) { cout << "\n"; }
    }
    return 0;
}
