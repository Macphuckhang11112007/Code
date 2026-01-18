#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
using namespace std;

int main() {
    // TODO
    int n;
    cin >> n;
    double avg = 0.0;
    for (int i = 0; i < n; i++) {
        double x;
        cin >> x;
        avg += x;
    }
    cout << fixed << setprecision(2) << avg / double(n);
}