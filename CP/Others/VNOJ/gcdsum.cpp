#include <bits/stdc++.h>

using namespace std;

const int N = 1e6;

using ll = long long;

int         n, spf[N + 5], phi[N + 5];
long long   sp[N + 5];
vector<int> pr;

ll solve() {
    ll          res = 0;
    vector<int> a   = {0};
    for (int i = 1; i * i <= n; i++) {
        a.push_back(i);
        if (n / i != i) { a.push_back(n / i); }
    }
    sort(a.begin(), a.end());
    for (int i = 1; i < a.size(); i++) { res += sp[n / a[i]] * (a[i - 1] + a[i] + 1) * (a[i] - a[i - 1]) / 2; }
    return res;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 2; i <= N; i++) {
        if (!spf[i]) {
            spf[i] = i;
            phi[i] = i - 1;
            pr.push_back(i);
        }
        for (int j : pr) {
            if (j > spf[i] || 1LL * i * j > N) { break; }
            spf[i * j] = j;
            if (j == spf[i]) {
                phi[i * j] = phi[i] * j;
            } else {
                phi[i * j] = phi[i] * (j - 1);
            }
        }
    }
    for (int i = 2; i <= N; i++) { sp[i] = sp[i - 1] + phi[i]; }
    while (cin >> n && n) { cout << solve() << "\n"; }
}
