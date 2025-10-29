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

// -------------------- [Entry Point] --------------------
const int N = 1e5;

int                   n, m, cnt, num[N + 1], low[N + 1], id_scc[N + 1], dist[N + 1];
vector<int>           tr[N + 1];
vector<pii>           a[N + 1];
map<int, vector<int>> scc;
map<int, int>         ap;
map<pii, int>         br;
stack<int>            s;
bitset<N + 1>         vis;

void SCC(int u) {
    // Directed Graph
    num[u] = low[u] = ++cnt;
    s.ep(u);
    for (auto [v, w] : a[u]) {
        if (!num[v]) {
            SCC(v);
            MIN(low[u], low[v]);
        } else if (!id_scc[v]) {
            MIN(low[u], num[v]);
        }
    }
    if (num[u] == low[u]) {
        vector<int> tmp;
        while (1) {
            int x = s.tp();
            s.pp();
            id_scc[x] = sz(scc) + 1;
            tmp.epb(x);
            if (x == u) {
                break;
            }
        }
        reverse(tmp.begin(), tmp.end());
        scc[sz(scc) + 1].epb(tmp);
    }
    return;
}

void APBR(int u, int p = 0) {
    // Undireted Graph
    int c  = 0;
    num[u] = low[u] = ++cnt;
    for (auto [v, w] : a[u]) {
        if (!num[v]) {
            c++;
            APBR(v, u);
            MIN(low[u], low[v]);
            if (p && num[u] <= low[v] && !ap[u]) {
                ap[u] = sz(ap);
            }
            if (num[u] < low[v]) {
                br[{min(u, v), max(u, v)}] = sz(br) + 1;
            }
        } else if (v != p) {
            MIN(low[u], num[v]);
        }
    }
    if (!p && c >= 2 && !ap[u]) {
        ap[u] = sz(ap);
    }
    return;
}

void Dijkstra1(int s, int t) {
    for (int u = 1; u <= n; u++) {
        dist[u] = IMAX;
    }
    pqueue_min<pii> pq;
    pq.ep(dist[s] = 0, s);
    while (sz(pq)) {
        int u = pq.tp().se;
        pq.pp();
        if (vis[u]) {
            continue;
        }
        vis[u] = 1;
        for (auto [v, w] : a[u]) {
            int x = dist[u] + w;
            if (dist[v] > x) {
                pq.ep(dist[v] = x, v);
                tr[v] = {u};
            } else if (dist[v] == x) {
                tr[v].epb(u);
            }
        }
    }
    return;
}

void Dijkstra2(int s, int t) {
    for (int u = 1; u <= n; u++) {
        dist[u] = IMAX;
    }
    dist[s] = 0;
    for (int i = 1; i <= n; i++) {
        int u = 0;
        for (int k = 1; k <= n; k++) {
            if (dist[k] > dist[u] || !u) {
                u = k;
            }
        }
        for (auto [v, w] : a[u]) {
            int x = dist[u] + w;
            MIN(dist[v], x);
        }
    }
    return;
}

void Solve() {
    cin >> n >> m;
    while (m--) {
        int u, v, w;
        cin >> u >> v >> w;
        a[u].epb(v, w);
        a[v].epb(u, w);    // Undirected Graph
    }
    // SCC
    for (int u = 1; u <= n; u++) {
        if (!num[u]) {
            SCC(u);
        }
    }
    cout << "SCC: " << sz(scc) << "\n";
    for (auto [id, vec] : scc) {
        cout << id << ": ";
        for (auto u : vec) {
            cout << u << " ";
        }
        cout << "\n";
    }

    // APBR
    for (int u = 1; u <= n; u++) {
        if (!num[u]) {
            APBR(u);
        }
    }
    cout << "AP: " << sz(ap) << "\n";
    for (auto [id, u] : ap) {
        cout << u << " ";
    }
    cout << "BR: " << sz(br) << "\n";
    for (auto [pr, id] : br) {
        auto [u, v] = pr;
        cout << "[" << u << ", " << v << "] ";
    }

    // Dijkstra1
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