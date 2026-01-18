#include <bits/stdc++.h>

using namespace std;

bool chk(int n) {
    int x = sqrt(n);
    return (x * x == n);
}

int main() {
    set<int> s;
    cin.tie(0)->sync_with_stdio(0);
    // freopen("Test.inp", "r", stdin);
    freopen("Test.out", "w", stdout);
    for (int x = 1; x <= 100; x++) {
        for (int y = 1; y <= 100; y++) {
            for (int a = -100; a <= 100; a++) {
                if (chk(x * y + x + a)) {
                    s.insert(x * y + x);
                    cout << x << " " << y << " " << a << " " << " " << x * y + x << " " << sqrt(x * y + x + a) << "\n";
                }
            }
        }
    }
    cout << "\n\n\n";
    int tmp = 1;
    int ok  = 1;
    for (int i : s) {
        cout << i << "\n";
        ok &= (tmp + 1 == i);
        tmp = i;
    }
    cout << "..." << ok;
    return 0;
}