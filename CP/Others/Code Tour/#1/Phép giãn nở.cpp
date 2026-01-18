#include <bits/stdc++.h>

#define pii pair<int, int>
#define fi first
#define se second

using namespace std;

const int N = 1000;

int        n, m, k;
bool       a[N + 1][N + 1], b[N + 1][N + 1], vst[N + 1][N + 1];
int        dx[] = {0, 1, 1, 1, 0, -1, -1, -1}, dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
queue<pii> q;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> a[i][j];
            if (a[i][j]) {
                vst[i][j] = 1;
                b[i][j]   = 1;
                q.push({i, j});
            }
        }
    }
    while (k-- && q.size()) {
        int sz = q.size();
        while (sz--) {
            pii u = q.front();
            q.pop();
            for (int i = 0; i < 8; i++) {
                pii v = {u.fi + dx[i], u.se + dy[i]};
                if (1 <= v.fi && v.fi <= n && 1 <= v.se && v.se <= m && !vst[v.fi][v.se]) {
                    vst[v.fi][v.se] = 1;
                    b[v.fi][v.se]   = 1;
                    q.push(v);
                }
            }
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cout << b[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}