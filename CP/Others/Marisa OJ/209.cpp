#include <bits/stdc++.h>

using namespace std;

const int N = 1e5;

int                          n, q, a[N + 5], id[N + 5], tree[N + 5], qr[N + 5];
vector<tuple<int, int, int>> vqr;
map<int, int>                pos;

void update(int i, int x) {
    for (; i && i <= n; i += i & -i) { tree[i] += x; }
}

int get(int i) {
    int res = 0;
    for (; i; i -= i & -i) { res += tree[i]; }
    return res;
}

int query(int l, int r) {
    return get(r) - get(l - 1);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    for (int i = 1; i <= n; i++) { cin >> a[i]; }
    for (int i = 1; i <= q; i++) {
        int l, r;
        cin >> l >> r;
        vqr.push_back({r, l, i});
    }
    sort(vqr.begin(), vqr.end());
    int k = 1;
    for (auto &[r, l, idx] : vqr) {
        for (; k <= r; k++) {
            update(pos[a[k]], -1);
            update(k, 1);
            pos[a[k]] = k;
        }
        qr[idx] = query(l, r);
    }
    for (int i = 1; i <= q; i++) { cout << qr[i] << "\n"; }
}
