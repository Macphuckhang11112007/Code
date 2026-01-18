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

struct bigint {
    static constexpr uint32_t BASE = 1000000000;
    static constexpr int BASE_DIGS = 9;

    vector<uint32_t> d; 
    int sign;           

    bigint(): sign(1) {}
    bigint(long long v) { 
        *this = v; 
    }
    explicit bigint(const string &s) { 
        read(s); 
    }

    bigint& operator=(long long v) {
        d.clear(); sign = 1;
        if (v < 0) sign = -1, v = -v;
        while (v) { 
            d.push_back((uint32_t)(v % BASE)); 
            v /= BASE; 
        }
        return *this;
    }
    void read(const string &s) {
        sign = 1; d.clear();
        int i = 0;
        while (i < (int)s.size() && isspace((unsigned char)s[i])) ++i;
        if (i < (int)s.size() && (s[i] == '-' || s[i] == '+')) sign = (s[i++] == '-') ? -1 : 1;
        for (int j = (int)s.size()-1; j >= i; j -= BASE_DIGS) {
            int x = 0;
            int L = max(i, j - BASE_DIGS + 1);
            for (int k = L; k <= j; ++k) x = x * 10 + (s[k] - '0');
            d.push_back((uint32_t)x);
        }
        trim();
    }

    bool isZero() const { 
        return d.empty(); 
    }
    void trim() {
        while (!d.empty() && d.back() == 0) d.pop_back();
        if (d.empty()) sign = 1;
    }
    static int cmpAbs(const bigint &a, const bigint &b) {
        if (a.d.size() != b.d.size()) return (a.d.size() < b.d.size()) ? -1 : 1;
        for (int i = (int)a.d.size() - 1; i >= 0; --i)
            if (a.d[i] != b.d[i]) return (a.d[i] < b.d[i]) ? -1 : 1;
        return 0;
    }
    static bigint abs(bigint x) { 
        x.sign = 1; 
        return x; 
    }

    friend bool operator==(const bigint& a, const bigint& b) {
        return a.sign == b.sign && a.d == b.d;
    }
    friend bool operator!=(const bigint& a, const bigint& b) { 
        return !(a == b); 
    }
    friend bool operator<(const bigint& a, const bigint& b) {
        if (a.sign != b.sign) return a.sign < b.sign;
        int c = cmpAbs(a, b);
        return (a.sign == 1) ? (c < 0) : (c > 0);
    }
    friend bool operator>(const bigint& a, const bigint& b) { 
        return b < a; 
    }
    friend bool operator<=(const bigint& a, const bigint& b) { 
        return !(b < a); 
    }
    friend bool operator>=(const bigint& a, const bigint& b) { 
        return !(a < b); 
    }

    friend ostream& operator<<(ostream& os, const bigint& v) {
        if (v.isZero()) { 
            os << 0; 
            return os; 
        }
        if (v.sign == -1) os << '-';
        os << v.d.back();
        for (int i = (int)v.d.size()-2; i >= 0; --i)
            os << setw(BASE_DIGS) << setfill('0') << v.d[i];
        return os;
    }

    static void addAbs(bigint &a, const bigint &b) {
        uint64_t carry = 0;
        size_t n = max(a.d.size(), b.d.size());
        if (a.d.size() < n) a.d.resize(n, 0);
        for (size_t i = 0; i < n; ++i) {
            uint64_t sum = carry + a.d[i] + (i < b.d.size() ? b.d[i] : 0ULL);
            a.d[i] = (uint32_t)(sum % BASE);
            carry = sum / BASE;
        }
        if (carry) a.d.push_back((uint32_t)carry);
    }
    static void subAbs(bigint &a, const bigint &b) {
        int64_t carry = 0;
        for (size_t i = 0; i < a.d.size(); ++i) {
            int64_t cur = (int64_t)a.d[i] - (i < b.d.size() ? (int64_t)b.d[i] : 0LL) - carry;
            if (cur < 0) cur += BASE, carry = 1; else carry = 0;
            a.d[i] = (uint32_t)cur;
        }
        a.trim();
    }

    bigint operator-() const { 
        bigint r = *this; 
        if (!r.isZero()) r.sign = -r.sign; 
        return r; 
    }

    friend bigint operator+(bigint a, const bigint& b) { 
        a += b; 
        return a; 
    }
    friend bigint operator-(bigint a, const bigint& b) { 
        a -= b; 
        return a; 
    }
    friend bigint operator*(bigint a, const bigint& b) { 
        a *= b; 
        return a; 
    }
    friend bigint operator/(bigint a, const bigint& b) { 
        a /= b; 
        return a; 
    }
    friend bigint operator%(bigint a, const bigint& b) { 
        a %= b; 
        return a; 
    }

