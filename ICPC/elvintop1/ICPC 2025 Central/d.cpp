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

string s;
string word[] = {
    "", "one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen","twenty"
};

void Read() {
    /* input function */
    cin >> s;
    
}


void Solve() {
    /* your code begin here */
    unordered_map<string,int> exact;
    for (int i = 1; i <= 20; ++i) exact[word[i]] = i;
    if (isdigit(s[0])) {
        int val = stoi(s);
        cout << word[val] << '\n';
        return;
    }
    auto it = exact.find(s);
    if (it != exact.end()) {
        cout << it->second << '\n';
        return;
    }
    for (int i = 1; i <= 20; ++i) {
        if (s.size() != word[i].size()) continue;
        int diff = 0;
        for (size_t k = 0; k < s.size() && diff <= 1; ++k)
            diff += (s[k] != word[i][k]);
        if (diff == 1) {           
            cout << i << '\n';
            return;
        }
    }
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

