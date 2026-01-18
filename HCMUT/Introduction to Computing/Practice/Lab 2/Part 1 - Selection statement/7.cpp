#include <iostream>

using namespace std;

int main() {
    int opt;
    cin >> opt;
    if (opt == 1) {
        double F, C;
        cin >> F;
        C = (5.0 / 9.0) * (F - 32.0);
        cout << C;
    } else if (opt == 2) {
        double C, F;
        cin >> C;
        F = (9.0 / 5.0) * C + 32.0;
        cout << F;
    } else if (opt == 3) {
        return 0;
    } else {
        cout << "Invalid option";
    }
}