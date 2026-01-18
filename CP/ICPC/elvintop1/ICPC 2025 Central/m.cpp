

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
const ll INF = LLONG_MAX / 4;
const int MX = 2e5+5;
const int dx[] = {0, 1, -1, 0};
const int dy[] = {1, 0, 0, -1};
template<class T> bool ckmin(T &a, T b) { if(a > b) {a = b;return true;} return false;}
template<class T> bool ckmax(T &a, T b) { if(a < b) {a = b;return true;} return false;}

int n;
vector<int> xs, ys;



vector<ll> shipCost(vector<vector<int>>& dist, int baseIdx)
{
    vector<vector<ll>> dp(1 << n, vector<ll>(n, INF));
    vector<ll> cost(1 << n, 0);

    for (int i = 0; i < n; ++i)
        dp[1 << i][i] = 0;

    for (int mask = 1; mask < 1 << n; ++mask) {
        int visited = __builtin_popcount(mask);
        for (int last = 0; last < n; ++last) {
            if (dp[mask][last] == INF) continue;

            for (int nxt = 0; nxt < n; ++nxt) {
                if (mask & (1 << nxt)) continue;
                int nmask = mask | (1 << nxt);
                ll cand = dp[mask][last] + 1LL * visited * dist[last][nxt];
                dp[nmask][nxt] = min(dp[nmask][nxt], cand);
            }
        }
    }

    cost[0] = 0;               
    for (int mask = 1; mask < 1 << n; ++mask) {
        ll best = INF;
        int sz = __builtin_popcount(mask);
        for (int last = 0; last < n; ++last)
            if (mask & (1 << last) && dp[mask][last] != INF)
                best = min(best, dp[mask][last] + 1LL * sz * dist[last][baseIdx]);
        cost[mask] = best;
    }
    return cost;
}


void Read() {
    /* input function */
    cin >> n;
    xs.reserve(n);
    ys.reserve(n);
    for (int i = 0; i < n; ++i) cin >> xs[i] >> ys[i];
}


void Solve() {
    /* your code begin here */
    int xA, yA, xB, yB;
    cin >> xA >> yA >> xB >> yB;
    vector<pair<int,int>> pts;
    for (int i = 0; i < n; ++i) pts.push_back({xs[i], ys[i]});
    pts.push_back({xA, yA});
    pts.push_back({xB, yB});

    vector<vector<int>> dist(n + 2, vector<int>(n + 2));
    for (int i = 0; i < n + 2; ++i)
        for (int j = 0; j < n + 2; ++j)
            dist[i][j] = abs(pts[i].first  - pts[j].first) + abs(pts[i].second - pts[j].second);

    vector<ll> costA = shipCost(dist, n);
    vector<ll> costB = shipCost(dist, n + 1);
    ll ans = INF;
    for (int mask = 0; mask < 1 << n; ++mask)
        ans = min(ans, costA[mask] + costB[(1 << n) - 1 - mask]);

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
