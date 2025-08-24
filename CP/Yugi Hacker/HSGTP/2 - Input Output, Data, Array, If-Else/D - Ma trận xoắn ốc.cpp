// MPK

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int kMaxN = 105;

int n;
int a[kMaxN][kMaxN];
pii dir[] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

void BFS()
{
    int        id = 0;
    queue<pii> q;
    q.push({1, 1});
    a[1][1] = 1;
    while (!q.empty())
    {
        pii u = q.front();
        q.pop();
        int cur_id = id;
        while (1)
        {
            pii v = {u.fi + dir[id].fi, u.se + dir[id].se};
            if (v.fi >= 1 && v.fi <= n && v.se >= 1 && v.se <= n &&
                !a[v.fi][v.se])
            {
                q.push(v);
                a[v.fi][v.se] = a[u.fi][u.se] + 1;
                break;
            }
            ++id;
            if (id >= 4) { id -= 4; }
            if (cur_id == id) { return; }
        }
    }
    return;
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    BFS();
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j) { cout << a[i][j] << ' '; }
        cout << '\n';
    }
    return 0;
}
