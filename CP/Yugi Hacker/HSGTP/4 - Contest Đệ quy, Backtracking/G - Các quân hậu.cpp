// MPK

#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int kN = 8 + 5, kState = 8 + 5;

int  ans;
char a[kN][kN];
int  vst[kN][kN][kState];
pii  dir[] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
              {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

void Push(pii u, int id, queue<pair<pii, int>> &q)
{
    if (1 <= u.fi && u.fi <= 8 && 1 <= u.se && u.se <= 8 &&
        !vst[u.fi][u.se][id])
    {
        vst[u.fi][u.se][id] = 1;
        ++vst[u.fi][u.se][8];
        q.push({u, id});
    }
    return;
}

vector<pair<pii, int>> Mark(int i, int j)
{
    queue<pair<pii, int>>  q;
    vector<pair<pii, int>> trace;
    for (int k = 0; k < 8; ++k) vst[i][j][k] = 1;
    ++vst[i][j][8];
    q.push({{i, j}, -1});
    while (!q.empty())
    {
        auto [u, id] = q.front();
        q.pop();
        trace.push_back({u, id});
        bool chk = (id == -1);
        for (int k = (chk ? 0 : id); k < 8; k += (chk ? 1 : 8))
        {
            pii v = {u.fi + dir[k].fi, u.se + dir[k].se};
            Push(v, k, q);
        }
    }
    return trace;
}

void Reset(const vector<pair<pii, int>> &trace)
{
    for (const pair<pii, int> &p : trace)
    {
        if (p.se != -1) { vst[p.fi.fi][p.fi.se][p.se] = 0; }
        else
        {
            for (int id = 0; id < 8; ++id) { vst[p.fi.fi][p.fi.se][id] = 0; }
        }
        --vst[p.fi.fi][p.fi.se][8];
    }
    return;
}

void BT(int i)
{
    if (i > 8)
    {
        ++ans;
        return;
    }
    else
    {
        for (int j = 1; j <= 8; ++j)
        {
            if (!vst[i][j][8] && a[i][j] == '.')
            {
                vector<pair<pii, int>> trace = Mark(i, j);
                BT(i + 1);
                Reset(trace);
            }
        }
    }
    return;
}

int main()
{
#ifndef CPH
    ifstream cin("Queens.inp");
    ofstream cout("Queens.out");
#endif
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 1; i <= 8; ++i)
    {
        string s;
        cin >> s;
        int j = 0;
        for (const char &c : s)
        {
            ++j;
            a[i][j] = c;
        }
    }
    BT(1);
    cout << ans;
    return 0;
}
