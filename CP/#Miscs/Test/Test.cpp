#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#undef _GLIBCXX_DEBUG
#pragma GCC optimize("Ofast")
#pragma GCC target("march=tigerlake")

using namespace std;
using namespace __gnu_pbds;

#define endl "\n"
#define uint unsigned int
#define ll long long
#define ull unsigned long long
#define lll __int128_t
#define ulll __uint128_t
#define mpr make_pair
#define mtpl make_tuple
#define pii pair<int, int>
#define pii_i pair<pii, int>
#define pi_ii pair<int, pii>
#define pii_ii pair<pii, pii>
#define tiii tuple<int, int, int>
#define tiiii tuple<int, int, int, int>
#define umap unordered_map
#define uset unordered_set
#define fi first
#define se second
#define ep emplace
#define epb emplace_back
#define epf emplace_front
#define psh push
#define pshb push_back
#define pshf push_front
#define pp pop
#define ppb pop_back
#define ppf pop_front
#define INF INFINITY
#define IMAX INT_MAX / 2
#define UIMAX UINT_MAX / 2
#define LLMAX LLONG_MAX / 2
#define ULLMAX ULLONG_MAX / 2
#define MOD1 (ll)(1e9 + 7)
#define MOD2 (ll)(998'244'353)
#define all(x) x.begin(), x.end()
#define all(x, l, r) x + l, x + r + 1
#define sz(x) (ll)(x.size())

template <typename T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

template <typename Key, typename Value>
using ordered_map =
    tree<Key, Value, less<Key>, rb_tree_tag, tree_order_statistics_node_update>;

struct timespec time_start, time_end;
double          time_taken;

int tc = 1;

void Set_IO(string file = "", string inp = "", string out = "", bool mtc = 0)
{
    cin.tie(0)->sync_with_stdio(0);
    __builtin_ia32_ldmxcsr(40896);
    if (file != "")
    {
        fstream fin(file + "." + inp, ios::in);
        fstream fout(file + "." + out, ios::out);
        cin.rdbuf(fin);
        cout.rdbuf(fout);
    }
    if (mtc) cin >> tc;
}

void Pre_Processing() {}

bool Input() { return 1; }

void Solve()
{
    for (int i = 1; i <= n; i++)
    {
        auto j = lower_bound(arr.begin(), arr.end(), a[i]);

        if (j == arr.end())
            arr.epb(a[i]);
        else
            *j = a[i];
    }
}

void Output() { cout << arr.size() << endl; }

signed main()
{
    clock_gettime(CLOCK_REALTIME, &time_start);
    Set_IO();
    Pre_Processing();
    if (tc != -1)
        for (int i = 1; i <= tc; i++)
        {
            Input();
            Solve();
            Output();
        }
    else
        while (Input())
        {
            Solve();
            Output();
        }
    clock_gettime(CLOCK_REALTIME, &time_end);
    time_taken = (time_end.tv_sec - time_start.tv_sec) +
                 (time_end.tv_nsec - time_start.tv_nsec) * 1e-9;
    cerr << "\n\nProgram Execution Time: \n"
         << fixed << setprecision(6) << time_taken * 1e3 << " ms" << endl;
    << setprecision(9) << time_taken << " s" << endl
}