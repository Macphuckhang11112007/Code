#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

using ll  = long long;
using pii = pair<int, int>;

const int N = 1e4;

struct seg {
    int cnt, len;
} tree[8 * N + 5];

int                      n;
ll                       ans;
multiset<pair<pii, pii>> points_x;
set<int>                 points_y;
unordered_map<int, int>  id, val;

int calc_len(int p, int l, int r) {
    if (tree[p].cnt) { return val[r + 1] - val[l]; }
    return (l != r ? tree[p << 1].len + tree[p << 1 | 1].len : 0);
}

void update(int p, int l, int r, int u, int v, int x) {
    if (l > r || u > v || v < l || r < u) { return; }
    if (u <= l && r <= v) {
        tree[p].cnt += x;
        tree[p].len = calc_len(p, l, r);
        return;
    }
    int mid = (l + r) >> 1;
    update(p << 1, l, mid, u, v, x);
    update(p << 1 | 1, mid + 1, r, u, v, x);
    tree[p].len = calc_len(p, l, r);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        points_x.insert({{x1, 1}, {y1, y2}});
        points_x.insert({{x2, -1}, {y1, y2}});
        points_y.insert(y1);
        points_y.insert(y2);
    }
    int cnt = 0;
    for (int i : points_y) {
        id[i]    = ++cnt;
        val[cnt] = i;
    }
    for (auto p = points_x.begin(); p != prev(points_x.end()); p++) {
        int x1 = p->fi.fi, type = p->fi.se, y1 = p->se.fi, y2 = p->se.se;
        int u = id[y1], v = id[y2] - 1;
        update(1, 1, cnt - 1, u, v, type);
        int x2 = next(p)->fi.fi;
        ans += 1LL * (x2 - x1) * tree[1].len;
    }
    cout << ans;
}
