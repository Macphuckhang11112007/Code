#include <bits/stdc++.h>

using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int tc;
    cin >> tc;
    while (tc--) {
        char type;
        cin >> type;
        string a, b;
        cin >> a >> b;
        if (type == '+') {
            reverse(a.begin(), a.end());
            reverse(b.begin(), b.end());
            while (a.size() < b.size()) {
                a += '0';
            }
            while (b.size() < a.size()) {
                b += '0';
            }
            string res   = "";
            int    carry = 0;
            for (int i = 0; i < int(a.size()); i++) {
                int sum = (a[i] - '0') + (b[i] - '0') + carry;
                res += to_string(sum % 10);
                carry = sum / 10;
            }
            if (carry) {
                res += to_string(carry);
            }
            reverse(res.begin(), res.end());
            while (res.size() > 1 && res[0] == '0') {
                res.erase(res.begin());
            }
            cout << res << '\n';
        } else if (type == '-') {
            if (a.size() < b.size() || (a.size() == b.size() && a < b)) {
                swap(a, b);
            }
            reverse(a.begin(), a.end());
            reverse(b.begin(), b.end());
            while (a.size() < b.size()) {
                a += '0';
            }
            while (b.size() < a.size()) {
                b += '0';
            }
            string res    = "";
            int    borrow = 0;
            for (int i = 0; i < int(a.size()); i++) {
                int sum = (a[i] - '0') - (b[i] - '0') - borrow;
                if (sum < 0) {
                    sum += 10;
                    borrow = 1;
                } else {
                    borrow = 0;
                }
                res += to_string(sum);
            }
            reverse(res.begin(), res.end());
            while (res.size() > 1 && res[0] == '0') {
                res.erase(res.begin());
            }
            cout << res << '\n';
        }
    }
}
