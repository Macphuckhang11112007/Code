#include <bits/stdc++.h>

using namespace std;

const int N = 1 << 9;

int n, cnt;
int a[N + 5][N + 5];

void solve(int c, int i, int j, int x, int y) {
    if (!c) {
        return;
    }
    int s = 1 << (c - 1), t = ++cnt;
    int u = i + s - 1, v = j + s - 1;
    for (int dx = 0; dx <= 1; dx++) {
        for (int dy = 0; dy <= 1; dy++) {
            int p = i + dx * s, q = j + dy * s;
            if (p <= x && x < p + s && q <= y && y < q + s)
                solve(c - 1, p, q, x, y);
            else {
                a[u + dx][v + dy] = t;
                solve(c - 1, p, q, u + dx, v + dy);
            }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    int x, y;
    cin >> x >> y;
    solve(n, 1, 1, x, y);
    for (int i = 1; i <= (1 << n); i++) {
        for (int j = 1; j <= (1 << n); j++) {
            cout << a[i][j] << " ";
        }
        cout << "\n";
    }
}
