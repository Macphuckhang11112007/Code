#include <iostream>

using namespace std;

int main () {
    double a, b, c;
    cin >> a >> b >> c;
    if (a < b) {
        if (b < c) {
            cout << c;
        } else if (b > c) {
            cout << b;
        }
    } else if (a > b) {
        if (a < c) {
            cout << c;
        } else if (a > c) {
            cout << a;
        }
    }
}
