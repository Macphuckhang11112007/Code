#include <bits/stdc++.h>

using namespace std;

const int N = 2000;

int n, a[N + 5], tr[N + 5][N + 5];
int ans[3];

// // Greedy + Lowerrbound (STL) O(nlogn)
// vector<int> lis;

// // DP Segment Tree O(nlogn)
// struct Seg
// {
//     int val  = 0;
//     int lazy = 0;
//     Seg() : val(0), lazy(0) {}
//     Seg(int _val, int _lazy) : val(_val), lazy(_lazy) {}
//     bool operator<(Seg &j) { return tie(val, lazy) < tie(j.val, j.lazy); }
//     Seg  max(const Seg &i, const Seg &j)
//     {
//         if (tie(i.val, i.lazy) > tie(j.val, j.lazy)) { return i; }
//         else { return j; }
//     }
// } seg[4 * N + 5];
// int                     arr[N + 5], dp[N + 5];
// unordered_map<int, int> idx;

// void Max(int &a, int &b, bool chk = 0)
// {
//     if (!chk) { a = max(a, b); }
//     else { b = max(a, b); }
// }

// void Build(int p, int l, int r)
// {
//     if (l > r) { return; }
//     if (l == r) { seg[p] = Seg(0, 0); }
//     else
//     {
//         int m = l + (r - l) / 2;
//         int u = 2 * p, v = 2 * p + 1;
//         Build(u, l, m);
//         Build(v, m + 1, r);
//         seg[p] = Seg(max(seg[u].val, seg[v].val), 0);
//     }
//     return;
// }

// void Lazy(int p)
// {
//     if (seg[p].lazy)
//     {
//         int u = 2 * p, v = 2 * p + 1;
//         int x = max(seg[p].lazy, seg[u].val), y = max(seg[p].lazy,
//         seg[v].val); Max(seg[p].val, seg[p].lazy); seg[u]      = Seg(x, x);
//         seg[v]      = Seg(y, y);
//         seg[p].lazy = 0;
//     }
//     return;
// }

// void Update(int p, int l, int r, int i, int j, int x)
// {
//     if (l > r || j < l || r < i) { return; }
//     if (i <= l && r <= j)
//     {
//         Max(seg[p].val, x);
//         Max(seg[p].lazy, x);
//         return;
//     }
//     else
//     {
//         int m = l + (r - l) / 2;
//         int u = 2 * p, v = 2 * p + 1;
//         Lazy(p);
//         Update(u, l, m, i, j, x);
//         Update(v, m + 1, r, i, j, x);
//         seg[p].val = max(seg[u].val, seg[v].val);
//     }
//     return;
// }

// int Get(int p, int l, int r, int i, int j)
// {
//     if (l > r || j < l || r < i || i > j) { return 0; }
//     if (i <= l && r <= j) { return seg[p].val; }
//     else
//     {
//         int m = l + (r - l) / 2;
//         int u = 2 * p, v = 2 * p + 1;
//         Lazy(p);
//         return max(Get(u, l, m, i, j), Get(v, m + 1, r, i, j));
//     }
//     return 0;
// }

// // DP Sqrt Decomposition O(n*sqrt(n))
// const int S = 450;

// struct SQD
// {
//     int val  = 0;
//     int lazy = 0;
//     SQD() : val(0), lazy(0) {}
//     SQD(int _val, int _lazy) : val(_val), lazy(_lazy) {}
// } sq[N / S + 5];

// int                     arr[N + 5], dp[N + 5], b[N + 5];
// unordered_map<int, int> idx;

// void Lazy(int p)
// {
//     if (sq[p].lazy)
//     {
//         for (int i = (p - 1) * S + 1; i <= min(p * S, int(idx.size())); i++)
//         {
//             b[i] = max(b[i], sq[p].lazy);
//         }
//         sq[p].lazy = 0;
//     }
//     return;
// }

