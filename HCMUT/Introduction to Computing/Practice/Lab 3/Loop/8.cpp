#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    // TODO
    for (int i = 5; i <= 85; i += 5) {
        double x = i / 180.0 * 3.14;
        cout << fixed << setprecision(2) << sin(x) << " " << cos(x) << " " << tan(x) << (i != 85 ? "\n" : "");
    }
}