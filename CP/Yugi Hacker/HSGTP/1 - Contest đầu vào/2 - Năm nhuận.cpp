#include <bits/stdc++.h>

using namespace std;

int n;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    if (n % 4 == 0)
    {
        if (n % 100 == 0)
        {
            if (n % 400 == 0) { cout << "Yes\n"; }
            else { cout << "No\n"; }
        }
        else { cout << "Yes\n"; }
    }
    else { cout << "No\n"; }
    return 0;
}
