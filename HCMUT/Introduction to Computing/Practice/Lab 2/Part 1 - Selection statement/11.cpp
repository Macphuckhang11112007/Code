#include <iostream>

using namespace std;

int main() {
    double x;
    cin >> x;
    if (x == (long long)(x)) {
        if ((long long)(x) % 2 == 0) {
            cout << x << " is even.";
        } else {
            cout << x << " is odd.";
        }
    } else {
        cout << x << " is not an integer.";
    }
}
