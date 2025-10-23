#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

void prob0() {
    double a, b, c;
    cout << "Enter the length of the first leg: ";
    cin >> a;
    cout << "Enter the length of the second leg: ";
    cin >> b;
    if (a > 0 && b > 0) {
        if (a <= 1e18 && b <= 18) {
            c = sqrt(a * a + b * b);
            cout << fixed << setprecision(2) << "The length of the hypotenuse is: " << c << endl;
        } else {
            cout << "The length of this triangle is too large which may cause error" << endl;
        }
    } else if (a <= 0 || b <= 0) {
        cout << "Not a triangle" << endl;
    }
    return;
}

void prob1() {
    double e = 11220184543.02 * 1e9, m = (log10(e) - 4.8) / 1.5;
    cout << fixed << setprecision(2) << "The magnitude M of that earthquake: " << m << endl;
    return;
}

void prob2() {
    double m, e;
    cout << "Enter the magnitude of the earthquake: ";
    cin >> m;
    e = pow(10, 1.5 * m + 4.8);
    cout << fixed << setprecision(2) << e << endl;
    return;
}

void prob3() {
    double h, a, theta;
    a     = 5;
    theta = 30 * M_PI / 180;
    h     = a * sin(theta);
    cout << fixed << setprecision(2) << h << endl;
    return;
}

void prob4() {
    double v, g, h, a;
    g = 9.81;
    cin >> h >> a;
    a = a * M_PI / 180;
    v = sqrt(g * h * tan(a));
    cout << fixed << setprecision(2) << v << endl;
    return;
}

int main() {
    cout << "Prob 3.0:" << endl;
    prob0();
    cout << endl;
    cout << "Prob 3.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 3.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 3.3:" << endl;
    prob3();
    cout << endl;
    cout << "Prob 3.4:" << endl;
    prob4();
    cout << endl;
    return 0;
}