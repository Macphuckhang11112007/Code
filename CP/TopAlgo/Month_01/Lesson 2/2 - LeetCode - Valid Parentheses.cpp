/*
─────────────────────────────────────────────────────────────────
1. ℹ️ FILE AND AUTHOR INFORMATION
─────────────────────────────────────────────────────────────────
    - ✍️ Author                : Mạc Phúc Khang - MPK (MacPhucKhang)
    - 💾 Repository Locations  :
        🌐 Remote Repository   : https://github.com/Macphuckhang11112007/Code/tree/main/CP/TopAlgo/Month 1/Lesson 2/2 - LeetCode - Valid Parentheses.cpp
        🖥️ Local Repository    : C:/Users/macph/Downloads/MPK/Code/CP/TopAlgo/Month 1/Lesson 2/2 - LeetCode - Valid Parentheses.cpp
    - 📄 File Name             : 2 - LeetCode - Valid Parentheses.cpp
    - 🏷️ File Version          : v1.0
    - 🗓️ Creation Date         : 19:35 05-07-2025
    - ✏️ Last Modified Date    : 20:00 05-07-2025
    - 🧠 Thinking Time         : 00:05
    - 💻 Coding Time           : 00:10
    - 📏 Code Length           : 35 - 264 lines / 12 KB

─────────────────────────────────────────────────────────────────
2. 🎯 PROBLEM STATEMENT
─────────────────────────────────────────────────────────────────
    - 🌟 Problem Title         : 20. Valid Parentheses
    - 🆔 Problem ID            : valid-parentheses
    - 🔗 Problem URL           : https://leetcode.com/problems/valid-parentheses/description/
    - 🏛️ Judge Platform        : LeetCode
    - 📜 Problem Source        : LeetCode
    - 🏷️ Problem Classification: String; Stack
    - 📊 Difficulty            : ⭐ (Easy)
    - ⏱️ Time Limit            : 1 s
    - 📦 Memory Limit          : 256 MB
    - 📈 Constraints           :
                                    1 <= s.length <= 10^4
                                    s consists of parentheses only '()[]{}'
    - 📝 Problem Description   :
                                    Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
                                    An input string is valid if:
                                        1. Open brackets must be closed by the same type of brackets.
                                        2. Open brackets must be closed in the correct order.
                                        3. Every close bracket has a corresponding open bracket of the same type.
    - ↔️ Input Format          : stdin
    - ↔️ Output Format         : stdout

─────────────────────────────────────────────────────────────────
3. 💡 SOLUTION DETAILS AND ANALYSIS
─────────────────────────────────────────────────────────────────
    - 🧭 Solution Overview and Strategy:
        The algorithm uses a stack to keep track of opening brackets. For each character in the string:
            - If it is an opening bracket ('(', '[', '{'), push it onto the stack.
            - If it is a closing bracket (')', ']', '}'):
                - If the stack is empty, the string is invalid.
                - Otherwise, check if the top of the stack is the matching opening bracket. If yes, pop it; if not, the string is invalid.
        At the end, if the stack is empty, the string is valid; otherwise, it is invalid.

    - 🛠️ Technical Implementation Details:
        - ⚡ Algorithms Used        : Stack-based matching of parentheses
        - 🧩 Data Structures Used  : Stack (std::stack<char>)
        - 📊 Declared Data Structures: stack<char> st;
        - ⚙️ Main Functions and Purpose: main() - reads input, checks validity, outputs result
        - 🧑‍💻 Custom Classes/Structs   : None

    - 🔍 Analysis and Notes:
        - 📉 Complexity           :
                - ⏰ Time Complexity   : O(n), where n is the length of the string
                - 📦 Space Complexity  : O(n), for the stack in the worst case
        - 🧪 Edge Cases and Limitations: Empty string, only opening or only closing brackets, mismatched pairs
        - 🌱 Notes and Learnings  : Classic stack usage for bracket matching

─────────────────────────────────────────────────────────────────
4. 🚀 SUBMISSION PERFORMANCE
─────────────────────────────────────────────────────────────────
    - 📤 Submission Source      : https://leetcode.com/problems/valid-parentheses/submissions/1687229524
    - ⏱️ Submission Time        : 20:05
    - ✅ Status / Verdict       : AC - Accepted
    - 🏆 Peak Resources Used    : 0 ms (Beats 100.00%), 8.84 MB (Beats 33.53%)
    - 💯 Final Score            : 100 / 100

─────────────────────────────────────────────────────────────────
5. 🔗 REFERENCES AND RELATED PROBLEMS
─────────────────────────────────────────────────────────────────
    - 📖 Related Problems     :
        - RELATED_PROBLEM_TITLE_OR_URL
    - 🌍 External References  :
        - FULL_URL_TO_EXTERNAL_REFERENCE

─────────────────────────────────────────────────────────────────
6. 📜 REVISION HISTORY
─────────────────────────────────────────────────────────────────
    - ➡️ v1.0 (19:35 05-07-2025) : Initial version. Implemented stack-based solution for LeetCode 20. Valid Parentheses. Reads input string, checks for valid parentheses using stack, outputs "true" or "false" accordingly. Includes detailed comments and metadata.

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
int tc = 1;
string str;
stack <char> st;
signed main ()
{
    file ("", "", "");
    while (tc--)
    {
        // Solve here!
        cin >> str;
        for (char& ch : str)
        {
            if (ch == '(' || ch == '[' || ch == '{') st.psh (ch);
            else
            {
                if (st.empty ())
                {
                    cout << "false";
                    return 0;
                }
                else
                {
                    if (abs (st.top () - ch) <= 2) st.pop ();
                    else
                    {
                        cout << "false";
                        return 0;
                    }
                }
            }
        }
        cout << (st.empty () ? "true" : "false");
    }
    return 0;
}