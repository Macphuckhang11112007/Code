#include <bits/stdc++.h>

using namespace std;

#define epb emplace_back

using ll = long long;

const int N   = 1e5;
const ll  MOD = 1e9 + 7;

int         n;
vector<int> adj[N + 5];
bool        vis[N + 5];
ll          ans = 1, sz[N + 5];

ll Pow(ll x, ll y) {
    if (!y) {
        return 1LL;
    }
    ll res = Pow(x, y / 2);
    (res *= res * (y & 1 ? x : 1LL) % MOD) %= MOD;
    return res;
}

void DFS(int u) {
    sz[u]  = 1;
    vis[u] = 1;
    for (int v : adj[u]) {
        if (!vis[v]) {
            DFS(v);
            sz[u] += sz[v];
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    DFS(1);
    for (int i = 1; i <= n; i++) {
        (ans *= sz[i]) %= MOD;
    }
    ans = Pow(ans, MOD - 2);
    for (int i = 1; i <= n; i++) {
        (ans *= i) %= MOD;
    }
    cout << ans;
}

// // MPK

// #pragma GCC optimize("Ofast,inline,unroll-loops,omit-frame-pointer")
// #if __GNUC__ == 14 && __cplusplus > 202002L
// #pragma GCC target("arch=x86-64")
// #else
// #pragma GCC target("arch=corei7-avx")
// #endif

// #include <bits/stdc++.h>

// using namespace std;
// using namespace chrono;

// typedef unsigned int       uint;
// typedef unsigned long long ull;
// typedef long long          ll;
// typedef double             dbl;
// typedef __int128_t         lll;
// typedef __uint128_t        ulll;

// typedef pair<int, int> pii;
// typedef pair<ll, ll>   pll;

// typedef tuple<int, int, int> tiii;
// typedef tuple<ll, ll, ll>    tlll;

// #define mset multiset
// #define mmap multimap
// #define uset unordered_set
// #define umap unordered_map
// #define pqueue priority_queue

// template <typename T, typename V = vector<T>, typename C = less<T>>
// using pqueue_max = pqueue<T, V, C>;

// template <typename T, typename V = vector<T>, typename C = greater<T>>
// using pqueue_min = pqueue<T, V, C>;

// #define endl "\n"
// #define endln cout << "\n"
// #define sz(x) ((ll)(x).size())

// #define DBG(x) cout << #x << " = " << (x) << "\n"

// #define MAX(x, y) x = max(x, y)
// #define MIN(x, y) x = min(x, y)
// #define elif else if

// #define mpr make_pair
// #define mtpl make_tuple

// #define fi first
// #define se second
// #define tp top
// #define bk back
// #define fr front
// #define GET(x, i) get<(i) - 1>(x)

// #define ep emplace
// #define epf emplace_front
// #define epb emplace_back
// #define psh push
// #define pshf push_front
// #define pshb push_back
// #define pp pop
// #define ppb pop_back
// #define ppf pop_front

// #define INF INFINITY
// #define _INF -INFINITY

// const int IMAX   = INT_MAX / 2;
// const ll  LLMAX  = LLONG_MAX / 2;
// const ull ULLMAX = ULLONG_MAX / 2;
// const int IMIN   = INT_MIN / 2;
// const ll  LLMIN  = LLONG_MIN / 2;
// const ll  MOD1   = 1000000007LL;
// const ll  MOD2   = 998244353LL;
// const dbl EPS    = 1E-9;

// int     tc = 1;
// string  sinp, sout;
// fstream finp, gout;

// void SetIO(string fl = "", string inp = "inp", string out = "out") {
//     __builtin_ia32_ldmxcsr(40896);
//     cin.tie(0)->sync_with_stdio(0);
//     sinp = fl + "." + inp;
//     sout = fl + "." + out;
//     if (fl != "") {
//         finp.open(sinp, ios::in);
//         gout.open(sout, ios::out);
//         if (!finp.is_open()) {
//             finp.close();
//             finp.open(sinp, ios::out);
//             finp.close();
//             finp.open(sinp, ios::in);
//         }
//         cin.rdbuf(finp.rdbuf());
//         cout.rdbuf(gout.rdbuf());
//     }
//     return;
// }

// // ==================== [Entry Point] ====================
// const int N = 1e5;

// int           n, sz[N + 1];
// vector<int>   a[N + 1];
// bitset<N + 1> vis;
// ll            fact[N + 1];
// map<ll, ll>   inv;

// ll binpow(ll x, ll y) {
//     ll res = 1;
//     while (y) {
//         if (y & 1) {
//             (res *= x) %= MOD1;
//         }
//         (x *= x) %= MOD1;
//         y >>= 1;
//     }
//     return res;
// }

// ll Inv(ll x) {
//     if (x <= 1) {
//         return max(x, 0LL);
//     }
//     return (inv.find(x) != inv.end() ? inv[x] : (inv[x] = MOD1 - (MOD1 / x * Inv(MOD1 % x)) % MOD1) %= MOD1);
// }

// ll C(ll x, ll y) {
//     if (y < 0 || x < y) {
//         return 0;
//     }
//     ll res = 1;
//     (res *= fact[x] * Inv(fact[y])) %= MOD1;
//     (res *= Inv(fact[x - y])) %= MOD1;
//     return res;
// }

// void DFS1(int u) {
//     vis[u] = sz[u] = 1;
//     for (auto v : a[u]) {
//         if (!vis[v]) {
//             DFS1(v);
//             sz[u] += sz[v];
//         }
//     }
//     return;
// }

// ll DFS2(int u, int c) {
//     ll res = C(c, sz[u]);
//     c      = sz[u] - 1;
//     vis[u] = 1;
//     for (auto v : a[u]) {
//         if (!vis[v]) {
//             (res *= DFS2(v, c)) %= MOD1;
//             c -= sz[v];
//         }
//     }
//     return res;
// }

// void Solve() {
//     cin >> n;
//     fact[0] = inv[1] = 1;
//     for (int i = 1; i <= n; i++) {
//         (fact[i] = fact[i - 1] * i) %= MOD1;
//     }
//     for (int i = 1; i < n; i++) {
//         int u, v;
//         cin >> u >> v;
//         a[u].epb(v);
//         a[v].epb(u);
//     }
//     DFS1(1);
//     for (int u = 1; u <= n; u++) {
//         vis[u] = 0;
//     }
//     cout << DFS2(1, n) << "\n";
//     return;
// }

// // ==================== [Exit Point] ====================

// signed Main() {
//     SetIO();
//     Solve();
//     finp.close();
//     gout.close();
//     return 0;
// }

// signed main() {
//     auto time_start = high_resolution_clock::now();
//     cerr << "\n\n";

//     signed ret = Main();

//     auto   time_end = high_resolution_clock::now();
//     double elapsed  = duration<double>(time_end - time_start).count() * 1e3;
//     int    w1       = 36 - to_string(int(elapsed)).size();
//     int    w2       = 25 - to_string(ret).size() - 2 * !ret;
//     cerr << "\n\n-------------------- [Execution Summary] --------------------\n";
//     cerr << "| Execution Time" << ": " << fixed << setprecision(4) << elapsed << " ms" << setw(w1) << "|\n";
//     cerr << "| Exit Code     : " << ret << setw(w2) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (!ret ? ", Success) |" : ", Error) |") << "\n";
//     cerr << "-------------------------------------------------------------\n\n";
//     return ret;
// }
