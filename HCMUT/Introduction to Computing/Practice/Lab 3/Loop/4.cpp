#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    double x;
    cin >> x;
    auto Pow = [](double a, int b) -> double {
        double res = 1;
        if (b < 0) {
            a = 1.0 / a;
        }
        while (b) {
            if (b & 1) {
                res *= a;
            }
            a *= a;
            b /= 2;
        }
        return res;
    };
    cout << fixed << setprecision(2) << Pow(x, n) << " " << Pow(x, -n);
}