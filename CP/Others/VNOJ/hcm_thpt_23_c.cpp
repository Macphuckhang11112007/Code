#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

const int N = 5e4;

struct rec {
    int id;
    int x1, y1, x2, y2;

    rec() : id(0), x1(0), y1(0), x2(0), y2(0) {}

    rec(int _id, int _x1, int _y1, int _x2, int _y2) : id(_id), x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
} a[N + 5], b[N + 5];

int                                      n, ans, par[N + 5], area[N + 5];
map<int, pair<vector<int>, vector<int>>> points_a, points_b;

int find_par(int u) {
    return (u == par[u] ? u : par[u] = find_par(par[u]));
}

void dsu(int u, int v) {
    int pu = find_par(u), pv = find_par(v);
    if (pu == pv) { return; }
    if (area[pu] < area[pv]) { swap(pu, pv); }
    par[pv] = pu;
    area[pu] += area[pv];
    ans = max(ans, area[pu]);
}

void sort_points(auto &arr, auto &vec) {
    auto comp = [&](int i, int j) {
        if (arr[i].y1 != arr[j].y1) { return arr[i].y1 < arr[j].y1; }
        return arr[i].id < arr[j].id;
    };
    sort(vec.begin(), vec.end(), comp);
}

void sweep_line(auto &arr, auto &points) {
    for (auto &[key, val] : points) {
        vector<int> &en = val.fi;
        vector<int> &st = val.se;
        if (en.empty() || st.empty()) { continue; }
        sort_points(arr, en);
        sort_points(arr, st);
        int j = 0;
        for (int i = 0; i < en.size(); i++) {
            while (j < st.size() && arr[en[i]].y1 > arr[st[j]].y2) { j++; }
            int k = j;
            while (k < st.size() && arr[en[i]].y2 >= arr[st[k]].y1) { dsu(en[i], st[k++]); }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x1, y1, x2, y2, c, d;
        cin >> x1 >> y1 >> c >> d;
        x2      = x1 + c;
        y2      = y1 + d;
        a[i]    = rec(i, x1, y1, x2, y2);
        b[i]    = rec(i, y1, x1, y2, x2);
        par[i]  = i;
        area[i] = c * d;
        ans     = max(ans, c * d);
        points_a[x1].se.push_back(i);
        points_a[x2].fi.push_back(i);
        points_b[y1].se.push_back(i);
        points_b[y2].fi.push_back(i);
    }
    sweep_line(a, points_a);
    sweep_line(b, points_b);
    cout << ans;
}
