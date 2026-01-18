// MPK

#include <bits/stdc++.h>

#define fi first
#define se second

using namespace std;

const int kMaxN = 1e5 + 5;

int                     n, k;
long long               ans = 0;
int                     a[kMaxN];
unordered_map<int, int> nums;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    for (int l = 1, r = 1; r <= n; ++r)
    {
        ++nums[a[r]];
        while (nums.size() > k)
        {
            auto p = nums.find(a[l++]);
            --p->se;
            if (!p->se) { nums.erase(p); }
        }
        if (nums.size() <= k) { ans += r - l + 1; }
    }
    cout << ans << '\n';
    return 0;
}
