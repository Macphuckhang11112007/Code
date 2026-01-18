#include <bits/stdc++.h>

using namespace std;

int       tc;
long long a, b, x, y;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        long long p, q;
        cin >> a >> b >> x >> y;
        if (x > y) {
            swap(x, y);
        }
        p = x - a;
        q = y - a;
        if ((p < 0 && q > 0) || (p > 0 && p == q)) {
            cout << "1 Long\n";
        } else {
            p = x - b;
            q = y - b;
            if ((p < 0 && q > 0) || (p <= x && q >= x)) {
                cout << "1 Van\n";
            } else {
                p = x - a;
                q = y - a;
                if ((p < 0 && q > 0) || (p < y - x + 1 && q >= y - x + 1)) {
                    cout << "1 Long\n";
                } else {
                    p = x - b;
                    q = y - b;
                    if ((p < 0 && q > 0) || (p <= 2 * x - y - 1 && q > 2 * x - y - 1)) {
                        cout << "1 Van\n";
                    } else {
                        p = x - a;
                        q = y - a;
                        if ((p < 0 && q > 0) || (p < 2 * y - 2 * x + 1 && q >= 2 * y - 2 * x + 1)) {
                            cout << "1 Long\n";
                        } else {
                            cout << "Impossible\n";
                        }
                    }
                }
            }
        }
    }
    return 0;
}