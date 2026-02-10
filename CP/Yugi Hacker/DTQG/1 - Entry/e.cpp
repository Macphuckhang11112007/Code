// #include <bits/stdc++.h>

// #define fi first
// #define se second

// using namespace std;

// typedef pair<int, int> pii;

// const int N = 1e5;

// int                            n, ans;
// vector<int>                    id[N + 5];
// vector<pair<int, vector<int>>> v;

// int main() {
//     freopen("gcd.inp", "r", stdin);
//     freopen("gcd.out", "w", stdout);
//     cin.tie(0)->sync_with_stdio(0);
//     cin >> n;
//     int maxx = 0;
//     for (int i = 1; i <= n; i++) {
//         int x;
//         cin >> x;
//         id[x].push_back(i);
//         maxx = max(maxx, x);
//     }
//     for (int i = maxx; i; i--) {
//         int         res = 0;
//         vector<int> pos;
//         for (int j = i; j <= maxx; j += i) {
//             for (int x : id[j]) {
//                 pos.push_back(x);
//             }
//         }
//         sort(pos.begin(), pos.end());
//         pos.erase(unique(pos.begin(), pos.end()), pos.end());
//         if (pos.size() > 1) {
//             for (auto [k, vec] : v) {
//                 if ((vec.size() == 2 && (pos[1] < vec[0] || pos[pos.size() - 2] > vec[1])) || (vec.size() == 3 && (pos[1] < vec[1] || pos[pos.size() - 1] < vec[1] || pos[0] > vec[1] || pos[pos.size() - 2] > vec[1]))) {
//                     ans = max(ans, k + i);
//                     break;
//                 }
//             }
//             if (pos.size() >= 4) {
//                 ans = max(ans, 2 * i);
//             } else {
//                 v.push_back({i, pos});
//             }
//         }
//     }
//     cout << ans;
// }

#include <bits/stdc++.h>

using namespace std;

const int N = 1e5;

int         n, maxx, ans;
vector<int> id[N + 5];
int         max_start[N + 5], min_end[N + 5], suf[N + 5];

int main() {
    freopen("gcd.inp", "r", stdin);
    freopen("gcd.out", "w", stdout);
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        id[x].push_back(i);
        maxx = max(maxx, x);
    }
    for (int i = 1; i <= maxx; i++) {
        vector<int> pos;
        for (int j = i; j <= maxx; j += i) {
            for (int x : id[j]) {
                pos.push_back(x);
            }
        }
        sort(pos.begin(), pos.end());
        pos.erase(unique(pos.begin(), pos.end()), pos.end());
        if (pos.size() > 1) {
            min_end[i]   = pos[1];
            max_start[i] = pos[pos.size() - 2];
        }
    }
    for (int i = 1; i <= maxx; i++) {
        if (max_start[i]) {
            suf[max_start[i]] = max(suf[max_start[i]], i);
        }
    }
    for (int i = n; i; i--) {
        suf[i] = max(suf[i], suf[i + 1]);
    }
    for (int i = 1; i <= maxx; i++) {
        if (min_end[i] && suf[min_end[i] + 1]) {
            ans = max(ans, i + suf[min_end[i] + 1]);
        }
    }
    cout << ans;
}
