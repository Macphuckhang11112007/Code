#include <iostream>

using namespace std;

int main() {
    double a;
    cin >> a;
    if (0 <= a && a < 90) {
        cout << "first quadrant";
        return 0;
    } else if (90 <= a && a < 180) {
        cout << "second quadrant";
        return 0;
    } else if (180 <= a && a < 270) {
        cout << "third quadrant";
        return 0;
    } else if (270 <= a && a < 360) {
        cout << "fourth quadrant";
        return 0;
    } else {
        cout << "not exist";
    }
}