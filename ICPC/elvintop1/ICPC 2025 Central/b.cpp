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


int n;
vector<pair<int,int>> g[MX];
ll cnt1[26] = {0};

void Read() {
    /* input function */
    cin >> n;
    for (int i = 0; i < n - 1; ++i) {
        int u, v, w; 
        cin >> u >> v >> w;
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
}


void Solve() {
    /* your code begin here */
    vector<int> X(n + 1, 0);
    vector<int> st = {1};
    vector<int> parent(n + 1, 0);
    parent[1] = -1;
    while (!st.empty()) {
        int u = st.back(); st.pop_back();
        for (auto [v, w] : g[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            X[v] = X[u] ^ w;
            st.push_back(v);
        }
    }
                

    for (int i = 1; i <= n; ++i)
        for (int b = 0; b < 26; ++b)
            if (X[i] & (1 << b)) ++cnt1[b];

    ll ans = 0;
    for (int b = 0; b < 26; ++b) {
        ll ones = cnt1[b];
        ll zeros = n - ones;
        ans += (1ll << b) * ones * zeros;
    }
    cout << ans << '\n';
}



signed main(void)
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t = 1;
    //cin >> t;
    while(t--) {
        Read();
        Solve();
    }
    return 0;
}

