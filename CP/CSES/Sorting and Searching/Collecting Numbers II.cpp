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
typedef pair<pii, int> pii_i;
typedef pair<int, pii> pi_ii;
typedef pair<pii, pii> pii_ii;
typedef pair<ll, ll>   pll;
typedef pair<pll, ll>  pll_l;
typedef pair<ll, pll>  pl_ll;
typedef pair<pll, pll> pll_ll;

typedef tuple<int, int, int>      tiii;
typedef tuple<ll, ll, ll>         tlll;
typedef tuple<int, int, int, int> tiiii;
typedef tuple<ll, ll, ll, ll>     tllll;

typedef vector<int>          vi;
typedef vector<ll>           vl;
typedef vector<pii>          vii;
typedef vector<pll>          vll;
typedef vector<pii>          vii;
typedef vector<pii, int>     vii_i;
typedef vector<int, pii>     vi_ii;
typedef vector<pii, pii>     vii_ii;
typedef vector<tiii>         viii;
typedef vector<tiiii>        viiii;
typedef vector<pll>          vll;
typedef vector<pll, ll>      vll_l;
typedef vector<ll, pll>      vl_ll;
typedef vector<pll, pll>     vll_ll;
typedef vector<tlll>         vlll;
typedef vector<tllll>        vllll;
typedef vector<vector<int>>  vvi;
typedef vector<vector<ll>>   vvll;
typedef vector<vector<char>> vvc;

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
#define rall(x) x.rbegin(), x.rend()
#define allir(x, l, r) x.begin() + l, x.begin() + r + 1
#define rallir(x, l, r) x.rbegin(), x.rbegin() + r + 1
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

#define FORC(i, l, r, d) for (ll i = l; i <= r; i += d)
#define RFORC(i, r, l, d) for (ll i = r; i <= l; i -= d)
#define FORAC(i, l, r, d) for (i = l; i <= r; i += d)
#define RFORAC(i, r, l, d) for (i = r; i <= l; i -= d)

#define FORIC(i, l, r, d) for (auto i = l; i != r; advance(i, d))
#define RFORIC(i, r, l, d) for (auto i = r; i != l; advance(i, -d))
#define FORAIC(i, l, r, d) for (i = l; i != r; advance(i, d))
#define RFORAIC(i, r, l, d) for (i = r; i != l; advance(i, -d))

#define FORE(i, x) for (auto i : x)
#define FORAE(i, x) for (auto &i : x)

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
const dbl EPS    = 1e-9;

template <typename T, typename... Args>
void PRINT(T x, Args... args) {
    cout << x;
    if constexpr (sizeof...(args)) {
        PRINT(args...);
    }
}

template <typename T, typename... Args>
void PRINTS(T x, Args... args) {
    cout << x << " ";
    if constexpr (sizeof...(args)) {
        PRINTS(args...);
    }
}

template <typename T, typename... Args>
void PRINTLN(T x, Args... args) {
    cout << x << "\n";
    if constexpr (sizeof...(args)) {
        PRINTLN(args...);
    }
}

template <typename... Args>
void PRINTC(Args... args) {
    PRINTS(args...);
    cout << "\n";
}

template <typename T, typename L, typename R>
void PRINTCON(T x, L l, R r) {
    for (ll i = l; i <= r; i++) {
        cout << x[i] << " ";
    }
    cout << "\n";
}

template <typename T, typename L, typename R>
void PRINTRCON(T x, R r, L l) {
    for (ll i = r; i >= l; i--) {
        cout << x[i] << " ";
    }
    cout << "\n";
}

template <typename T, typename L, typename R>
void PRINTCONI(T x, L l, R r) {
    for (auto i = next(x, l); i != next(x, r + 1); i++) {
        cout << *i << " ";
    }
    cout << "\n";
}

template <typename T, typename L, typename R>
void PRINTRCONI(T x, R r, L l) {
    for (auto i = next(x, r); i != next(x, l - 1); i--) {
        cout << *i << " ";
    }
    cout << "\n";
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
const int N = 2e5;

int n, q, id[N + 5], a[N + 5];
int ans = 1;

void PreProcess() {

    return;
}

void chk(int i, int j) {
    int k = id[a[i] - 1], t = id[a[i] + 1];
    if ((i < k) != (j < k)) {
        ans += (i < k ? -1 : 1);
    }
    if ((i < t) != (j < t)) {
        ans += (i < t ? 1 : -1);
    }
    return;
}

void Solve() {
    cin >> n >> q;
    FOR(i, 1, n) {
        int x;
        cin >> x;
        a[i]  = x;
        id[x] = i;
    }
    FOR(i, 1, n) {
        ans += (id[i] < id[i - 1]);
    }
    while (q--) {
        int i, j;
        cin >> i >> j;
        if (i > j) {
            swap(i, j);
        }
        if (abs(a[i] - a[j]) == 1 && abs(i - j) == 1) {
            ans += (a[i] < a[j] ? 1 : -1);
        } else {
            chk(i, j);
            chk(j, i);
        }
        swap(id[a[i]], id[a[j]]);
        swap(a[i], a[j]);
        PRINTLN(ans);
    }
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