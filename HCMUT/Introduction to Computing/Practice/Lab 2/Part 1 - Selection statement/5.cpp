#include <iostream>

using namespace std;

int main() {
    double s;
    cin >> s;
    if (s < 0 || s > 100) {
        cout << "Grade is not exist";
    } else if (s >= 90) {
        cout << "A";
    } else if (s >= 80) {
        cout << "B";
    } else if (s >= 70) {
        cout << "C";
    } else if (s >= 60) {
        cout << "D";
    } else {
        cout << "F";
    }
}