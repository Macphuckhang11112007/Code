#include <bits/stdc++.h>

using namespace std;

const int kMaxN = 100 + 5;

int n;
// string arr[kMaxN];
long long arr[kMaxN];

// int main()
// {
//     cin.tie(0)->sync_with_stdio(0);
//     cin >> n;
//     for (int i = 1; i <= n; ++i) { cin >> arr[i]; }
//     sort(arr + 1, arr + n + 1,
//          [](const string& i, const string& j)
//          {
//              if (i.size() == j.size()) { return i > j; }
//              else { return i + j > j + i; }
//          });
//     for (int i = 1; i <= n; ++i) { cout << arr[i]; }
//     return 0;
// }

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n;
    for (int i = 1; i <= n; ++i) { cin >> arr[i]; }
    sort(arr + 1, arr + n + 1,
         [](const long long& i, const long long& j)
         {
             int num_i = floor(log10(i)) + 1, num_j = floor(log10(j)) + 1;
             if (num_i == num_j) { return i > j; }
             else { return i * pow(10, num_j) + j > j * pow(10, num_i) + i; }
         });
    for (int i = 1; i <= n; ++i) { cout << arr[i]; }
    return 0;
}
