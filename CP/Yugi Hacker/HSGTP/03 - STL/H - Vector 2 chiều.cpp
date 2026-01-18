#include <bits/stdc++.h>

#define ll long long

using namespace std;

const int kMaxNM = 1e6 + 5;

int                n, m, q;
int                org_row[kMaxNM + 1];
vector<vector<ll>> a;
ll                 sum_row[kMaxNM], sum_col[kMaxNM];

int main()
{
    cin.tie(0)->sync_with_stdio(0);
    cin >> n >> m >> q;
    a.resize(n + 5, vector<ll>(m + 5));
    for (int i = 1; i <= n; ++i)
    {
        ll tmp_sum_row = 0;
        for (int j = 1; j <= m; ++j)
        {
            ll val;
            cin >> val;
            a[i][j]    = val;
            org_row[i] = i;
            tmp_sum_row += val;
            sum_col[j] += val;
        }
        sum_row[i] = tmp_sum_row;
    }
    while (q--)
    {
        char ch;
        cin >> ch;
        if (ch == '1')
        {
            int row1, row2;
            cin >> row1 >> row2;
            swap(org_row[row1], org_row[row2]);
        }
        else if (ch == '2')
        {
            int row, col;
            ll  val;
            cin >> row >> col >> val;
            (sum_row[org_row[row]] -= a[org_row[row]][col]) += val;
            (sum_col[col] -= a[org_row[row]][col]) += val;
            a[org_row[row]][col] = val;
        }
        else if (ch == 'r')
        {
            int row;
            cin >> row;
            cout << sum_row[org_row[row]] << "\n";
        }
        else if (ch == 'c')
        {
            int col;
            cin >> col;
            cout << sum_col[col] << "\n";
        }
    }
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= m; ++j) { cout << a[org_row[i]][j] << " "; }
        cout << "\n";
    }
    return 0;
}
