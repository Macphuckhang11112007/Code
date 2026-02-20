// #include <bits/stdc++.h>

// using namespace std;

// int           n, m;
// set<set<int>> st;

// int main() {
//     cin.tie(0)->sync_with_stdio(0);
//     cin >> n >> m;
//     for (int i = 1; i <= n; i++) {
//         set<int> row;
//         for (int j = 1; j <= m; j++) {
//             int x;
//             cin >> x;
//             row.insert(x);
//         }
//         st.insert(row);
//     }
//     cout << st.size() << "\n";
// }

#include <bits/stdc++.h>

using namespace std;

int                 n, m;
vector<vector<int>> mat;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        vector<int> row;
        row.reserve(m + 5);
        for (int j = 1; j <= m; j++) {
            int x;
            cin >> x;
            row.push_back(x);
        }
        sort(row.begin(), row.end());
        row.erase(unique(row.begin(), row.end()), row.end());
        mat.push_back(row);
    }
    sort(mat.begin(), mat.end());
    cout << (unique(mat.begin(), mat.end()) - mat.begin()) << "\n";
}
