#include <bits/stdc++.h>

using namespace std;

const int MAXN = 3e4 + 5;

int n, a [MAXN], t;

bool BFS ()
{
    queue <int> q; q.push (1);
    while (!q.empty ())
    {
        int i = q.front (); q.pop ();
        int j = i + a [i];
        if (j == t) return 1;
        else if (j < n) q.push (j);
    }
    return 0;
}

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> n >> t;
    for (int i = 1; i < n; i++) cin >> a [i];
    cout << (BFS () ? "YES" : "NO");
    return 0;
}