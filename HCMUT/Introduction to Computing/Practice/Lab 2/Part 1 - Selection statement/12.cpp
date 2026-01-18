#include <iostream>

using namespace std;

int main() {
    double x;
    cin >> x;
    if (x > 0) {
        cout << x << " is a positive number.";
    } else if (x < 0) {
        cout << x << " is a negative number.";
    } else if (x == 0) {
        cout << x << " is zero.";
    }
}
