#include <bits/stdc++.h>

using namespace std;

const int N = 2000;

int n, m, q, a[N + 5][N + 5], sumV[N + 5][N + 5], sumEV[N + 5][N + 5], sumEH[N + 5][N + 5];

int getSumV(int x1, int y1, int x2, int y2) {
    return sumV[x2][y2] - sumV[x2][y1 - 1] - sumV[x1 - 1][y2] + sumV[x1 - 1][y1 - 1];
}

int getSumEV(int x1, int y1, int x2, int y2) {
    return sumEV[x2][y2] - sumEV[x2][y1 - 1] - sumEV[x1][y2] + sumEV[x1][y1 - 1];
}

int getSumEH(int x1, int y1, int x2, int y2) {
    return sumEH[x2][y2] - sumEH[x2][y1] - sumEH[x1 - 1][y2] + sumEH[x1 - 1][y1];
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> q;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            char ch;
            cin >> ch;
            a[i][j]     = (ch - '0');
            sumV[i][j]  = sumV[i - 1][j] + sumV[i][j - 1] - sumV[i - 1][j - 1] + a[i][j];
            sumEV[i][j] = sumEV[i - 1][j] + sumEV[i][j - 1] - sumEV[i - 1][j - 1] + (a[i - 1][j] && a[i][j]);
            sumEH[i][j] = sumEH[i - 1][j] + sumEH[i][j - 1] - sumEH[i - 1][j - 1] + (a[i][j - 1] && a[i][j]);
        }
    }
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        int res = getSumV(x1, y1, x2, y2) - (getSumEV(x1, y1, x2, y2) + getSumEH(x1, y1, x2, y2));
        cout << res << "\n";
    }
}
