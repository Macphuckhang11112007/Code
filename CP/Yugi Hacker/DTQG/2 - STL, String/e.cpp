#include <bits/stdc++.h>

using namespace std;

int n;
set<int> s;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--) {
        int x;
        cin >> x;
        s.insert(x);
    }
    cout << s.size() << "\n";
}
