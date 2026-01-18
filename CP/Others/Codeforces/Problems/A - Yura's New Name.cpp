// MPK
// Finish Time: 8/18/2025, 8:44:31 PM AC
// Start Time: 8/18/2025, 8:00:42 PM

#include <bits/stdc++.h>

using namespace std;

int    t, n, ans;
string s;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        ans = 0;
        vector<char> prev_chars(2 + 1, '_');
        cin >> s;
        n = s.size() + 1;
        s = " " + s + "__";
        for (int i = 1; i <= n + (s.size() <= 4 && s[1] == '^'); i++)
        {
            char c = s[i];
            if (c == '_' && prev_chars[2] == '_')
            {
                ++ans;
                prev_chars[1] = '^';
                prev_chars[2] = c;
            }
            else { prev_chars[1] = prev_chars[2]; }
            prev_chars[2] = c;
        }
        cout << ans << "\n";
    }
    return 0;
}
