// MPK

#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 5000 + 5;

int n, x;
int a[kMaxN], id[kMaxN];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> x;
    for (int i = 1; i <= n; ++i)
    {
        cin >> a[i];
        id[i] = i;
    }
    sort(id + 1, id + n + 1,
         [](const int &i, const int &j) { return a[i] < a[j]; });
    sort(a + 1, a + n + 1);
    for (int high = 1; high <= n; ++high)
    {
        int low = 1, mid = high - 1;
        while (low < mid)
        {
            if (1LL * a[low] + a[mid] + a[high] == 1LL * x)
            {
                vector<int> ids{id[low], id[mid], id[high]};
                sort(ids.begin(), ids.end());
                cout << ids[0] << ' ' << ids[1] << ' ' << ids[2] << '\n';
                return 0;
            }
            else if (1LL * a[low] + a[mid] + a[high] < 1LL * x) { ++low; }
            else if (1LL * a[low] + a[mid] + a[high] > 1LL * x) { --mid; }
        }
    }
    cout << "IMPOSSIBLE\n";
    return 0;
}