    bigint& operator+=(const bigint& v) {
        if (v.isZero()) return *this;
        if (sign == v.sign) {
            addAbs(*this, v);
        } else {
            int c = cmpAbs(*this, v);
            if (c == 0) { 
                d.clear(); 
                sign = 1; 
            }
            else if (c > 0) subAbs(*this, v);
            else { 
                bigint tmp = v; 
                subAbs(tmp, *this); 
                *this = move(tmp); 
            }
        }
        return *this;
    }
    bigint& operator-=(const bigint& v) {
        if (v.isZero()) return *this;
        if (sign != v.sign) {
            addAbs(*this, v);
        } 
        else {
            int c = cmpAbs(*this, v);
            if (c == 0) { 
                d.clear(); 
                sign = 1;
            }
            else if (c > 0) subAbs(*this, v);
            else { 
                bigint tmp = v; 
                subAbs(tmp, *this); 
                tmp.sign = -tmp.sign; 
                *this = move(tmp); 
            }
        }
        return *this;
    }

    void mul_uint(uint32_t m) {
        if (isZero() || m == 1) return;
        if (m == 0) { 
            d.clear(); 
            sign = 1; 
            return; 
        }
        uint64_t carry = 0;
        for (int i = 0; i < d.size(); ++i) {
            uint64_t cur = carry + (uint64_t)d[i] * m;
            d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        if (carry) d.push_back((uint32_t)carry);
    }

    void add_uint(uint32_t v) {
        uint64_t carry = v;
        size_t i = 0;
        while (carry) {
            if (i == d.size()) d.push_back(0);
            uint64_t cur = (uint64_t)d[i] + carry;
            d[i] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
            ++i;
        }
    }

    bigint& operator*=(const bigint& v) {
        if (isZero() || v.isZero()) { 
            d.clear(); 
            sign = 1; 
            return *this; 
        }
        vector<uint32_t> res(d.size() + v.d.size(), 0);
        for (size_t i = 0; i < d.size(); ++i) {
            uint64_t carry = 0;
            for (size_t j = 0; j < v.d.size() || carry; ++j) {
                uint64_t cur = res[i + j] + carry;
                if (j < v.d.size()) cur += (uint64_t)d[i] * v.d[j];
                res[i + j] = (uint32_t)(cur % BASE);
                carry = cur / BASE;
            }
        }
        d.swap(res);
        sign *= v.sign;
        trim();
        return *this;
    }

    static void divmodAbs(const bigint& a, const bigint& b, bigint& q, bigint& r) {
        q.d.assign(a.d.size(), 0);
        q.sign = 1;
        r.d.clear(); r.sign = 1;

        if (cmpAbs(a, b) < 0) { q.trim(); r = a; return; }
        bigint divisor = b; 

        for (int i = (int)a.d.size() - 1; i >= 0; --i) {
            r.d.insert(r.d.begin(), 0); 
            if (!r.d.empty()) {
                uint64_t cur = (uint64_t)r.d[0] + a.d[i];
                r.d[0] = (uint32_t)(cur % BASE);
                uint64_t carry = cur / BASE;
                size_t k = 1;
                while (carry) {
                    if (k == r.d.size()) r.d.push_back(0);
                    uint64_t cur2 = (uint64_t)r.d[k] + carry;
                    r.d[k] = (uint32_t)(cur2 % BASE);
                    carry = cur2 / BASE;
                    ++k;
                }
            } else {
                r.d.push_back(a.d[i]);
            }
            r.trim();

            uint32_t lo = 0, hi = BASE - 1, best = 0;
            while (lo <= hi) {
                uint32_t mid = lo + (hi - lo) / 2;
                bigint t = divisor; t.mul_uint(mid);
                if (cmpAbs(t, r) <= 0) { 
                    best = mid; 
                    lo = mid + 1; 
                }
                else hi = (mid == 0 ? 0 : mid - 1);
                if (mid == 0 && hi == 0 && lo == 0) break; 
            }
            q.d[i] = best;
            if (best) {
                bigint t = divisor; 
                t.mul_uint(best);
                subAbs(r, t);
            }
        }
        q.trim(); 
        r.trim();
    }

    bigint& operator/=(const bigint& v) {
        bigint a = bigint::abs(*this), b = bigint::abs(v), q, r;
        divmodAbs(a, b, q, r);
        q.sign = (q.isZero() ? 1 : sign * v.sign);
        *this = move(q);
        return *this;
    }
    bigint& operator%=(const bigint& v) {
        bigint a = bigint::abs(*this), b = bigint::abs(v), q, r;
        divmodAbs(a, b, q, r);
        *this = move(r); 
        return *this;
    }

    static bigint gcd(bigint a, bigint b) {
        a.sign = 1; b.sign = 1;
        while (!b.isZero()) {
            a %= b;
            swap(a, b);
        }
        return a;
    }

    static bigint pow_uint(bigint base, unsigned exp) {
        bigint res = 1;
        while (exp) {
            if (exp & 1U) res *= base;
            if (exp >>= 1U) base *= base;
        }
        return res;
    }
};



int n,k;

void Read() {
    /* input function */
    cin >> n >> k;
}


void Solve() {
    /* your code begin here */
    bigint a,b; 
    if (n > k) {
        a = 0;
        b = 1;
    } else {
        bigint s = k - n;
        a = (s + 1) * a.pow_uint(k + 1, n - 1);
        b = b.pow_uint(k, n);

        bigint g = a.gcd(a, b);
        a /= g; 
        b /= g;
    }

    cout << a << ' ' << b << '\n';
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


