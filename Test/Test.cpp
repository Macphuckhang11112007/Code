#include <bits/stdc++.h>

using namespace std;

int fib(int n) {
    static int f[100] = {0};
    if (n <= 2) {
        return f[n] = n - 1;
    }
    if (f[n]) {
        return f[n];
    }
    return f[n] = fib(n - 1) + fib(n - 2);
}

int gcd(int a, int b) {
    return (!b ? a : gcd(b, a % b));
}

int sum_digit_rec(int n) {
    if (n == 0) {
        return 0;
    }
    return sum_digit_rec(n / 10) + n % 10;
}

int sum_digit_iter(int n) {
    int res = 0;
    while (n) {
        res += n % 10;
        n /= 10;
    }
    return res;
}

bool palindrome_iter(string s) {
    for (int i = 0; 2 * i < s.size(); i++) {
        if (s[i] != s[s.size() - i - 1]) {
            return 0;
        }
    }
    return 1;
}

bool palindrome_rec(int i, string s) {
    if (2 * i >= s.size()) {
        return 1;
    }
    if (s[i] == s[s.size() - i - 1]) {
        return palindrome_rec(i + 1, s);
    }
    return 0;
}

int main() {
    cout << fib(30) << "\n";
    cout << gcd(30, 42) << "\n";
    cout << sum_digit_rec(129) << "\n";
    cout << sum_digit_iter(129) << "\n";
    cout << palindrome_iter("aabbaa") << "\n";
    cout << palindrome_rec(0, "abbaa") << "\n";
    return 0;
}