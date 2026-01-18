#include <bits/stdc++.h>

using namespace std;

#define fi first
#define se second

typedef long long    ll;
typedef pair<ll, ll> pll;

const int N    = 1e6;
const ll  BASE = 10, MOD = 33;

int              n, q, a[N + 5], b[N + 5];
ll               h[N + 5], p[N + 5] = {1};
pair<ll, string> ans;
string           str;
vector<string>   arr;
// map<pll, ll>     cnt;
// map<pll, pll> mp;
map<ll, ll>  cnt;
map<ll, pll> mp;

void MAX(auto &x, auto y) {
    x = max(x, y);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> str >> q;
    n   = str.size();
    str = " " + str;
    // for (int i = 1; i <= n; i++) {
    //     a[i] = (a[i - 1] + str[i] - '0') % 3;
    //     b[i] = (b[i - 1] + (i & 1 ? str[i] - '0' : -(str[i] - '0')) + 11) % 11;
    // }
    // cnt[{0, 0}] = 1;
    // mp[{0, 0}]  = {0, -1};
    // for (int i = 1; i <= n; i++) {
    //     if (q == 1) {
    //         ans.fi += cnt[{a[i], b[i]}]++;
    //     } else {
    //         if (mp.find({a[i], b[i]}) == mp.end()) {
    //             mp[{a[i], b[i]}] = {i, -1};
    //         } else {
    //             MAX(mp[{a[i], b[i]}].se, ll(i));
    //         }
    //     }
    // }
    // if (q == 1) {
    //     cout << ans.fi;
    // } else {
    //     for (auto [key, val] : mp) {
    //         auto [l, r] = val;
    //         if (r != -1) {
    //             string tmp = str.substr(l + 1, r - l);
    //             if (ans.se.size() < tmp.size()) {
    //                 ans.se = tmp;
    //             } else if (ans.se.size() == tmp.size()) {
    //                 MAX(ans.se, tmp);
    //             }
    //         }
    //     }
    //     cout << (ans.se.size() ? ans.se : "-1");
    // }
    for (int i = 1; i <= n; i++) {
        p[i] = (p[i - 1] * BASE) % MOD;
    }
    for (int i = 1; i <= n; i++) {
        h[i] = (h[i - 1] + (str[i] - '0') * p[n - i]) % MOD;
    }
    if (q == 1) {
        cnt[0] = 1;
        for (int i = 1; i <= n; i++) {
            ans.fi += cnt[h[i]]++;
        }
        cout << ans.fi;
    } else {
        mp[0] = {0, -1};
        for (int i = 1; i <= n; i++) {
            if (mp.find(h[i]) == mp.end()) {
                mp[h[i]] = {i, -1};
            } else {
                MAX(mp[h[i]].se, ll(i));
            }
        }
        for (auto [key, val] : mp) {
            auto [l, r] = val;
            if (r != -1) {
                string tmp = str.substr(l + 1, r - l);
                if (ans.se.size() < tmp.size()) {
                    ans.se = tmp;
                } else if (ans.se.size() == tmp.size()) {
                    MAX(ans.se, tmp);
                }
            }
        }
        cout << (ans.se.size() ? ans.se : "-1");
    }
}
