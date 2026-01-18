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
const int N = 1e6;

int n, a[N + 10];
ll  s;
int ans;

void Solve() {
    cin >> n >> s;
    FOR(i, 1, n) {
        cin >> a[i];
    }
    int l = 1, r = 1;
    ll  sum = a[1];
    while (r <= n) {
        if (l <= r) {
            if (sum <= s) {
                MAX(ans, r - l + 1);
                r++;
                sum += a[r];
            } else {
                sum -= a[l];
                l++;
            }
        } else {
            r++;
            sum = a[l];
        }
    }
    PRINTLN(ans);
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