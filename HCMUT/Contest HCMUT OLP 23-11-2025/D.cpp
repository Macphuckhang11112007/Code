#include <bits/stdc++.h>

using namespace std;
const int          MOD = 998244353;
int                n, m, tc, a[100][100];
long long          dp[100][100];
multiset<set<int>> s[100][100];

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        cin >> n >> m;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                dp[i][j] = 0;
                s[i][j].clear();
                cin >> a[i][j];
            }
        }
        dp[1][1] = 1;
        s[1][1]  = {{a[1][1]}};
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (i == 1 && j == 1) {
                    continue;
                }
                for (auto x : s[i - 1][j]) {
                    set<int> tmp;
                    tmp.insert(a[i][j]);
                    for (auto val : x) {
                        tmp.insert(val);
                    }
                    s[i][j].insert(tmp);
                    (dp[i][j] += tmp.size()) %= MOD;
                }
                for (auto x : s[i - 1][j]) {
                    set<int> tmp;
                    tmp.insert(a[i][j]);
                    for (auto val : x) {
                        tmp.insert(val);
                    }
                    s[i][j].insert(tmp);
                    (dp[i][j] += tmp.size()) %= MOD;
                }
            }
        }
        cout << dp[n][m] << "\n";
    }
}