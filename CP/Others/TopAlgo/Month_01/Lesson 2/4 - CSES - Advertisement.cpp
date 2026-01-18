/*
─────────────────────────────────────────────────────────────────
1. ℹ️ FILE AND AUTHOR INFORMATION
─────────────────────────────────────────────────────────────────
- ✍️ Author                : Mạc Phúc Khang - MPK (MacPhucKhang)
- 💾 Repository Locations:
    🌐 Remote Repository   : https://github.com/Macphuckhang11112007/Code/tree/main/CP\TopAlgo\Month 1\Lesson 2\4 - CSES - Advertisement.cpp
    🖥️ Local Repository    : C:/Users/macph/Downloads/MPK/Code/CP/TopAlgo/Month 1/Lesson 2/4 - CSES - Advertisement.cpp
- 📄 File Name             : 4 - CSES - Advertisement.cpp
- 🗓️ Creation Date         : 13:17 06-07-2025
- 💻 Coding Time           : 01:53
- 📏 Code Length           : 244 lines / 10 KB

─────────────────────────────────────────────────────────────────
2. 🎯 PROBLEM STATEMENT
─────────────────────────────────────────────────────────────────
- 🌟 Problem Title         : Advertisement
- 🆔 Problem ID            : 1142
- 🔗 Problem URL           : https://cses.fi/problemset/task/1142
- 🏛️ Judge Platform        : CSES
- 📜 Problem Source        : CSES Problem Set
- 🏷️ Problem Classification: Stack
- 📊 Difficulty            : 4 ⭐ (Medium+)
- ⏱️ Time Limit            : 1 s
- 📦 Memory Limit          : 512 MB
- 📈 Constraints           : KEY_CONSTRAINTS
- 📝 Problem Description   : DETAILED_PROBLEM_DESCRIPTION_SUMMARIZED
- ↔️ Input Format          : stdin
- ↔️ Output Format         : stdout

─────────────────────────────────────────────────────────────────
3. 💡 SOLUTION DETAILS AND ANALYSIS
─────────────────────────────────────────────────────────────────
### 3.1. 🧭 Solution Overview and Strategy:
SOLUTION_OVERVIEW_AND_STRATEGY

### 3.2. 🛠️ Technical Implementation Details:
- ⚡ Algorithms Used           : LIST_ALGORITHMS_USED
- 🧩 Data Structures Used      : LIST_DATA_STRUCTURES_USED
- 📊 Declared Data Structures  : DECLARE_MAIN_DATA_STRUCTURES_IN_CODE
- ⚙️ Main Functions and Purpose: MAIN_FUNCTIONS_AND_THEIR_PURPOSE
- 🧑‍💻 Custom Classes/Structs    : CUSTOM_CLASSES_OR_STRUCTS

### 3.3. 🔍 Analysis and Notes:
- 📉 Complexity:
    - ⏰ Time Complexity       : O(TIME_COMPLEXITY_EXPRESSION)
    - 📦 Space Complexity      : O(SPACE_COMPLEXITY_EXPRESSION)
- 🧪 Edge Cases and Limitations: EDGE_CASES_AND_LIMITATIONS
- 🌱 Notes and Learnings       : YOUR_NOTES_AND_KEY_LEARNINGS_HERE

─────────────────────────────────────────────────────────────────
4. 🚀 SUBMISSION PERFORMANCE
─────────────────────────────────────────────────────────────────
- 📤 Submission Source  : https://cses.fi/problemset/result/13595276
- ⏱️ Submission Time    : 14:39:12 06/07/2025
- ✅ Status / Verdict   : AC - Accepted
- 🏆 Peak Resources Used: 0.14 s, 3.05 MB
- 💯 Final Score        : 10 / 10

─────────────────────────────────────────────────────────────────
5. 🔗 REFERENCES AND RELATED PROBLEMS
─────────────────────────────────────────────────────────────────
### 📖 Related Problems   :
- RELATED_PROBLEM_TITLE_OR_URL
### 🌍 External References:
- FULL_URL_TO_EXTERNAL_REFERENCE

─────────────────────────────────────────────────────────────────
// Any additional notes or sections can be added here.
─────────────────────────────────────────────────────────────────
*/

// #undef _GLIBCXX_DEBUG
// #pragma GCC optimize("Ofast,inline,unroll-loops")
// #pragma GCC target("tune=skylake")

#include <bits/stdc++.h>

using namespace std;

///////////////////////////////////
// 🔢 Constants
///////////////////////////////////
const int INF = 1e9 + 7;
const int MOD = 1e9 + 7;
const int MOD2 = 998244353;
const long long LINF = 1e18 + 9;
const double EPS = 1e-9;
const double PI = acos (-1.0);

