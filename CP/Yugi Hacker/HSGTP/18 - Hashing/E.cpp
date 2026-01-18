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

#define INF INFINITY
#define _INF -INFINITY

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

// ==================== [Entry Point] ====================
const int N    = 5e4;
const ll  BASE = 1'111'111'111'111'111'111LL, MOD = (1LL << 61) - 1;

string s;
int    n, ans;
ll     h[N + 2], rh[N + 2], p[N + 1] = {1};

ll mul(ll x, ll y) {
    lll res = lll(1) * x * y;
    res     = (res >> 61) + (res & MOD);
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

ll add(ll x, ll y) {
    ll res = x + y;
    if (res >= MOD) {
        res -= MOD;
    }
    return res;
}

ll sub(ll x, ll y) {
    ll res = x - y;
    if (res < 0) {
        res += MOD;
    }
    return res;
}

ll getHash(int l, int r) {
    return sub(h[r], mul(h[l - 1], p[r - l + 1]));
}

ll getRHash(int l, int r) {
    return sub(rh[l], mul(rh[r + 1], p[r - l + 1]));
}

void Solve() {
    cin >> n >> s;
    s = " " + s;
    for (int i = 1; i <= n; i++) {
        p[i] = mul(p[i - 1], BASE);
    }
    for (int i = 1; i <= n; i++) {
        h[i] = add(mul(h[i - 1], BASE), s[i]);
    }
    for (int i = n; i; i--) {
        rh[i] = add(mul(rh[i + 1], BASE), s[i]);
    }
    int l = 0, r = n / 2;
    while (l <= r) {
        int  mid = (l + r) / 2;
        int  len = 2 * mid;
        bool ok  = 0;
        for (int i = mid; !ok && i <= n; i++) {
            int d = len / 2;
            ok |= (getHash(i - len + 1, i - len + d) == getRHash(i - d + 1, i));
        }
        if (ok) {
            ans = len;
            l   = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    l = 0;
    r = (n - 1) / 2;
    while (l <= r) {
        int  mid = (l + r) / 2;
        int  len = 2 * mid + 1;
        bool ok  = 0;
        for (int i = mid; !ok && i <= n; i++) {
            int d = len / 2;
            ok |= (getHash(i - len + 1, i - len + d) == getRHash(i - d + 1, i));
        }
        if (ok) {
            ans = max(ans, len);
            l   = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    cout << ans;
    return;
}

// ==================== [Exit Point] ====================

signed Main() {
    SetIO();
    Solve();
    finp.close();
    gout.close();
    return 0;
}

signed main() {
    auto time_start = high_resolution_clock::now();
    cerr << "\n\n";

    signed ret = Main();

    auto   time_end = high_resolution_clock::now();
    double elapsed  = duration<double>(time_end - time_start).count() * 1e3;
    int    w1       = 36 - to_string(int(elapsed)).size();
    int    w2       = 25 - to_string(ret).size() - 2 * !ret;
    cerr << "\n\n-------------------- [Execution Summary] --------------------\n";
    cerr << "| Execution Time" << ": " << fixed << setprecision(4) << elapsed << " ms" << setw(w1) << "|\n";
    cerr << "| Exit Code     : " << ret << setw(w2) << "(0x" << setfill('0') << setw(8) << hex << uppercase << ret << (!ret ? ", Success) |" : ", Error) |") << "\n";
    cerr << "-------------------------------------------------------------\n\n";
    return ret;
}