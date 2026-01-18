#include <iostream>

using namespace std;

void prob1() {
    int age;
    cout << "Please enter your age: ";
    cin >> age;
    // if age>= 18 { // forget to use () for the condtions
    //     cout << "You are allowed to use this app";
    // }; // do not use ; here, it is syntax error
    // fix:
    if (age >= 18) {
        cout << "You are allowed to use this app";
    } else {
        cout << "You are not allowed to use this app";
    }
    cout << endl;
    return;
}

void prob2() {
    int    age;
    double eyesight;
    cout << "Please input your age: ";
    cin >> age;
    cout << "Please input your eyesight score: ";
    cin >> eyesight;
    if (age >= 18 && eyesight >= 7.5) {
        cout << "Eligible to take the test";
    } else {
        cout << "Not eligible";
    }
    return;
}

void prob3() {
    double a, b, c;
    cout << "Enter coefficients a, b, and c: ";
    cin >> a >> b >> c;
    if (a == 0) {
        if (b == c) {
            cout << "Infinite solutions." << endl;
        } else {
            cout << "No solution." << endl;
        }
    } else {
        double x = (c - b) / a;
        cout << "The solution is: " << x << endl;
    }
    return;
}

int main() {
    cout << "Prob 3.1:" << endl;
    prob1();
    cout << endl;
    cout << "Prob 3.2:" << endl;
    prob2();
    cout << endl;
    cout << "Prob 3.3:" << endl;
    prob3();
    cout << endl;
    return 0;
}