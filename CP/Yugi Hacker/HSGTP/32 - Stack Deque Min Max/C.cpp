#include <bits/stdc++.h>

using namespace std;

const int N = 1e6;

int        n, a[N + 5];
deque<int> dq;
int        s1[N + 5], s2[N + 5], len[N + 5];
int        ans;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    dq.push_back(0);
    cin >> n;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    for (int i = 1; i <= n; i++) {
        while (dq.size() && a[dq.back()] >= a[i]) { dq.pop_back(); }
        s1[i] = dq.back() + 1;
        dq.push_back(i);
    }
    dq.clear();
    dq.push_back(n + 1);
    for (int i = n; i; i--) {
        while (dq.size() && a[dq.front()] >= a[i]) { dq.pop_front(); }
        s2[i] = dq.front() - 1;
        dq.push_front(i);
    }
    for (int i = 1; i <= n; i++) { len[i] = min(a[i], (i - s1[i]) + (s2[i] - i) + 1); }
    ans = *max_element(len + 1, len + n + 1);
    cout << ans;
}
