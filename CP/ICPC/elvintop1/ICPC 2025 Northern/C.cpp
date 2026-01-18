#include <bits/stdc++.h>
using namespace std;

#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "
// debug & operator << (debug & dd, P p) { dd << "(" << p.x << ", " << p.y << ")"; return dd; }
#define debug1(...) fprintf(stderr, __VA_ARGS__), fflush(stderr)
#define time__(d) for(long blockTime=NULL;(blockTime==NULL?(blockTime=clock())!=NULL:false);debug1("%s:%.4fs",d,(double)(clock()-blockTime)/CLOCKS_PER_SEC))

typedef long long ll;

#define fi first
#define se second
#define sz(x) (int)x.size()
#define all(x) begin(x), end(x)

const int MOD = 1e9+7; // 998244353
const ll inf = 1e9 + 111000;
const int MX = 2e5+5;
const int dx[] = {0, 1, -1, 0};
const int dy[] = {1, 0, 0, -1};
template<class T> bool ckmin(T &a, T b) { if(a > b) {a = b;return true;} return false;}
template<class T> bool ckmax(T &a, T b) { if(a < b) {a = b;return true;} return false;}


struct HopcroftKarp {
    int nU, nV;
    vector<vector<int>> *adj; 
    vector<int> dist, pairU, pairV;

    void init(int _nU, int _nV, vector<vector<int>> &g) {
        nU = _nU; nV = _nV; adj = &g;
        pairU.assign(nU, -1);
        pairV.assign(nV, -1);
        dist.resize(nU);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < nU; ++u) {
            if (pairU[u] == -1) { 
                dist[u] = 0; 
                q.push(u); 
            }
            else dist[u] = -1;
        }
        bool reachableFreeV = 0;
        while (!q.empty()) {
            int u = q.front(); 
            q.pop();
            for (int v : (*adj)[u]) {
                int u2 = pairV[v];
                if (u2 != -1 && dist[u2] == -1) {
                    dist[u2] = dist[u] + 1;
                    q.push(u2);
                }
                if (u2 == -1) reachableFreeV = 1;
            }
        }
        return reachableFreeV;
    }

    bool dfs(int u) {
        for (int v : (*adj)[u]) {
            int u2 = pairV[v];
            if (u2 == -1 || (dist[u2] == dist[u] + 1 && dfs(u2))) {
                pairU[u] = v;
                pairV[v] = u;
                return 1;
            }
        }
        dist[u] = -1;
        return 0;
    }

    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 0; u < nU; ++u)
                if (pairU[u] == -1 && dfs(u)) ++matching;
        }
        return matching;
    }
};

struct Cell { 
    int i,j,k; 
};

int x,y,z;


void Read() {
    /* input function */
    cin >> x >> y >> z;
}


void Solve() {
    /* your code begin here */
    vector<Cell> ones;
    ones.reserve(x * y * z);

    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            for (int k = 0; k < z; ++k) {
                int v; 
                cin >> v;
                if (v == 1) ones.push_back({i,j,k});
            }
        }
    }

    if (ones.empty()) { 
        cout << 0 << '\n';
        return; 
    }

    int dim[3] = {x,y,z};
    int A = 0;
    if (dim[1] < dim[A]) A = 1;
    if (dim[2] < dim[A]) A = 2;
    int B = (A == 0 ? 1 : 0);
    int C = 3 - A - B; 

    int nA = dim[A], nB = dim[B], nC = dim[C];

    vector<ll> maskBC(nB * nC, 0);
    auto idxBC = [&](int b, int c) -> int { return b * nC + c; };

    for (auto &t : ones) {
        int a, b, c;
        if (A == 0) { 
            a = t.i; 
            b = (B == 1 ? t.j : t.k); 
            c = (C == 1 ? t.j : t.k); 
        }
        else if (A == 1) { 
            a = t.j; 
            b = (B == 0 ? t.i : t.k); 
            c = (C == 0 ? t.i : t.k); 
        }
        else { 
            a = t.k; 
            b = (B == 0 ? t.i : t.j); 
            c = (C == 0 ? t.i : t.j); 
        }
    }
    fill(maskBC.begin(), maskBC.end(), 0);
    for (auto &t : ones) {
        int coord[3] = {t.i, t.j, t.k};
        int a = coord[A], b = coord[B], c = coord[C];
        maskBC[idxBC(b,c)] |= (1ll << a);
    }

    vector<int> activePairs;
    activePairs.reserve(maskBC.size());
    for (int p = 0; p < maskBC.size(); ++p)
        if (maskBC[p]) activePairs.push_back(p);

    int best = INT_MAX;
    vector<vector<int>> adj(nB);
    HopcroftKarp hk;

    ll full = (nA == 64 ? ~0ll : ((1 << nA) - 1));
    ll LIMIT = (nA == 64 ? 0 : (1 << nA));
    for (ll sel = 0; sel < (1 << nA); ++sel) {
        int cntSel = __builtin_popcountll(sel);
        if (cntSel >= best) continue; 
        for (int b = 0; b < nB; ++b) adj[b].clear();
        ll notSel = ~sel & full;

        for (int p : activePairs) {
            if (maskBC[p] & notSel) {
                int b = p / nC, c = p % nC;
                adj[b].push_back(c);
            }
        }

        hk.init(nB, nC, adj);
        int matchSize = hk.maxMatching();
        best = min(best, cntSel + matchSize);
    }

    cout << best << '\n';
}



signed main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t = 1;
    cin >> t;
    while(t--) {
        Read();
        Solve();
    }
    return 0;
}
