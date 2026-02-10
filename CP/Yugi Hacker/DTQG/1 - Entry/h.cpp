#include <bits/stdc++.h>

using namespace std;

const long long N = 1e6;

long long n, q;
long long a[N + 5];
long long fen1[N + 5], fen2[N + 5];

void pointUpdate(auto &fen, long long i, long long x) {
    for (; i <= n; i += i & -i) {
        fen[i] += x;
    }
}

void rangeUpdate(long long l, long long r, long long x) {
    pointUpdate(fen1, l, x);
    pointUpdate(fen1, r + 1, -x);
    pointUpdate(fen2, l, x * (l - 1));
    pointUpdate(fen2, r + 1, -x * r);
}

long long pointQuery(auto &fen, long long i) {
    long long res = 0;
    for (; i; i -= i & -i) {
        res += fen[i];
    }
    return res;
}

long long rangeQuery(long long l, long long r) {
    return pointQuery(fen1, r) * r - pointQuery(fen2, r) - (pointQuery(fen1, l - 1) * (l - 1) - pointQuery(fen2, l - 1));
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            long long l, r, x;
            cin >> l >> r >> x;
            rangeUpdate(l, r, x);
        } else {
            long long l, r;
            cin >> l >> r;
            cout << rangeQuery(l, r) << "\n";
        }
    }
}
