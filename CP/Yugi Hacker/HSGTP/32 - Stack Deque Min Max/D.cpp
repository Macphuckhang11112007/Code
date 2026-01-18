#include <bits/stdc++.h>

using namespace std;

const int N = 1e6;

int        n, q, a[N + 5];
deque<int> dq;
int        dp[N + 5];

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    dq.push_front(n + 1);
    for (int i = n; i; i--) {
        while (dq.size() && a[dq.front()] <= a[i]) { dq.pop_front(); }
        dp[i] = dp[dq.front()] + 1;
        dq.push_front(i);
    }
    while (q--) {
        int i;
        cin >> i;
        cout << dp[i] << "\n";
    }
}
