#include<bits/stdc++.h>
using namespace std;

int main() {
    int t;  
    cin >> t;

    while (t--) {
        int n, C, D;
        bool ok = false;
        cin >> n >> C >> D;

        vector<int> a(n), b(n);
        int max_a = -1e9, min_a = 1e9, max_b = -1e9, min_b = 1e9;

        for (int i = 0; i < n; ++i) {
            cin >> a[i];
            max_a = max(max_a, a[i]);
            min_a = min(min_a, a[i]);
        }

        for (int i = 0; i < n; ++i) {
            cin >> b[i];
            max_b = max(max_b, b[i]);
            min_b = min(min_b, b[i]);
        }

        if (max_a - min_a <= C && max_b - min_b <= D) {
            cout << 0 << endl;
        } else {
            int operations = 0;

            while (max_a - min_a > C || max_b - min_b > D) {
                if (max_a - min_a > C) {
                    int index = min_element(a.begin(), a.end()) - a.begin();
                    a[index]++;
                    b[index]++;
                    operations++;
                }

                if (max_b - min_b > D) {
                    int index = min_element(b.begin(), b.end()) - b.begin();
                    a[index]++;
                    b[index]++;
                    operations++;
                }

                max_a = *max_element(a.begin(), a.end());
                min_a = *min_element(a.begin(), a.end());
                max_b = *max_element(b.begin(), b.end());
                min_b = *min_element(b.begin(), b.end());
                if (operations > 10) {
                    ok = true;
                    cout << -1 << '\n';
                    break;
                }
            }

            if(!ok) cout << operations << endl;
        }
    }
    return 0;
}
