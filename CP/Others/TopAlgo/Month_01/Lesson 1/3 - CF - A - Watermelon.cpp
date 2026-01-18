#include <bits/stdc++.h>

using namespace std;

int w;

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> w;
    if (w % 2 == 0 && w > 2) cout << "YES";
    else cout << "NO";
    return 0;
}