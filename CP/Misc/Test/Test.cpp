// MPK

#pragma GCC optimize("Ofast,inline,unroll-loops,omit-frame-pointer")
#pragma GCC target("arch=x86-64")

#include <bits/stdc++.h>

using namespace std;

const int S = 1e5;

int                 n, m;
int                 sum, ans;
vector<int>         a, b, sumA, sumB;
vector<vector<int>> l;
vector<int>         posA[2 * S + 1], posB[2 * S + 1];

signed main() {
    cin.tie(0)->sync_with_stdio(0);
    freopen("Test.inp", "r", stdin);
    cin >> n >> m;
    a.resize(n + 1);
    b.resize(m + 1);
    sumA.resize(n + 1);
    sumB.resize(m + 1);
    l.resize(n + 1, vector<int>(m + 1));
    sum     = S;
    posA[S] = {0};
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        sum += a[i];
        sumA[i] = sum;
        posA[sum].emplace_back(i);
    }
    sum     = S;
    posB[S] = {0};
    for (int i = 1; i <= m; i++) {
        cin >> b[i];
        sum += b[i];
        sumB[i] = sum;
        posB[sum].emplace_back(i);
    }
    for (int id = 1 - m; id <= n - 1; id++) {
        for (int r2 = max(1, id + 1), c2 = max(1, 1 - id); r2 <= n && c2 <= m; r2++, c2++) {
            l[r2][c2] = (a[r2] * b[c2] == 1 ? l[r2 - 1][c2 - 1] + 1 : 0);


            int len = l[r2][c2];
            int r1  = r2 - len + 1;
            int c1  = c2 - len + 1;
            int lA, rA, lB, rB;
            lA = lower_bound(posA[sumA[r2]].begin(), posA[sumA[r2]].end(), r1 - 1) - posA[sumA[r2]].begin();
            rA = lower_bound(posA[sumA[r2]].begin(), posA[sumA[r2]].end(), r2) - posA[sumA[r2]].begin() - 1;
            lB = lower_bound(posB[sumB[c2]].begin(), posB[sumB[c2]].end(), c1 - 1) - posB[sumB[c2]].begin();
            rB = lower_bound(posB[sumB[c2]].begin(), posB[sumB[c2]].end(), c2) - posB[sumB[c2]].begin() - 1;
            // cout << r2 << " " << c2 << "    " << r1 << " " << c1 << "    " << lA << " " << rA << "    " << lB << " " << rB << "    ";
            while (lA <= rA && lB <= rB) {
                if (r2 - posA[sumA[r2]][lA] > c2 - posB[sumB[c2]][lB]) {
                    lA++;
                    ans++;
                } else if (r2 - posA[sumA[r2]][lA] < c2 - posB[sumB[c2]][lB]) {
                    lB++;
                    ans++;
                } else {
                    lA++;
                    lB++;
                    ans++;
                }
            }
            ans += max(0, rA - lA + 1);
            ans += max(0, rB - lB + 1);
            // cout << len << "    " << ans << "\n";
        }
    }
    cout << ans << "\n";
    return 0;
}