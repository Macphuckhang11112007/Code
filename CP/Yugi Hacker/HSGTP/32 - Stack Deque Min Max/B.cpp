#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 1e6;

int        n, k, a[N + 5];
deque<int> dq;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    for (int i = 1; i <= n; i++) {
        while (dq.size() && dq.front() < i - k + 1) { dq.pop_front(); }
        while (dq.size() && a[dq.back()] >= a[i]) { dq.pop_back(); }
        dq.push_back(i);
        if (i >= k) { cout << a[dq.front()] << " "; }
    }
}
