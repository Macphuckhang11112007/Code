#include <cctype>
#include <iostream>

using namespace std;

int main() {
    char c;
    cin >> c;
    c = toupper(c);
    switch (c) {
        case 'M':
            cout << "Individual is married";
            break;
        case 'D':
            cout << "Individual is divorced";
            break;
        case 'W':
            cout << "Individual is widowed";
            break;
        default:
            cout << "An invalid code was entered";
    }
}