#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

typedef long long      ll;
typedef pair<int, int> pii;

const int N = 2e5 + 5;

struct Query {
    int type;
    int u;
    ll  val;
};

int          n, q;
ll           sum[N], a[N], sz[N];
int          par[N];
bool         is_cut[N];
pii          edges[N];
multiset<ll> sett;

int Par(int u) {
    return (u != par[u] ? par[u] = Par(par[u]) : u);
}

void Union(int u, int v) {
    int pu = Par(u);
    int pv = Par(v);
    if (pu == pv) return;

    if (sz[pu] < sz[pv]) swap(pu, pv);

    sett.erase(sett.find(sum[pu]));
    sett.erase(sett.find(sum[pv]));

    par[pv] = pu;
    sz[pu] += sz[pv];
    sum[pu] += sum[pv];

    sett.insert(sum[pu]);
}

void solve() {
    sett.clear();

    if (!(cin >> n)) return;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        par[i]    = i;
        sz[i]     = 1;
        is_cut[i] = false;
    }

    for (int i = 1; i < n; i++) {
        cin >> edges[i].fi >> edges[i].se;
        is_cut[i] = false;
    }

    cin >> q;
    vector<Query> qr(q);

    for (int i = 0; i < q; i++) {
        cin >> qr[i].type;
        if (qr[i].type == 1) {
            cin >> qr[i].val;
            is_cut[qr[i].val] = true;
        } else if (qr[i].type == 2) {
            cin >> qr[i].u >> qr[i].val;
        }
    }

    for (int i = 1; i <= n; i++) sum[i] = a[i];

    for (auto &Q : qr) {
        if (Q.type == 2) { sum[Q.u] += Q.val; }
    }

    for (int i = 1; i <= n; i++) sett.insert(sum[i]);

    for (int i = 1; i < n; i++) {
        if (!is_cut[i]) { Union(edges[i].fi, edges[i].se); }
    }

    vector<ll> ans;
    reverse(qr.begin(), qr.end());

    for (auto &Q : qr) {
        if (Q.type == 1) {
            int idx = Q.val;
            int u   = edges[idx].fi;
            int v   = edges[idx].se;

            ll sum1 = sum[Par(u)];
            ll sum2 = sum[Par(v)];
            ans.push_back(abs(sum1 - sum2));

            Union(u, v);

        } else if (Q.type == 2) {
            int u = Par(Q.u);

            sett.erase(sett.find(sum[u]));
            sum[u] -= Q.val;
            sett.insert(sum[u]);

        } else if (Q.type == 3) {
            ans.push_back(*sett.rbegin());
        }
    }

    reverse(ans.begin(), ans.end());
    for (ll x : ans) cout << x << "\n";
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int tc;
    if (cin >> tc) {
        while (tc--) { solve(); }
    }
    return 0;
}

// #include <bits/stdc++.h>

// #define fi first
// #define se second

// using namespace std;

// typedef long long    ll;
// typedef pair<ll, ll> pll;

// const int N = 2e5 + 1;

// int                tc;
// int                n, q;
// ll                 sum[N + 1], a[N + 1], sz[N + 1], par[N + 1];
// vector<vector<ll>> qr;
// vector<pll>        g;
// map<int, int>      mp;
// multiset<ll>       sett;

// int Par(int u) {
//     return (u != par[u] ? par[u] = Par(par[u]) : u);
// }

// int DSU(int u, int v) {
//     int pu = Par(u), pv = Par(v);
//     if (pu == pv) {
//         return sum[pu];
//     }
//     if (sz[pu] < sz[pv]) {
//         swap(pu, pv);
//     }
//     par[pv] = pu;
//     sz[pu] += pv;
//     auto p = sett.find(sum[pu]);
//     if (p != sett.end()) {
//         sett.erase(p);
//     }
//     p = sett.find(sum[pv]);
//     if (p != sett.end()) {
//         sett.erase(p);
//     }
//     sum[pu] += sum[pv];
//     sett.insert(sum[pu]);
//     return sum[pu];
// }

// int main() {
//     cin.tie(0)->sync_with_stdio(0);
//     cin >> tc;
//     while (tc--) {
//         stack<int> st;
//         cin >> n;
//         for (int i = 1; i <= n; i++) {
//             cin >> a[i];
//             par[i] = i;
//             sz[i]  = 1;
//             sum[i] = a[i];
//             sett.insert(sum[i]);
//         }
//         for (int i = 1; i < n; i++) {
//             int u, v;
//             cin >> u >> v;
//             g.push_back({u, v});
//         }
//         cin >> q;
//         for (int i = 1; i <= q; i++) {
//             int opt;
//             cin >> opt;
//             if (opt == 1) {
//                 int x;
//                 cin >> x;
//                 qr.push_back({opt, --x});
//             } else if (opt == 2) {
//                 int u, x;
//                 cin >> u >> x;
//                 qr.push_back({opt, u, x});
//             } else if (opt == 3) {
//                 qr.push_back({opt});
//             }
//         }
//         reverse(qr.begin(), qr.end());
//         for (auto i : qr) {
//             int opt = i[0];
//             if (opt == 1) {
//                 int x = i[1];
//                 mp[x] = 1;
//             } else if (opt == 2) {
//                 auto [u, x] = make_tuple(i[1], i[2]);
//                 a[u] += x;
//                 auto p = sett.find(sum[u]);
//                 if (p != sett.end()) {
//                     sett.erase(p);
//                 }
//                 sum[u] += x;
//                 sett.insert(sum[u]);
//             }
//         }
//         for (int i = 0; i < n - 1; i++) {
//             if (!mp[i]) {
//                 DSU(g[i].fi, g[i].se);
//             }
//         }
//         for (auto i : qr) {
//             int opt = i[0];
//             if (opt == 1) {
//                 int x = i[1];
//                 st.push(abs(DSU(g[x].fi, g[x].fi) - DSU(g[x].se, g[x].se)));
//                 DSU(g[x].fi, g[x].se);
//             } else if (opt == 2) {
//                 auto [u, x] = make_tuple(i[1], i[2]);
//                 auto p      = sett.find(sum[Par(u)]);
//                 if (p != sett.end()) {
//                     sett.erase(p);
//                 }
//                 sum[Par(u)] -= x;
//                 sett.insert(sum[Par(u)]);
//             } else if (opt == 3) {
//                 st.push(*sett.rbegin());
//             }
//         }
//         while (st.size()) {
//             int x = st.top();
//             cout << x << "\n";
//             st.pop();
//         }
//     }
//     return 0;
// }
