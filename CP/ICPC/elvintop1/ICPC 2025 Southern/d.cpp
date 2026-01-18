#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD = 1000000007;
ll mod_pow(ll a, ll b, ll m) {
  ll res = 1;
  while (b) {
    if (b & 1) res = res * a % m;
    a = a * a % m;
    b >>= 1;
  }
  return res;
}
ll mod_inv(ll a, ll m) { return mod_pow(a, m - 2, m); }
vector<ll> get_divs(ll x) {
  vector<ll> d;
  for (ll i = 1; i * i <= x; i++) {
    if (x % i == 0) {
      d.push_back(i);
      if (i != x / i) d.push_back(x / i);
    }
  }
  sort(d.begin(), d.end());
  return d;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  ll n, k;
  cin >> n >> k;
  auto divs = get_divs(n);
  int m = divs.size();
  map<ll, int> dtoi;
  // cout << divs[m - 1] << '\n';
  for (int i = 0; i < m; i++) dtoi[divs[i]] = i;
  // cout << dtoi[n] << '\n';
  vector<ll> pms;
  auto factor = [](ll x) -> map<ll, int> {
    map<ll, int> res;
    for (ll i = 2; i * i <= x; i++) {
      if (x % i == 0) {
        int c = 0;
        while (x % i == 0) {
          x /= i;
          c++;
        }
        res[i] = c;
      }
    }
    if (x > 1) res[x] = 1;
    return res;
  };
  auto fn = factor(n);
  for (auto& pr : fn) pms.push_back(pr.first);
  vector<vector<ll>> T(m, vector<ll>(m, 0));
  for (int i = 0; i < m; i++) {
    ll v = divs[i];
    if (v == 1) {
      T[i][dtoi[1]] = 1 % MOD;
      continue;
    }
    int omega = 0;
    map<ll, int> ex;
    for (auto p : pms) {
      if (v % p != 0) continue;
      int c = 0;
      ll vv = v;
      while (vv % p == 0) {
        vv /= p;
        c++;
      }
      ex[p] = c;
      omega += c;
    }
    ll invom = mod_inv(omega, MOD);
    for (auto& pr : ex) {
      ll p = pr.first;
      int ep = pr.second;
      ll u = v / p;
      int j = dtoi[u];
      ll contrib = (ll)ep * invom % MOD;
      T[i][j] = (T[i][j] + contrib) % MOD;
    }
  }
  auto mul = [&](const vector<vector<ll>>& a, const vector<vector<ll>>& b) -> vector<vector<ll>> {
    int sz = m;
    vector<vector<ll>> c(sz, vector<ll>(sz, 0));
    for (int i = 0; i < sz; i++)
      for (int l = 0; l < sz; l++)
        for (int j = 0; j < sz; j++) c[i][j] = (c[i][j] + a[i][l] * b[l][j] % MOD) % MOD;
    return c;
  };
  auto powm = [&](vector<vector<ll>> a, ll e) -> vector<vector<ll>> {
    int sz = m;
    vector<vector<ll>> res(sz, vector<ll>(sz, 0));
    for (int i = 0; i < sz; i++) res[i][i] = 1;
    while (e > 0) {
      if (e & 1) res = mul(res, a);
      a = mul(a, a);
      e >>= 1;
    }
    return res;
  };
  auto Tk = powm(T, k);
  vector<ll> ini(m);
  for (int i = 0; i < m; i++) ini[i] = divs[i] % MOD;
  vector<ll> fin(m, 0);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < m; j++) fin[i] = (fin[i] + Tk[i][j] * ini[j] % MOD) % MOD;
  int start = dtoi[n];
  // cout << dtoi[n] << '\n';
  // cout << start << '\n';
  cout << fin[start] << '\n';
  return 0;
}