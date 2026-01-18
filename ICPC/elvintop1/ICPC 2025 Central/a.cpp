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


int n,k;
int a[MX];


int addmod(int a, int b) {
    int s = a + b;
    if (s >= MOD) s -= MOD;
    return s;
}

int mulmod(ll a, ll b) {
    return int((a * b) % MOD);
}

void Read() {
    /* input function */
    cin >> n >> k;
    for (int i = 0; i < n; ++i) cin >> a[i];
}


void Solve() {
    /* your code begin here */
    vector<int> g(max(0, n - 1));
    for (int i = 0; i+1 < n; ++i) g[i] = a[i] - a[i + 1] + 1;

    vector<vector<int>> C(k + 1, vector<int>(k + 1, 0));
    for (int i = 0; i <= k; ++i) {
        C[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            C[i][j] = addmod(C[i - 1][j - 1], C[i - 1][j]);
        }
    }

    int dp[2][21][21][21]; 
    memset(dp, 0, sizeof dp);
    int cur = 0, nxt = 1;
    dp[cur][0][0][0] = 1;

    for (int i = 0; i < n; ++i) {
        memset(dp[nxt], 0, sizeof dp[nxt]);
        for (int S = 0; S <= k; ++S) {
            for (int O = 0; O <= k; ++O) {
                for (int L = 0; L <= k; ++L) {
                    int wsf = dp[cur][S][O][L];
                    if (!wsf) continue;

                    int sMin = max(0, L);
                    int smax = k - S;
                    if (sMin > smax) continue;

                    if (i < n - 1) {
                        for (int s = sMin; s <= smax; ++s) {
                            int ps = C[k - S][s];
                            if (!ps) continue;
                            int s2 = S + s;
                            int osum = O + s; 
                            for (int t = 0; t <= osum; ++t) {
                                int pe = C[osum][t];
                                if (!pe) continue;

                                int o2 = osum - t;
                                int l2 = g[i] + t;
                                if (l2 < 0) l2 = 0;

                                if (l2 > k - s2) continue;

                                int add = mulmod(wsf, ps);
                                add = mulmod(add, pe);
                                dp[nxt][s2][o2][l2] = addmod(dp[nxt][s2][o2][l2], add);
                            }
                        }
                    } else {
                        for (int s = sMin; s <= smax; ++s) {
                            if (S + s != k) continue;        
                            int ps = C[k - S][s];
                            if (!ps) continue;
                            int add = mulmod(wsf, ps);
                            dp[nxt][k][0][0] = addmod(dp[nxt][k][0][0], add);
                        }
                    }
                }
            }
        }
        swap(cur, nxt);
    }

    cout << dp[cur][k][0][0] % MOD << '\n';
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
