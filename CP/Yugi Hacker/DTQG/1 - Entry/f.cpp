#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

typedef pair<int, int> pii;

const int N = 2000;

int n, a[N + 5], id[N + 5], pos[N + 5];
pii seg[4 * N + 5];
int par[N + 5];

void update(int p, int l, int r, int i, int x, int idx) {
    if (l == r) {
        seg[p] = {x, idx};
        return;
    }
    int mid = (l + r) / 2;
    if (i <= mid) {
        update(2 * p, l, mid, i, x, idx);
    } else {
        update(2 * p + 1, mid + 1, r, i, x, idx);
    }
    seg[p] = max(seg[2 * p], seg[2 * p + 1]);
}

pii query(int p, int l, int r, int u, int v) {
    if (r < u || l > v) {
        return {0, 0};
    }
    if (u <= l && r <= v) {
        return seg[p];
    }
    int mid = (l + r) / 2;
    return max(query(2 * p, l, mid, u, v), query(2 * p + 1, mid + 1, r, u, v));
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[i] = i;
    }
    sort(pos + 1, pos + n + 1, [](int i, int j) { return (a[i] != a[j] ? a[i] < a[j] : i < j); });
    for (int i = 1, cnt = 0; i <= n; i++) {
        if (i == 1 || a[pos[i]] != a[pos[i - 1]]) {
            cnt++;
        }
        id[pos[i]] = cnt;
    }
    pii p;
    for (int i = 1; i <= n; i++) {
        auto [x, idx] = query(1, 1, n, 1, id[i] - 1);
        par[i]        = idx;
        p             = max(p, {x + 1, i});
        update(1, 1, n, id[i], x + 1, i);
    }
    vector<int> lis;
    while (p.se) {
        lis.push_back(a[p.se]);
        p.se = par[p.se];
    }
    reverse(lis.begin(), lis.end());
    cout << p.fi << "\n";
    for (auto x : lis) {
        cout << x << " ";
    }
}
