#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

void prob0() {
    double a, b, c;
    cout << "Enter the length of the first leg: ";
    cin >> a;
    cout << "Enter the length of the second leg: ";
    cin >> b;
    if (a > 0 && b > 0) {
        if (a <= 1e18 && b <= 18) {
            c = sqrt(a * a + b * b);
            cout << fixed << setprecision(2) << "The length of the hypotenuse is: " << c << endl;
        } else {
            cout << "The length of this triangle is too large which may cause error" << endl;
        }
    } else if (a <= 0 || b <= 0) {
        cout << "Not a triangle" << endl;
    }
    return;
}

void prob1() {
    string s;
    cout << "Enter Full Product Code: ";
    cin >> s;
    string t = s.substr(4);
    cout << "Product Detail String: " << t << endl;
    return;
}

void prob2() {
    string name;
    int    p;
    cout << "Enter your full name: ";
    getline(cin >> ws, name);
    cout << "Enter length P: ";
    cin >> p;
    string t = name.substr(0, p);
    cout << "First P characters: " << t << endl;
    return;
}

void prob3() {
    string name;
    int    l, n;
    cout << "Enter your full name: ";
    getline(cin >> ws, name);
    cout << "Enter total length L: ";
    cin >> l;
    cout << "Enter index of last name start (N_last): ";
    cin >> n;
    string t = name.substr(n, l - n);
    cout << "Last name: " << t << endl;
    return;
}

int main() {
    cout << "Prob 4.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 4.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 4.3:" << endl;
    prob3();
    cout << endl;
    return 0;
}