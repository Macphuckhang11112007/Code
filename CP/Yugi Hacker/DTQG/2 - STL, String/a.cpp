#include <bits/stdc++.h>

using namespace std;

int         n;
vector<int> a;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    while (n--) {
        string type;
        cin >> type;
        if (type == "push") {
            int x;
            cin >> x;
            a.push_back(x);
        } else if (type == "pop") {
            if (a.size()) {
                a.pop_back();
            }
        } else if (type == "index") {
            int x;
            cin >> x;
            cout << (x <= int(a.size()) ? a[x - 1] : -1) << "\n";
        } else if (type == "size") {
            cout << a.size() << "\n";
        }
    }
}
