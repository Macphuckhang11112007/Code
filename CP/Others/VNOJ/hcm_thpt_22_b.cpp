#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const int N = 2e5, MOD = 108;

int         n, q;
ll          tree1[N + 5], tree2[N + 5];
vector<int> fib = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 590, 570, 153, 723, 876, 592, 461, 46, 507, 553, 53, 606, 659, 258, 917, 168, 78, 246, 324, 570, 894, 457, 344, 801, 138, 939, 70, 2, 72, 74, 146, 220, 366, 586, 952, 531, 476, 0, 476, 476, 952, 421, 366, 787, 146, 933, 72, 1005, 70, 68, 138, 206, 344, 550, 894, 437, 324, 761, 78, 839, 917, 749, 659, 401, 53, 454, 507, 961, 461, 415, 876, 284, 153, 437, 590, 20, 610, 630, 233, 863, 89, 952, 34, 986, 13, 999, 5, 1004, 2, 1006, 1, 0};

void update(auto &tree, int i, int x) {
    for (; i <= n; i += i & -i) { tree[i] += x; }
}

ll get(auto &tree, int i) {
    ll res = 0;
    for (; i; i -= i & -i) { res += tree[i]; }
    return res;
}

ll sum(int i) {
    return (i + 1) * get(tree1, i) - get(tree2, i);
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            ll  k;
            cin >> l >> r >> k;
            k     = (k - 1 + MOD) % MOD;
            int x = fib[k];
            update(tree1, l, x);
            update(tree1, r + 1, -x);
            update(tree2, l, l * x);
            update(tree2, r + 1, -(r + 1) * x);
        } else {
            int l, r;
            cin >> l >> r;
            cout << sum(r) - sum(l - 1) << "\n";
        }
    }
}
