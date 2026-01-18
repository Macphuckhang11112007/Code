


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


int ask(int idx) {
    cout << "? " << idx << endl; 
    int c; 
    cin >> c;
    return c; 
}


void Read() {
    /* input function */
    cin >> n;
}


void Solve() {
    /* your code begin here */
    vector<int> perm;
    perm.reserve(6);
    vector<bool> vis(7, 0);
    vector<int> col1(n + 3), cnt1(7 , 0);
    for (int b = 1; b <= n; ++b) {
        int color = ask(6 * (b - 1) + 1);
        col1[b] = color;
        cnt1[color]++;
    }
    int c1 = 1;
    for (int c = 2; c <= 6; ++c) {
        if (cnt1[c] < cnt1[c1]) c1 = c;
    }
    perm.push_back(c1); 
    vis[c1] = 1;

    vector<int> cur;
    for (int b = 1; b <= n; ++b) {
        if (col1[b] == c1) cur.push_back(b);
    }

    for (int pos = 2; pos <= 4; ++pos) {
        vector<int> freq(7, 0);
        vector<int> val(cur.size());
        for (int i = 0; i < cur.size(); ++i) {
            int b = cur[i];
            int color = ask(6 * (b - 1) + pos);
            val[i] = color;
            freq[color]++;
        }
        int choose = -1, bestCnt = INT_MAX;
        for (int c = 1; c <= 6; ++c) {
            if (vis[c]) continue;
            if (freq[c] < bestCnt) { bestCnt = freq[c]; choose = c; }
        }
        perm.push_back(choose); 
        vis[choose] = 1;
        vector<int> next;
        for (int i = 0; i < cur.size(); ++i)
            if (val[i] == choose) next.push_back(cur[i]);
        cur.swap(next);
    }

    int c5;
    if (cur.empty()) {
        for (int c = 1; c <= 6; ++c) {
            if (!vis[c]) { 
                c5 = c;
                break; 
            }
        }
    } else {
        int b = cur[0];
        int color5 = ask(6 * (b - 1) + 5);
        int a = -1, bcolor = -1;
        for (int c = 1; c <= 6; ++c) {
            if (!vis[c]) { 
                (a == -1 ? a : bcolor) = c; 
            }
        }
        c5 = (color5 == a) ? bcolor : a;
    }
    perm.push_back(c5); 
    vis[c5] = 1;

    for (int c = 1; c <= 6; ++c) {
        if (!vis[c]) { 
            perm.push_back(c);  
        }
    }

    cout << "! ";
    for(int v : perm) cout << v << ' ' ;
    cout << endl;
    cout.flush();
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
