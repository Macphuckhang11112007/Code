#include <bits/stdc++.h>

using namespace std;

int tc, n, k;
double ans;
double dp[210][210];

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin >> tc;
	while (tc--) {
		ans = 0;
		cin >> n >> k;
		for (int i = 0; i <= n; i++) {
			dp[0][i] = 1;
		}
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j < i; j++) {
				// if (i == j) {
				// 	dp[i][j] = 1.0 / 
				// 	continue;
				// }
				dp[i][j] = dp[i - 1][j] + (!j ? 0 : dp[i][j - 1] * 1.0 / (1.0 * k * (k - j)));
			}
		}
		cout << fixed << setprecision(9) << dp[n] << "\n";
	}
	return 0;
}