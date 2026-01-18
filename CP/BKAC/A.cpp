#include <bits/stdc++.h>

using namespace std;

const int N = 100;

int tc;
int n;
int a[N + 1][N + 1];
int ans;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        ans = 1e9;
        cin >> n;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                char c;
                cin >> c;
                a[i][j] = (c == '#');
            }
        }
        int tmp = 0;
        for (int i = 1; i <= n; i++) {
            int ok = 0;
            for (int j = 1; !ok && j <= n; j++) {
                ok |= a[i][j];
            }
            tmp += ok;
        }
        ans = min(ans, tmp);
        tmp = 0;
        for (int j = 1; j <= n; j++) {
            int ok = 0;
            for (int i = 1; !ok && i <= n; i++) {
                ok |= a[i][j];
            }
            tmp += ok;
        }
        ans = min(ans, tmp);
        cout << ans << "\n";
    }
    return 0;
}