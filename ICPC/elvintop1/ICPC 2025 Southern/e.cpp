#include <bits/stdc++.h>
using namespace std;

#define ll long long

bool can_pair(ll x, ll y) {
  if (x > y) swap(x, y);
  ll g = __gcd(x, y);
  return g == 1 || g == x;
}

ll f(int k) {
  if (k == 0) return 0;
  if (k % 2 == 0) return k / 2LL;
  return (k / 2LL) + 2;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while(t--) {
    int n;
    cin >> n;
    int a[n + 1];
    for(int i = 0;i < n;++i) cin >> a[i];
    ll cost = 0;
    int pending = 0;
    for (int i = 0; i < n; ) {
      int left = n - i;
      if (left >= 2) {
        bool can = can_pair(a[i], a[i + 1]);
        if (can) {
          cost += 1;
          i += 2;
        } else {
          cost += 2;
          i += 2;
          pending++;
        }
      } else if (left == 1) {
        if (pending > 0) {
          cost += 1;
          pending--;
          i += 1;
        } else {
          cost += 2;
          i += 1;
        }
      }
    }
    cost += f(pending);
    cout << cost << '\n';
  }
  return 0;
}