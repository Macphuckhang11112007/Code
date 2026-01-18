#include <bits/stdc++.h>

using namespace std;

int n, s;
map<int, int> mapp;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> s;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        if (mapp.find(s - x) != mapp.end()) {
            cout << mapp[s - x] << " " << i;
            return 0;
        }
        mapp[x] = i;
    }
    cout << "IMPOSSIBLE";
    return 0;
}
