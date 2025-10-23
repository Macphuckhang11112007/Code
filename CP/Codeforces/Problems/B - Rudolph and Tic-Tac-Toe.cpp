// MPK
// Finish Time: 8/19/2025, 11:43:35 AM AC
// Finish Time: 8/19/2025, 11:43:13 AM WA
// Finish Time: 8/19/2025, 11:39:14 AM WA
// Start Time: 8/19/2025, 10:52:10 AM

#include <bits/stdc++.h>

using namespace std;

int  t;
int  dx[] = {0, 0, 1, 1, 1}, dy[] = {0, 1, 1, 0, -1};
char a[3 + 1][3 + 1];

bool in(int i, int j) { return (1 <= i && i <= 3 && 1 <= j && j <= 3); }

int DFS(int i, int j, char s, int id)
{
    int res = 0;
    if (!in(i, j) || a[i][j] != s) { return 0; }
    if (!id)
    {
        for (int k = 1; k <= 4; k++)
        {
            int x = i + dx[k], y = j + dy[k];
            res = max(res, DFS(x, y, s, k) + 1);
        }
    }
    else
    {
        int x = i + dx[id], y = j + dy[id];
        return DFS(x, y, s, id) + 1;
    }
    return res;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> t;
    while (t--)
    {
        for (int i = 1; i <= 3; i++)
        {
            for (int j = 1; j <= 3; j++) { cin >> a[i][j]; }
        }
        bool ok = 0;
        for (int i = 1; i <= 3 && !ok; i++)
        {
            for (int j = 1; j <= 3 && !ok; j++)
            {
                if (a[i][j] != '.')
                {
                    int res = DFS(i, j, a[i][j], 0);
                    if (res >= 3)
                    {
                        ok = 1;
                        cout << a[i][j] << "\n";
                    }
                }
            }
        }
        if (!ok) { cout << "DRAW\n"; }
    }
    return 0;
}