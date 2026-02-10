#include <bits/stdc++.h>

using namespace std;

typedef long long ll;


const ll MOD = 1e9 + 7, N = 1000;

ll tc, n, k, fact[N + 5], invFact[N + 5];

ll fastPow(ll x, ll y) {
    ll res = 1;
    x %= MOD;
    while (y) {
        if (y & 1) {
            res = res * x % MOD;
        }
        x = x * x % MOD;
        y >>= 1;
    }
    return res;
}

ll C(ll x, ll y) {
    if (y < 0 || y > x) {
        return 0;
    }
    return fact[x] * invFact[y] % MOD * invFact[x - y] % MOD;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    fact[0] = 1;
    for (int i = 1; i <= N; i++) {
        fact[i] = fact[i - 1] * i % MOD;
    }
    invFact[N] = fastPow(fact[N], MOD - 2);
    for (int i = N - 1; i >= 0; i--) {
        invFact[i] = invFact[i + 1] * (i + 1) % MOD;
    }
    cin >> tc;
    while (tc--) {
        cin >> n >> k;
        cout << C(n, k) << "\n";
    }
}
