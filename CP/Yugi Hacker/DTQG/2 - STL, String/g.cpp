#include <bits/stdc++.h>

using namespace std;

const int N = 1000;

int  tc, n, cnt;
char a[N + 5];
bool has_num;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--) {
        has_num = 0;
        cnt     = 0;
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i];
            has_num |= isdigit(a[i]);
        }
        if (has_num) {
            for (int i = 0; i < n; i++) {
                if (isalpha(a[i])) {
                    for (int j = 0; j < cnt; j++) {
                        cout << a[i];
                    }
                    cnt = 0;
                } else {
                    cnt = cnt * 10 + (a[i] - '0');
                }
            }
        } else {
            int  cnt = 0;
            char tmp = '\0';
            for (int i = 0; i < n; i++) {
                if (!tmp || tmp == a[i]) {
                    tmp = a[i];
                    cnt++;
                } else {
                    cout << cnt << tmp;
                    tmp = a[i];
                    cnt = 1;
                }
            }
            cout << cnt << tmp;
        }
        cout << "\n";
    }
}
