#include <bits/stdc++.h>

using namespace std;

const int N = 1e6;

int        n, a[N + 5];
deque<int> dq;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        while (dq.size() && a[dq.back()] <= a[i]) { dq.pop_back(); }
        cout << (dq.size() ? dq.back() : -1) << " ";
        dq.push_back(i);
    }
}
