#include <bits/stdc++.h>

using namespace std;

const int N = 1e6;

int        n, p, a, b, r, ans;
bitset<N>  vis;
queue<int> q;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> p >> a >> b >> r;
    if (n < p) { vis[n] = 1; }
    n %= p;
    q.push(n);
    while (q.size()) {
        int sz = q.size();
        while (sz--) {
            int u = q.front();
            q.pop();
            if (u == r && ans) {
                cout << ans;
                return 0;
            }
            int x = u + a, y = u + b, z = u + a + b;
            if (x >= p) { x -= p; }
            if (y >= p) { y -= p; }
            if (z >= p) { z -= p; }
            if (z >= p) { z -= p; }
            if (!vis[x]) {
                vis[x] = 1;
                q.push(x);
            }
            if (!vis[y]) {
                vis[y] = 1;
                q.push(y);
            }
            if (!vis[z]) {
                vis[z] = 1;
                q.push(z);
            }
        }
        ans++;
    }
    cout << -1;
}
