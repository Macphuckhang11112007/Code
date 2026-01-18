#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const ll INF = 1e18;

int n;
ll cnt_pos, cnt_neg, cnt_0s, cnt_1s;
ll min_pos = 0, max_pos = 0, min_neg = 0, max_neg = 0, mn = LLONG_MAX, mx = LLONG_MIN; 

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin >> n;
	for (int i = 1; i <= n; i++) {
		ll x;
		cin >> x;
		mn = min(mn, x);
		mx = max(mx, x);
		if (x > 0) {
			min_pos = min(x, (!min_pos ? INF : min_pos));
			max_pos = max(x, max_pos);
			cnt_pos++;
			cnt_1s += (x == 1);
		} else if (x == 0) {
			cnt_0s++;
		} else if (x < 0) {
			min_neg = min(x, min_neg);
			max_neg = max(x, (!max_neg ? -INF : max_neg));
			cnt_neg++;
		}
	}
	if (!cnt_0s) {
		if(~cnt_neg & 1) {
			if(min_pos) cout << min_pos  << " " << min_pos << '\n';
			else cout << min_neg << " " << min_neg << '\n';
		}
		else {
			cout << max_neg << " " << max_neg << '\n';
		}
	}
	else if (cnt_0s == 1) {
		if(~cnt_neg & 1) {
			cout << "0 0\n";
		}
		else {
			cout << (max_pos ? max_pos : max_neg) << " " << min_neg << '\n';
		}
	}
	else {
		cout << mx << " " << mn << '\n';
	}
}