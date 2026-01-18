// MPK
// Finish Time: 8/17/2025, 5:46:19 PM AC
// Start Time: 8/17/2025, 5:26:44 PM

#include <bits/stdc++.h>

using namespace std;

int    t, n;
string a;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        cin >> a;
        n     = stoi(a);
        int s = sqrt(n);
        if (s * s == n) { cout << "0 " << s << "\n"; }
        else { cout << "-1\n"; }
    }
    return 0;
}