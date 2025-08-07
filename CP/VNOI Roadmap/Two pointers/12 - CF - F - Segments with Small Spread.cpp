// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 1e5 + 5;

int                 n;
long long           ans = 0, k;
long long           a[kMaxN];
multiset<long long> nums;

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) { cin >> a[i]; }
    for (int l = 1, r = 1; r <= n; ++r)
    {
        nums.insert(a[r]);
        while (!nums.empty() && *nums.rbegin() - *nums.begin() > k)
        {
            nums.erase(nums.find(a[l++]));
        }
        ans += nums.size();
    }
    cout << ans << '\n';
    return 0;
}
