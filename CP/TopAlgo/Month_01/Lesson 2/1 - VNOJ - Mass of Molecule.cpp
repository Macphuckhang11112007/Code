#include <bits/stdc++.h>

using namespace std;

string str;
int sz;
int i = 0, x = 0;

int mass (char& ch)
{
    if (ch == 'C') return 12;
    else if (ch == 'H') return 1;
    else if (ch == 'O') return 16;
    return 0;
}

int calc ()
{
    char ch = str [i];
    if (ch == ' ') return 0;
    i++;
    char nch = str [i];
    if ('2' <= ch && ch <= '9') return calc ();
    if (ch != '(' && ch != ')')
    {
        if ('2' <= nch && nch <= '9') return mass (ch) * (nch - '0') + calc ();
        else return mass (ch) + calc ();
    }
    else
    {
        if (ch == '(')
        {
            int tmp = calc (); tmp *= x;
            return tmp + calc ();
        }
        else
        {
            if ('2' <= nch && nch <= '9')
                x = nch - '0';
            else
                x = 1;
            return 0;
        }
    }
    return 0;
}

// int main ()
// {
//     cin.tie (0)->sync_with_stdio (0);
//     cin >> str;
//     str += ' ';
//     sz = str.size ();
//     cout << calc ();
//     return 0;
// }

int solve ()
{
    stack <int> st;
    for (char& ch : str)
    {
        bool isnum = ('2' <= ch && ch <= '9');
        if (ch == '(') st.push (0);
        else if (!isnum && ch != ')') st.push (mass (ch));
        else if (ch == ')')
        {
            int sum = 0;
            while (1)
            {
                int x = st.top ();
                st.pop ();
                if (x == 0) break;
                sum += x;
            }
            st.push (sum);
        }
        else
        {
            int x = st.top ();
            st.pop ();
            st.push (x * (ch - '0'));
        }
    }
    int sum = 0;
    while (!st.empty ())
    {
        int x = st.top ();
        st.pop ();
        sum += x;
    }
    return sum;
}

int main ()
{
    cin.tie (0)->sync_with_stdio (0);
    cin >> str;
    str += ' ';
    sz = str.size ();
    cout << solve ();
    return 0;
}