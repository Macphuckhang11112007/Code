#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 5005;

int n, sum;
int in_arr[kMaxN];
map<int, int> val_id_map;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> sum;
    for (int i = 1; i <= n; ++i) {
        cin >> in_arr[i];
        for (int j = 1; j < i; ++j) {
            auto p = val_id_map.find(sum - in_arr[i] - in_arr[j]);
            if (p != val_id_map.end()) {
                set<int> id_set{i, j, p->second};
                if (id_set.size() != 3) continue;
                for (const int &id : id_set) {
                    cout << id << " ";
                }
                return 0;
            }
        }
        val_id_map[in_arr[i]] = i;
    }
    cout << "IMPOSSIBLE";
    return 0;
}
