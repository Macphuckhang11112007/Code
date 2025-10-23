// MPK

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
const int N = 1000, K = 30;

int           n, m, k, ans = IMAX;
char          a[N + 1][N + 1];
pii           s, t;
int           d[N + 1][N + 1][K + 1];
bitset<K + 1> vst[N + 1][N + 1];
int           dx[] = {0, 1, 0, -1}, dy[] = {-1, 0, 1, 0};

struct Comp {
    bool operator()(tuple<int, int, pii> &i, tuple<int, int, pii> &j) const {
        if (get<0>(i) != get<0>(j)) {
            return get<0>(i) > get<0>(j);
        } else if (get<1>(i) != get<1>(j)) {
            return get<1>(i) < get<1>(j);
        } else {
            return get<2>(i) > get<2>(j);
        }
    }
};

void Solve() {
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            char c;
            cin >> c;
            a[i][j] = c;
            if (c == 'S') {
                s = {i, j};
            } else if (c == 'F') {
                t = {i, j};
            }
            for (int p = 0; p <= k; p++) {
                d[i][j][p] = IMAX;
            }
        }
    }
    for (int i = 0; i <= k; i++) {
        d[s.fi][s.se][i] = 0;
    }
    pqueue<tuple<int, int, pii>, vector<tuple<int, int, pii>>, Comp> pq;
    pq.ep(d[s.fi][s.se][0], 0, s);
    while (sz(pq)) {
        auto [dist, curr_k, u] = pq.tp();
        pq.pp();
        if (vst[u.fi][u.se][curr_k]) {
            continue;
        }
        vst[u.fi][u.se][curr_k] = 1;
        if (u == t) {
            cout << dist << "\n";
            return;
        }
        for (int i = 0; i < 4; i++) {
            pii v = {u.fi + dx[i], u.se + dy[i]};
            if (1 <= v.fi && v.fi <= n && 1 <= v.se && v.se <= m && a[v.fi][v.se] != '#') {
                int w = 1 + !curr_k, x = dist + w, y = (a[v.fi][v.se] != 'C' ? max(0, curr_k - 1) : k);
                if (d[v.fi][v.se][y] > x) {
                    pq.ep(d[v.fi][v.se][y] = x, y, v);
                }
            }
        }
    }
    cout << "-1\n";
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