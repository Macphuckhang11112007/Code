#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 3e4 + 5;

struct Strength
{
    long long val;
    int       id_l, id_r;
    Strength() : val(0), id_l(0), id_r(0) {};
    Strength(long long _val, int _id_l, int _id_r)
        : val(_val), id_l(_id_l), id_r(_id_r) {};
};

int        n, tc;
int        arr[kMaxN], id_left[kMaxN], id_right[kMaxN];
stack<int> id_min;
Strength   ans;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> tc;
    while (tc--)
    {
        cin >> n;
        id_min = stack<int>();
        ans    = Strength();
        arr[0] = arr[n + 1] = 0;
        for (int i = 1; i <= n; ++i) { cin >> arr[i]; }
        id_min.push(0);
        for (int i = 1; i <= n; ++i)
        {
            while (arr[id_min.top()] >= arr[i]) { id_min.pop(); }
            id_left[i] = id_min.top() + 1;
            id_min.push(i);
        }
        id_min = stack<int>();
        id_min.push(n + 1);
        for (int i = n; i; --i)
        {
            while (arr[id_min.top()] >= arr[i]) { id_min.pop(); }
            id_right[i] = id_min.top() - 1;
            id_min.push(i);
        }
        for (int i = 1; i <= n; ++i)
        {
            long long width            = id_right[i] - id_left[i] + 1;
            long long current_strength = arr[i] * width;
            if (current_strength > ans.val)
            {
                ans.val  = current_strength;
                ans.id_l = id_left[i];
                ans.id_r = id_right[i];
            }
        }
        cout << ans.val << " " << ans.id_l << " " << ans.id_r << "\n";
    }
    return 0;
}
