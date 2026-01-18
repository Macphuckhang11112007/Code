#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int MOD = 1e9 + 7, INV = 5e8 + 4;
const int N = 5e5, MAX = 5e5;

int         n, q, a[N + 5];
vector<int> divs[MAX + 5];
int         ans, sum[MAX + 5];

int add(int x, int y) {
    int res = 0;
    if (y < 0) { y += MOD; }
    res = x + y;
    if (res >= MOD) { res -= MOD; }
    return res;
}

void update(int x, int type) {
    if (type == 1) {
        for (int d : divs[x]) {
            int p  = x / d;
            ans    = add(ans, 1LL * p * sum[d] % MOD);
            sum[d] = add(sum[d], p);
        }
    } else {
        for (int d : divs[x]) {
            int p  = x / d;
            sum[d] = add(sum[d], -p);
            ans    = add(ans, -1LL * p * sum[d] % MOD);
        }
    }
}

int main() {
    freopen("gcds.inp", "r", stdin);
    freopen("gcds.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    for (int i = 1; i <= MAX; i++) {
        for (int j = i; j <= MAX; j += i) { divs[j].push_back(i); }
    }
    cin >> n >> q;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        update(a[i], 1);
    }
    cout << ans << "\n";
    while (q--) {
        int i, x;
        cin >> i >> x;
        update(a[i], -1);
        update(x, 1);
        a[i] = x;
        cout << ans << "\n";
    }
}
