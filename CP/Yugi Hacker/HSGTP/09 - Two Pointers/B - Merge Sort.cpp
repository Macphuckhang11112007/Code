// MPK

#ifndef THEMIS
#ifndef ONLINE_JUDFE
#define _GLIBCXX_DEBUG
#endif
#pragma GCC optimize("Ofast,inline,unroll-loops,omit-frame-pointer")
#if __GNUC__ == 14 && __cplusplus > 202002L
#pragma GCC target("arch=x86-64")
#else
#pragma GCC target("arch=corei7-avx")
#endif
#endif

#include <bits/stdc++.h>

using namespace std;
using namespace chrono;

typedef unsigned int       uint;
typedef unsigned long long ull;
typedef long long          ll;
typedef __int128_t         lll;
typedef __uint128_t        ulll;
typedef double             dbl;
typedef long double        ldbl;

typedef pair<int, int> pii;
typedef pair<ll, ll>   pll;

typedef tuple<int, int, int>      tiii;
typedef tuple<ll, ll, ll>         tlll;
typedef tuple<int, int, int, int> tiiii;
typedef tuple<ll, ll, ll, ll>     tllll;

#define mpr make_pair
#define mtpl make_tuple

typedef vector<int>  vi;
typedef vector<ll>   vl;
typedef vector<pll>  vll;
typedef vector<pii>  vpii;
typedef vector<tiii> vtiii;

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
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.end()
#define allr(x, l, r) x + l, x + r + 1
#define rallr(x, r, l) x + r, x + l - 1
#define sz(x) ((ll)(x).size())

#define DBG(x) cout << #x << " = " << (x)
#define DBGS(x) cout << #x << " = " << (x) << " "
#define DBGLN(x) cout << #x << " = " << (x) << "\n"

#define FOR(i, l, r) for (ll i = l; i <= r; i++)
#define RFOR(i, r, l) for (ll i = r; i <= l; i--)
#define FORA(i, l, r) for (i = l; i <= r; i++)
#define RFORA(i, r, l) for (i = r; i <= l; i--)
#define FORI(i, l, r) for (auto i = l; i != r; i++)
#define RFORI(i, r, l) for (auto i = r; i != l; i--)
#define FORAI(i, l, r) for (i = l; i != r; i++)
#define RFORAI(i, r, l) for (i = r; i != l; i--)
#define MAX(x, y) x = max(x, y)
#define MIN(x, y) x = min(x, y)
#define elif else if

const dbl INF    = INFINITY;
const int IMAX   = INT_MAX / 2;
const ll  LLMAX  = LLONG_MAX / 2;
const ull ULLMAX = ULLONG_MAX / 2;
const int IMIN   = INT_MIN / 2;
const ll  LLMIN  = LLONG_MIN / 2;
const ll  MOD1   = 1000000007LL;
const ll  MOD2   = 998244353LL;
const dbl EPS    = 1e-9;

template <typename T, typename... Args>
void PRINT(T x, Args... args) {
    cout << x;
    if constexpr (sizeof...(args))
        PRINT(args...);
}

template <typename T, typename... Args>
void PRINTS(T x, Args... args) {
    cout << x << " ";
    if constexpr (sizeof...(args))
        PRINTS(args...);
}

template <typename T, typename... Args>
void PRINTLN(T x, Args... args) {
    cout << x << "\n";
    if constexpr (sizeof...(args))
        PRINTLN(args...);
}

int tc = 1;

void SetIO(int mtc = 0, string file = "", string inp = ".inp", string out = ".out") {
    if (file != "") {
        fstream fin(file + inp, ios::in);
        fstream fout(file + out, ios::out);
        cin.rdbuf(fin.rdbuf());
        cout.rdbuf(fout.rdbuf());
    }
    cin.tie(0)->sync_with_stdio(0);
    __builtin_ia32_ldmxcsr(40896);
    if (mtc == 1) {
        cin >> tc;
    } else if (mtc == -1) {
        tc = -1;
    }
    return;
}

// -------------------- [Entry Point] --------------------
#define eb emplace_back

const int N = 1e6;

int          n, m;
int          a[N + 10], b[N + 10];
vector<int>  v1;
vector<char> v2;

void Solve() {
    cin >> n;
    FOR(i, 1, n) {
        cin >> a[i];
    }
    cin >> m;
    FOR(i, 1, m) {
        cin >> b[i];
    }
    int i = 1, j = 1;
    while (i <= n && j <= m) {
        if (a[i] <= b[j]) {
            v1.eb(a[i]);
            v2.eb('a');
            i++;
        } else {
            v1.eb(b[j]);
            v2.eb('b');
            j++;
        }
    }
    while (i <= n) {
        v1.eb(a[i]);
        v2.eb('a');
        i++;
    }
    while (j <= m) {
        v1.eb(b[j]);
        v2.eb('b');
        j++;
    }
    for (auto i : v1) {
        PRINTS(i);
    }
    endln;
    for (auto i : v2) {
        PRINT(i);
    }
    endln;
    return;
}

signed Main() {
    SetIO();
    if (tc != -1) {
        for (int i = 1; i <= tc; i++) {
            Solve();
        }
    } else {
        while (1) {
            Solve();
        }
    }
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
    double elapsed  = (time_end - time_start).count() * 1e3;
    int    w1       = 36 - to_string(int(elapsed * 1e3)).size();
    int    w2       = 25 - to_string(ret).size() - 2 * !ret;
    cerr << "\n\n-------------------- [Execution Summary] --------------------\n";
    cerr << "| Execution Time" << ": " << fixed << setprecision(4) << elapsed << " ms" << setw(w1) << "|\n";
    cerr << "| Exit Code     : " << ret << setw(w2) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (!ret ? ", Success) |" : ", Error) |") << "\n";
    cerr << "-------------------------------------------------------------\n\n";
#endif
    return ret;
}