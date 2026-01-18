#include <bits/stdc++.h>

using namespace std;

int                  n, m;
vector<vector<char>> a;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    a.resize(n + 1, vector<char>(m + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            char c;
            cin >> c;
            a[i][j]   = c - '0';
            pre[i][j] = pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1] + a[i][j];
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            for (int k = 1; k <= 4; k++) {
                if (k == 1) {
                    col[i] += a[i][j];
                } else if (k == 2) {
                    row[j] += a[i][j];
                } else if (k == 3) {
                    diag1[i - j + 1] += a[i][j];
                } else if (k == 4) {
                    diag2[i + j - 1] += a[i][j];
                }
            }
        }
    }
    for (int i = m; i >= 1; i--) {
        dcol[i] = 10 * dcol[i + 1] + col[i];
    }
    for (int i = n; i >= 1; i--) {
        drow[i] = 10 * drow[i + 1] + row[i];
    }
    for (int idx = n - m + 1; idx >= 2 - m; idx--) {}
    for (int idx = n - m + 1; idx >= 2 - m; idx--) {}
    for (int i = 1; i <= m; i++) {
        srow[i] = 10 * srow[i - 1] + pre[n][m] - pre[n][i - 1] - 10 * drow[i];
    }
    for (int i = 1; i <= n; i++) {
        scol[i] = 10 * scol[i - 1] + pre[n][m] - pre[i - 1][m] - 10 * dcol[i];
    }
    for (int i = 1; i <= min(n, m); i++) {
        srow[i] = 10 * srow[i - 1] + pre[n][m] - pre[n][i - 1] - 10 * drow[i];
    }
    for (int i = 1; i <= min(n, m); i++) {
        srow[i] = 10 * srow[i - 1] + pre[n][m] - pre[n][i - 1] - 10 * drow[i];
    }
    for (int i = 1; i <= min(n, m);)
}