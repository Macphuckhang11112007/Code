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
const int dr[] = {0, 1, -1, 0};
const int dc[] = {1, 0, 0, -1};
template<class T> bool ckmin(T &a, T b) { if(a > b) {a = b;return true;} return false;}
template<class T> bool ckmax(T &a, T b) { if(a < b) {a = b;return true;} return false;}


int R,C;
string g[MX];
bool vis[100][100][4];

struct State {
    int r, c, hp;
};


void Read() {
    /* input function */
    cin >> R >> C;
    for(int i = 0;i < R;++i) cin >> g[i];
}


void Solve() {
    /* your code begin here */
    queue<State> q;
    vector<vector<array<int, 4>>> dist(R, vector<array<int, 4>>(C, { -1, -1, -1, -1 }));
    int sr = -1, sc = -1;
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            if (g[i][j] == 'S') { sr = i; sc = j; }

    q.push({sr, sc, 3});
    dist[sr][sc][3] = 0;

    while (!q.empty()) {
        auto [r, c, hp] = q.front();
        q.pop();

        if (g[r][c] == 'D') {
            cout << dist[r][c][hp] << '\n';
            return;
        }

        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            int nhp = hp - (g[nr][nc] == '+' ? 1 : 0);
            if (nhp == 0) continue;
            if (dist[nr][nc][nhp] != -1) continue;
            dist[nr][nc][nhp] = dist[r][c][hp] + 1;
            q.push({nr, nc, nhp});
        }
    }
    cout << -1 << '\n';
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

