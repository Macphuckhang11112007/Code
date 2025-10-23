#include <iostream>

using namespace std;

int main() {
    cout << 2 + 4 * 3 - 5 << endl;                                 // 2 + 12 - 5 = 9
    cout << 10 - 8 / 2 + 6 * 2 - 3 << endl;                        // 10 - 4 + 12 - 3 = 15
    cout << 0 / 5 * 3 + 4 * 2 - 12 / 3 * 2 + 7 - 6 / 2 << endl;    // 0 + 8 - 8 + 7 - 3 = 4
    cout << 25 - 7 * 4 % 3 + 12 / 2 << endl;                       // 25 - 28 % 3 + 6 = 25 - 1 + 6 = 30
    cout << 9 * 4 % 7 * 6 / 3 + 8 % 5 * 2 - 11 / 2 << endl;        // 2 + 6 - 5 = 3
    return 0;
}