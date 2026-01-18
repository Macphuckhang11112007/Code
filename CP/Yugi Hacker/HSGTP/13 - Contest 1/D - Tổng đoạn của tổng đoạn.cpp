// MPK

#ifndef ONLINE_JUDGE
#define _GLIBCXX_DEBUG
#endif

#pragma GCC optimize("Ofast,inline,unroll-loops,omit-frame-pointer")
#if __GNUC__ == 14 && __cplusplus > 202002L
#pragma GCC target("arch=x86-64")
#else
#pragma GCC target("arch=corei7-avx")
#endif

#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

typedef unsigned int       uint;
typedef unsigned long long ull;
typedef long long          ll;
typedef double             dbl;
typedef __int128_t         lll;
typedef __uint128_t        ulll;

typedef pair<int, int> pii;
typedef pair<ll, ll>   pll;

typedef tuple<int, int, int> tiii;
typedef tuple<ll, ll, ll>    tlll;

#define mset multiset
#define mmap multimap
#define uset unordered_set
#define umap unordered_map
#define pqueue priority_queue

template <typename T, typename V = vector<T>, typename C = less<T>>
using pqueue_max = pqueue<T, V, C>;

template <typename T, typename V = vector<T>, typename C = greater<T>>
using pqueue_min = pqueue<T, V, C>;

#define endl "\n"
#define endln cout << "\n"
#define sz(x) ((ll)(x).size())

#define DBG(x) cout << #x << " = " << (x) << "\n"

#define MAX(x, y) x = max(x, y)
#define MIN(x, y) x = min(x, y)
#define elif else if

#define mpr make_pair
#define mtpl make_tuple

#define fi first
#define se second
#define tp top
#define bk back
#define fr front
#define GET(x, i) get<(i) - 1>(x)

#define ep emplace
#define epf emplace_front
#define epb emplace_back
#define psh push
#define pshf push_front
#define pshb push_back
#define pp pop
#define ppb pop_back
#define ppf pop_front

const int IMAX   = INT_MAX / 2;
const ll  LLMAX  = LLONG_MAX / 2;
const ull ULLMAX = ULLONG_MAX / 2;
const int IMIN   = INT_MIN / 2;
const ll  LLMIN  = LLONG_MIN / 2;
const ll  MOD1   = 1000000007LL;
const ll  MOD2   = 998244353LL;
const dbl EPS    = 1E-9;

int     tc = 1;
string  sinp, sout;
fstream finp, gout;

void SetIO(string fl = "", string inp = "inp", string out = "out") {
    __builtin_ia32_ldmxcsr(40896);
    cin.tie(0)->sync_with_stdio(0);
    sinp = fl + "." + inp;
    sout = fl + "." + out;
    if (fl != "") {
        finp.open(sinp, ios::in);
        gout.open(sout, ios::out);
        if (!finp.is_open()) {
            finp.close();
            finp.open(sinp, ios::out);
            finp.close();
            finp.open(sinp, ios::in);
        }
        cin.rdbuf(finp.rdbuf());
        cout.rdbuf(gout.rdbuf());
    }
    return;
}

// -------------------- [Entry Point] --------------------
const int N = 3e5;

int n, q;
int a[N + 1];
ll  s[N + 1], p[N + 1], ss[N + 1];

ll ID(int x) {
    return 1LL * n * x - (x - 1) * x / 2;
}

int Calc(ll x) {
    int res = 0;
    int l = 0, r = n;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (ID(m) <= x) {
            res = m;
            l   = m + 1;
        } else {
            r = m - 1;
        }
    }
    return res;
    // ll  delta = 1LL * (2 * n + 1) * (2 * n + 1) - 8LL * x;
    // int res   = 0;
    // if (delta >= 0) {
    //     res = (2.0 * n + 1.0 - sqrt(delta)) / 2.0;
    //     if (res < 0) {
    //         res = (2.0 * n + 1.0 + sqrt(delta)) / 2.0;
    //     }
    // }
    // return res;
}

ll S(ll x) {
    int k  = Calc(x);
    ll  id = ID(k);
    int u = k + 1, v = u + x - id - 1;
    ll  res = ss[k] + (v + 1) * (s[v] - s[u - 1]) - (p[v] - p[u - 1]);
    return res;
}

void Solve() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        s[i] = s[i - 1] + a[i];
        p[i] = p[i - 1] + i * a[i];
    }
    for (int i = 1; i <= n; i++) {
        ss[i] = ss[i - 1] + (n + 1) * (s[n] - s[i - 1]) - (p[n] - p[i - 1]);
    }
    cin >> q;
    while (q--) {
        ll l, r;
        cin >> l >> r;
        cout << S(r) - S(l - 1) << "\n";
    }
    return;
}

signed Main() {
    SetIO();
    Solve();
    finp.close();
    gout.close();
    return 0;
}

signed main() {
#ifndef CPH
    auto time_start = high_resolution_clock::now();
    cerr << "\n\n";
#endif
    signed ret = Main();
#ifndef CPH
    auto   time_end = high_resolution_clock::now();
    double elapsed  = duration<double>(time_end - time_start).count() * 1e3;
    int    w1       = 36 - to_string(int(elapsed)).size();
    int    w2       = 25 - to_string(ret).size() - 2 * !ret;
    cerr << "\n\n-------------------- [Execution Summary] --------------------\n";
    cerr << "| Execution Time" << ": " << fixed << setprecision(4) << elapsed << " ms" << setw(w1) << "|\n";
    cerr << "| Exit Code     : " << ret << setw(w2) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (!ret ? ", Success) |" : ", Error) |") << "\n";
    cerr << "-------------------------------------------------------------\n\n";
#endif
    return ret;
}