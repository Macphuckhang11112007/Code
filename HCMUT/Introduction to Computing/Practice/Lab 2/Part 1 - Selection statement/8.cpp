#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    double a, b, c, d;
    cin >> a >> b >> c;
    d = b * b - 4 * a * c;
    cout << fixed << setprecision(4);
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                cout << "Countless solutions\n";
            } else {
                cout << "No solution\n";
            }
        } else {
            cout << -c / b;
        }
    } else {
        if (d < 0) {
            double r, i;
            r = -b / (2 * a);
            i = sqrt(abs(d)) / (2 * a);
            cout << r << " + " << i << "*i\n";
            cout << r << " - " << i << "*i\n";
        } else if (d == 0) {
            cout << -b / (2 * a) << "\n";
        } else {
            double x1, x2;
            x1 = (-b + sqrt(d)) / (2 * a);
            x2 = (-b - sqrt(d)) / (2 * a);
            cout << x1 << "\n" << x2 << "\n";
        }
    }
}
