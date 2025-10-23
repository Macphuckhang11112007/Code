#include <bits/stdc++.h>

using namespace std;

const int N = 1e6, MOD = 1e9 + 7;

int n, k;
int dp[N + 5];
bitset<N + 5> a;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= k; i++) {
        int x;
        cin >> x;
        a[x] = 1;
    }
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!a[i]) {
            dp[i] = (dp[i - 1] + dp[i - 2]) % MOD;
        }
    }
    cout << dp[n];
    return 0;
}