///////////////////////////////////
// 🔤 Typedefs & Short Names
///////////////////////////////////
#define int long long
#define ll long long
#define ull unsigned long long
#define dbl double
#define ldbl long double
#define pii pair <int, int>
#define pll pair <ll, ll>
#define tpl tuple
#define tiii tpl <int, int, int>
#define tlll tpl <ll, ll, ll>

///////////////////////////////////
// 📚 STL Containers & Utilities
///////////////////////////////////
#define vi vector <int>
#define vl vector <ll>
#define vii vector <pii>
#define vll vector <pll>
#define viii vector <tiii>
#define vlll vector <tlll>
#define vvi vector <vi>
#define vvl vector <vl>
#define vvii vector <vii>
#define vvll vector <vll>
#define vviii vector <viii>
#define vvlll vector <vlll>

#define mp map
#define ump unordered_map
#define uset unordered_set
#define pq priority_queue

///////////////////////////////////
// 🚀 Shorthand Operations
///////////////////////////////////
#define fi first
#define se second
#define psh push
#define pb push_back
#define pf push_front
#define eb emplace_back
#define ef emplace_front
#define ins insert
#define ep emplace
#define mpr make_pair
#define mtpl make_tpl
#define get(x, i) get <i> (x)

#define sz(x) (ll)(x.size())
#define all(x) (x.begin(), x.end())
#define rall(x) (x.rbegin(), x.rend())
#define alln(x, n) (x + 1, x + n + 1)

///////////////////////////////////
// 🔁 Loop Macros
///////////////////////////////////
#define rep(i, n) for (int i = 1; i <= n; i++)
#define per(i, n) for (int i = n; i >= 1; i--)
#define reps(i, a, b) for (int i = a; i <= b; i++)
#define pers(i, a, b) for (int i = a; i >= b; i--)

///////////////////////////////////
// 🧠 Bit Manipulation
///////////////////////////////////
#define BIT(x,i) (((x) >> (i)) & 1)
#define SET(x,i) ((x) | (1LL << (i)))
#define FLIP(x,i) ((x) ^ (1LL << (i)))
#define CLR(x,i) ((x) & ~(1LL << (i)))
#define LOWBIT(x) ((x) & -(x))
#define POPCOUNT(x) __builtin_popcountll(x)
#define LSB(x) __builtin_ctzll(x)
#define MSB(x) (63 - __builtin_clzll(x))

///////////////////////////////////
// 🎯 Floating Point Comparisons
///////////////////////////////////
#define eq(a,b) abs(a - b) < EPS
#define leq(a,b) a < b + EPS
#define geq(a,b) a > b - EPS
#define prec(n) fixed << setprecision(n)

///////////////////////////////////
// 🧾 Output & Debug Tools
///////////////////////////////////
#define yes cout << "YES\n"
#define no cout << "NO\n"
#define endl '\n'
#define dbg(x) cerr << #x << ": " << x << endl
#define printv(v) for (auto& x : v) cout << x << ' '; cout << endl
#define print2d(v) for (auto& row : v) { for (auto x : row) cout << x << ' '; cout << endl; }
#define line cout << "==================================================\n"

///////////////////////////////////
// 🔡 Char Conversions
///////////////////////////////////
#define cnum(x) ((x) - '0')
#define calpha(x) ((x) - 'a')
#define cAlpha(x) ((x) - 'A')

///////////////////////////////////
// 📂 File Redirection (Optional)
///////////////////////////////////
void file (string fl, string in, string out)
{
    cin.tie (0)->sync_with_stdio (0); // Optional: flush-to-zero denormals
    // __builtin_ia32_ldmxcsr(40896);  
    if (fl != "")
    {
        freopen ((fl + in).c_str (), "r", stdin);
        freopen ((fl + out).c_str (), "w", stdout);
    }
}

///////////////////////////////////
// 🚨 Entry Point
///////////////////////////////////
const int MAXN = 2e5 + 5;

int tc = 1;
int n, a [MAXN], lpos [MAXN], rpos [MAXN];
int ans;
stack <int> st;
signed main ()
{
    file ("", "", "");
    while (tc--)
    {
        // Solve here!
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> a [i];
        a [0] = a [n + 1] = -INF;
        st.psh (0);
        for (int i = 1; i <= n; i++)
        {
            while (a [st.top ()] >= a [i]) st.pop ();
            lpos [i] = st.top ();
            st.push (i);
        }
        while (!st.empty ()) st.pop ();
        st.psh (n + 1);
        for (int i = n; i >= 1; i--)
        {
            while (a [st.top ()] >= a [i]) st.pop ();
            rpos [i] = st.top ();
            st.push (i);
        }
        for (int i = 1; i <= n; i++) ans = max (ans, a [i] * (rpos [i] - lpos [i] - 1));
        cout << ans;
    }
    return 0;
}