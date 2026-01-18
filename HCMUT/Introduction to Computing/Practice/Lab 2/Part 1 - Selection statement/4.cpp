#include <iostream>

using namespace std;

int main() {
    int m28[] = {2}, m30[] = {4, 6, 9, 11}, m31[] = {1, 3, 5, 7, 8, 10, 12};
    int m;
    cin >> m;
    for (int i : m28) {
        if (i == m) {
            cout << "28 or 29";
            return 0;
        }
    }
    for (int i : m30) {
        if (i == m) {
            cout << "30";
            return 0;
        }
    }
    for (int i : m31) {
        if (i == m) {
            cout << "31";
            return 0;
        }
    }
}