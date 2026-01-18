#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int               tc;
ll                a;

void solve() {
    cin >> a;
    if (a < 0LL) {
        cout << 1LL << ' ' << abs(a) << '\n';
    } else if (a == 0) {
        cout << 1LL << ' ' << 3LL << '\n';
    } else if (a > 0LL) {
        ll x = ll(sqrt(a)) + 1LL;
        ll y = x * x - a - 1LL;
        if (y == 0) {
            x++;
            y = x * x - a - 1LL;
        }
        // if (1 <= y && y <= 1'000'000'000LL)
        cout << 1LL << ' ' << y << '\n';
        // else
        //     cout << "1\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> tc;
    while (tc--) {
        solve();
    }
    return 0;
}