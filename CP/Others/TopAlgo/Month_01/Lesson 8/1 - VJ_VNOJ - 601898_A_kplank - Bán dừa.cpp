#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e6 + 5;

int        n, ans = 0;
int        arr[kMaxN], id_left[kMaxN], id_right[kMaxN];
stack<int> id_min;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i) { cin >> arr[i]; }
    id_min.push(0);
    for (int i = 1; i <= n; ++i)
    {
        while (arr[id_min.top()] >= arr[i]) { id_min.pop(); }
        id_left[i] = id_min.top() + 1;
        id_min.push(i);
    }
    while (!id_min.empty()) { id_min.pop(); }
    id_min.push(n + 1);
    for (int i = n; i; --i)
    {
        while (arr[id_min.top()] >= arr[i]) { id_min.pop(); }
        id_right[i] = id_min.top() - 1;
        id_min.push(i);
    }
    for (int i = 1; i <= n; ++i)
    {
        int id_len = id_right[i] - id_left[i] + 1;
        if (id_len >= arr[i]) { ans = max(ans, arr[i]); }
    }
    cout << ans << '\n';
    return 0;
}
