// MPK

#if !defined(ONLINE_JUDGE) && !defined(THEMIS)
#define _GLIBCXX_DEBUG
#endif

#ifndef THEMIS
#pragma GCC optimize("O3,inline,unroll-loops,omit-frame-pointer")
#if __GNUC__ == 14 && __cplusplus > 202002L
#pragma GCC target("arch=x86-64")
#else
#pragma GCC target("arch=corei7-avx")
#endif
#endif

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace std::chrono;
using namespace __gnu_pbds;

#define endl "\n"
#define endln cout << "\n"

#define uint unsigned int
#define ll long long
#define ull unsigned long long
#define lll __int128_t
#define ulll __uint128_t
#define dbl double
#define ldbl long double
#define int8 int8_t
#define uint8 uint8_t
#define int16 int16_t
#define uint16 uint16_t
#define int32 int32_t
#define uint32 uint32_t
#define int64 int64_t
#define uint64 uint64_t
#define int128 __int128_t
#define uint128 __uint128_t

#define mpr make_pair
#define mtpl make_tuple

#define pii pair<int, int>
#define pii_i pair<pii, int>
#define pi_ii pair<int, pii>
#define pii_ii pair<pii, pii>
#define tiii tuple<int, int, int>
#define tlll tuple<ll, ll, ll>
#define tiiii tuple<int, int, int, int>
#define tllll tuple<ll, ll, ll, ll>
#define pll pair<ll, ll>
#define pll_l pair<pll, ll>
#define pl_ll pair<ll, pll>
#define pll_ll pair<pll, pll>

#define vi vector<int>
#define vl vector<ll>
#define vii vector<pii>
#define vll vector<pll>
#define vii vector<pii>
#define vii_i vector<pii, int>
#define vi_ii vector<int, pii>
#define vii_ii vector<pii, pii>
#define viii vector<tiii>
#define viiii vector<tiiii>
#define vll vector<pll>
#define vll_l vector<pll, ll>
#define vl_ll vector<ll, pll>
#define vll_ll vector<pll, pll>
#define vlll vector<tlll>
#define vllll vector<tllll>
#define vvi vector<vector<int>>
#define vvll vector<vector<ll>>
#define vvc vector<vector<char>>

#define umap unordered_map
#define uset unordered_set

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
#define epv(_x, _val) _x.ep(_val)
#define epfv(_x, _val) _x.epf(_val)
#define epbv(_x, _val) _x.epb(_val)
#define pshv(_x, _val) _x.psh(_val)
#define pshfv(_x, _val) _x.pshf(_val)
#define pshbv(_x, _val) _x.pshb(_val)

#define INF INFINITY
#define IMAX INT_MAX / 2
#define UIMAX UINT_MAX / 2
#define LLMAX LLONG_MAX / 2
#define ULLMAX ULLONG_MAX / 2
#define MOD1 (ll)(1e9 + 7)
#define MOD2 (ll)(998'244'353)
#define EPS (dbl)(1e-9)

#define all(_x) _x.begin(), _x.end()
#define rall(_x) prev(_x.end()), _x.begin()
#define allr(_x, _l, _r) _x + _l, _x + _r + 1
#define sz(_x) (ll)(_x.size())
#define get(_x, _i) (get<(_i) - 1>(_x))

#define DBG(_x) cout << #_x << " = " << _x << "\n"
#define PRINT(_x) cout << _x
#define PRINTS(_x) cout << _x << " "
#define PRINTLN(_x) cout << _x << "\n"
#define FOR(_i, _l, _r) for (long long _i = _l; _i <= _r; _i++)
#define RFOR(_i, _r, _l) for (long long _i = _r; _i >= _l; _i--)
#define MAX(_x, _y) _x = max(_x, _y)
#define MIN(_x, _y) _x = min(_x, _y)

template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename Key, typename Value>
using ordered_map = tree<Key, Value, less<Key>, rb_tree_tag, tree_order_statistics_node_update>;

mt19937_64 rng(steady_clock::now().time_since_epoch().count());

int tc = 1;

long long Rand() {
    return rng();
}

long long RandRange(long long l, long long r) {
    uniform_int_distribution<long long> dist(l, r);
    return dist(rng);
}

double RandReal() {
    uniform_real_distribution<double> dist(-1e9, 1e9);
    return dist(rng);
}

double RandRealRange(double l, double r) {
    uniform_real_distribution<double> dist(l, r);
    return dist(rng);
}

void SetIO(int mtc = 0, string file = "", string inp = ".inp", string out = ".out") {
    if (file != "") {
        fstream fin(file + inp, ios::in);
        fstream fout(file + out, ios::out);
        cin.rdbuf(fin.rdbuf());
        cout.rdbuf(fout.rdbuf());
    }
#ifndef _GLIBCXX_DEBUG
    cin.tie(0)->sync_with_stdio(0);
#endif
    if (mtc == 1) {
        cin >> tc;
    } else if (mtc == -1) {
        tc = -1;
    }
    return;
}

// -------------------- [Entry Point] --------------------
const int N = 400;

int n, a[N + 10][N + 10];
int ans = -IMAX / 2, diag1[N + 10][N + 10], diag2[N + 10][N + 10];

void PreProcess();
bool Input();
void Reset();
void Solve();
void Output();

void PreProcess() {

    return;
}

bool Input() {
    cin >> n;
    FOR(i, 1, n) {
        FOR(j, 1, n) {
            cin >> a[i][j];
        }
    }
    return 1;
}

void Reset() {

    return;
}

void Solve() {
    FOR(i, 1, n) {
        FOR(j, 1, n) {
            diag1[i][j] = diag1[i - 1][j - 1] + a[i][j];
            diag2[i][j] = diag2[i - 1][j + 1] + a[i][j];
        }
    }
    FOR(i, 1, n) {
        FOR(j, i, n) {
            int sz = j - i + 1;
            FOR(k, 1, n - sz + 1) {
                MAX(ans, diag1[j][k + sz - 1] - diag1[i - 1][k - 1] - (diag2[j][k] - diag2[i - 1][k + sz]));
            }
        }
    }
    return;
}

void Output() {
    PRINTLN(ans);
    return;
}

signed Main() {
    SetIO();
    PreProcess();
    if (tc != -1) {
        for (int i = 1; i <= tc; i++) {
            Input();
            Reset();
            Solve();
            Output();
        }
    } else {
        while (Input()) {
            Reset();
            Solve();
            Output();
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
    auto             time_end = high_resolution_clock::now();
    duration<double> elapsed  = time_end - time_start;
    cerr << "\n\n-------------------- [Execution Summary] "
            "--------------------\n";
    cerr << "| Execution Time" << ": ";
    if (elapsed.count() < 1) {
        cerr << fixed << setprecision(4) << elapsed.count() * 1e3 << " ms" << setw(36 - to_string(int(elapsed.count() * 1e3)).length()) << "|\n";
    } else {
        cerr << fixed << setprecision(7) << elapsed.count() << " s" << setw(34 - to_string(int(elapsed.count())).length()) << "|\n";
    }
    cerr << "| Exit Code     : " << ret << setw(25 - to_string(ret).length() - 2 * !ret) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (ret == 0 ? ", Success) |" : ", Error) |") << "\n";
    cerr << "-------------------------------------------------------------\n\n";
#endif
    return ret;
}