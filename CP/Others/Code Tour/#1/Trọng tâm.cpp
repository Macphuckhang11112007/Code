#include <bits/stdc++.h>

using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> x[i] >> y[i];
    }
    for (int i = 1; i <= n; i++) {
        (dp[(x[i] + y[i]) % k][((x[i] - y[i]) % k + k) % k]++) %= MOD;
        (dp[0][0] += dp[(x[i] + y[i]) % k][((x[i] - y[i]) % k + k) % k]) %= MOD;
    }
}