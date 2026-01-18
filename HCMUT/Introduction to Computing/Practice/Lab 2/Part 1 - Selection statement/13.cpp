#include <iostream>

using namespace std;

int main () {
    double a, b, c;
    cin >> a >> b >> c;
    if (a > b) {
        double tmp = a;
        a = b;
        b = tmp;
    }
    if (b > c) {
        double tmp = b;
        b = c;
        c = tmp;
    }
    if (a + b > c && a + c > b) {
        if (a == b && b == c) {
            cout << "Equilateral triangle";
        } else if (a == b || b == c) {
            if (a == b) {
                cout << "Scalene triangle";
            } else {
                cout << "Isosceles triangle";
            }
        } else {
            cout << "Normal triangle";
        }
    } else {
        cout << "Not a triangle";
    }
}

