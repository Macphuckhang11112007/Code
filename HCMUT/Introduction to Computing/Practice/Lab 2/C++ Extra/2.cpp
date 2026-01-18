#include <iostream>

using namespace std;

int main() {
    double x;
    cin >> x;
    if (x <= 5) {
        cout << x * 0.0 / 100.0;
    } else if (x <= 10) {
        cout << x * 5.0 / 100.0;
    } else if (x <= 18) {
        cout << x * 10.0 / 100.0;
    } else if (x <= 32) {
        cout << x * 15.0 / 100.0;
    } else if (x <= 52) {
        cout << x * 20.0 / 100.0;
    } else if (x <= 80) {
        cout << x * 25.0 / 100.0;
    } else {
        cout << x * 30.0 / 100.0;
    }
}