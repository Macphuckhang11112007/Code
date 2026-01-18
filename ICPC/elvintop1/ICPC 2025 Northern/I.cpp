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


ll n,x,k;


void Read() {
    /* input function */
    cin >> n >> k >> x;
}


void Solve() {
    /* your code begin here */
    int s0 = k * (k + 1) / 2;
    int l = (n == 0 ? 0 : 64 - __builtin_clzll(n));
    if(l == 0) {
        cout << (x == 0 ? 1 : 0) << '\n';
        return;
    }
    if ((x >> l) != 0) {
        cout << 0 << '\n';
        return;
    }
    vector<int> cnt(s0 + 1, 0);
    cnt[0] = 1;
    for(int i = 1;i <= (int)k;++i) {
        for(int s = s0;s >= i;--s) {
            cnt[s] = (cnt[s - i] + cnt[s]) % MOD;
        }
    }

    int cmax = s0  + 1;
    vector<int> dp(cmax + 1, 0), ndp(cmax + 1, 0);
    dp[0] = 1;
    for(int b = 0;b < l;++b) {
        int nb = (int)((n >> b) & 1ll);
        int xb = (int)((x >> b) & 1ll);
        fill(all(ndp), 0);

        if (xb == 1) {
            int s = s0;
            for (int c = 0; c <= cmax; ++c) {
                int ways = dp[c];
                if (!ways) continue;
                int u = ((s + c) & 1) ^ nb;   
                int c2 = (s + c + u) >> 1;    
                ndp[c2] = (ndp[c2] + ways) % MOD;
            }
        } 
        else {
            for (int c = 0; c <= cmax; ++c) {
                ll ways_c = dp[c];
                if (!ways_c) continue;
                for (int s = 0; s < s0; ++s) {
                    int mult = cnt[s];  
                    if (!mult) continue;
                    int u = ((s + c) & 1) ^ nb;
                    int c2 = (s + c + u) >> 1;
                    ll add = (ways_c * mult) % MOD;
                    ndp[c2] = (ndp[c2] + add) % MOD;
                }
            }
        }

        dp.swap(ndp);
    }
    cout << dp[0] << '\n';
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