// void Update(int l, int r, int x)
// {
//     int block_l = (l - 1) / S + 1, block_r = (r - 1) / S + 1;
//     if (block_l == block_r)
//     {
//         Lazy(block_l);
//         for (int i = l; i <= r; i++)
//         {
//             b[i]            = max(b[i], x);
//             sq[block_l].val = max(sq[block_l].val, x);
//         }
//     }
//     else
//     {
//         int low = block_l * S, high = (block_r - 1) * S + 1;
//         Lazy(block_l);
//         for (int i = l; i <= low; i++)
//         {
//             b[i]            = max(b[i], x);
//             sq[block_l].val = max(sq[block_l].val, x);
//         }
//         for (int i = block_l + 1; i < block_r; i++)
//         {
//             sq[i] = SQD(max(sq[i].val, x), x);
//         }
//         Lazy(block_r);
//         for (int i = high; i <= r; i++)
//         {
//             b[i]            = max(b[i], x);
//             sq[block_r].val = max(sq[block_r].val, x);
//         }
//     }
//     return;
// }

// int Get(int l, int r)
// {
//     if (l > r) { return 0; }
//     int block_l = (l - 1) / S + 1;
//     int block_r = (r - 1) / S + 1;
//     int res     = 0;

//     if (block_l == block_r)
//     {
//         Lazy(block_l);
//         for (int i = l; i <= r; i++) { res = max(res, b[i]); }
//     }
//     else
//     {
//         Lazy(block_l);
//         int low = block_l * S, high = (block_r - 1) * S + 1;
//         for (int i = l; i <= low; ++i) { res = max(res, b[i]); }

//         for (int i = block_l + 1; i < block_r; ++i)
//         {
//             res = max(res, sq[i].val);
//         }

//         Lazy(block_r);
//         for (int i = high; i <= r; i++) { res = max(res, b[i]); }
//     }
//     return res;
// }

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    // DP O(n^2)
    vector<vector<int>> dp(n + 5, vector<int>(n + 5, n + 1));
    for (int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }
    a[0]     = INT_MIN / 2;
    a[n + 1] = INT_MAX / 2;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            if (a[i] < a[dp[i - 1][j]] && a[i] > a[dp[i - 1][j - 1]]) {
                dp[i][j] = i;
                tr[i][j] = dp[i - 1][j - 1];
                if (ans[1] < j) {
                    ans[1] = j;
                    ans[2] = i;
                }
            } else {
                dp[i][j] = tr[i][j] = dp[i - 1][j];
            }
        }
    }
    vector<int> vec;
    cout << ans[1] << "\n";
    int id = ans[2];
    while (1 <= id && id <= n) {
        vec.push_back(a[id]);
        id = tr[id][ans[1]];
        --ans[1];
    }
    reverse(vec.begin(), vec.end());
    for (int i : vec) {
        cout << i << " ";
    }

    // // Greedy + Lowerbound (STL) O(nlogn)
    // for (int i = 1; i <= n; i++)
    // {
    //     auto p = lower_bound(lis.begin(), lis.end(), a[i]);
    //     if (p != lis.end()) { *p = a[i]; }
    //     else { lis.push_back(a[i]); }
    // }
    // cout << lis.size() << "\n";

    // // DP Segment Tree O(nlogn)
    // for (int i = 1; i <= n; i++) { arr[i] = a[i]; }
    // sort(arr + 1, arr + n + 1);
    // int id = 0;
    // for (int i = 1; i <= n; i++)
    // {
    //     if (!idx[arr[i]]) { idx[arr[i]] = ++id; }
    // }
    // for (int i = 1; i <= n; i++)
    // {
    //     dp[i] = Get(1, 1, idx.size(), 1, idx[a[i]] - 1) + 1;
    //     Update(1, 1, idx.size(), idx[a[i]], idx[a[i]], dp[i]);
    // }
    // cout << *max_element(dp + 1, dp + n + 1);

    // // DP Sqrt Decomposition O(n*sqrt(n))
    // for (int i = 1; i <= n; i++) { arr[i] = a[i]; }
    // sort(arr + 1, arr + n + 1);
    // int id = 0;
    // for (int i = 1; i <= n; i++)
    // {
    //     if (!idx[arr[i]]) { idx[arr[i]] = ++id; }
    // }
    // for (int i = 1; i <= n; i++)
    // {
    //     dp[i] = Get(1, idx[a[i]] - 1) + 1;
    //     Update(idx[a[i]], idx[a[i]], dp[i]);
    // }
    // cout << *max_element(dp + 1, dp + n + 1);

    return 0;
}